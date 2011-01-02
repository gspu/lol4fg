// ZipArchive.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "ZipArchive.h"
#include <windows.h>
#include "time.h"

#include <iostream>	
#include <sstream>
#include <vector>
//#include <fstream.h>
#include <zlib.h>
#include <assert.h>
#include <time.h>
#include "functions.h"

#define READ_BUFFER 32


//ZipArchive::ZipArchive()
//{
//
//}

const ZipArchive::uint_4b ZipArchive::fileEntryHeader = 0x04034B50;
const ZipArchive::uint_4b ZipArchive::centralDirHeader = 0x02014B50;
const ZipArchive::uint_4b ZipArchive::finalHeader = 0x06054B50;
const ZipArchive::uint_4b ZipArchive::bufSize = 16384;

void ZipArchive::init(std::string filename,bool createIfNotExists,bool overwriteWithEmptyZip)
{
	using namespace std;
	mFileName = filename;
	bool mustWriteEmpty = false;
	if(!FileExists(filename))
	{
		if(!createIfNotExists)
		{
			throw zsCannotOpen;
		}
		mustWriteEmpty = true;
		if(!open())
			throw zsCannotOpen;
	}
	else
	{
		if(overwriteWithEmptyZip)
		{
			if(!open_truncate())
				throw zsCannotOpen;
			mustWriteEmpty = true;
		}
		else
		{
			if(!open())
				throw zsCannotOpen;
		}
	}
	

	if(mustWriteEmpty)
		writeEmptyZip();

	if(!initZipStructure())
	{
		close();
		throw lastState;
	}
	close();
}
//
//ZipArchive::ZipArchive(std::string filename,bool createIfNotExists)
//{
//	
//	using namespace std;
//	mFileName = filename;
//	bool mustWriteEmpty = false;
//	if(!FileExists(filename))
//	{
//		if(!createIfNotExists)
//		{
//			throw zsCannotOpen;
//		}
//		mustWriteEmpty = true;
//	}
//	
//	if(!open())
//		throw zsCannotOpen;
//
//	if(mustWriteEmpty)
//		writeEmptyZip();
//
//	if(!initZipStructure())
//	{
//		close();
//		throw lastState;
//	}
//	close();
//}

bool ZipArchive::initZipStructure()
{
	using namespace std;
	uint_4b footerPos;
	//now try to find the "header" first
	//for a comment-less file, it should be 22 bytes before the end
	
	filesInZip.clear();
	filePointers.clear();
	
	zipFile.seekg(0,ios_base::end);
	fileSize = zipFile.tellg();


	zipFile.seekg(-22,ios_base::end);
	uint_4b locSig;
	zipFile.read(reinterpret_cast<char *>(&locSig),4);

	if(finalHeader == locSig)
	{
		//locSig = finalHeader;
		footerPos = (int)zipFile.tellg()-4;
	}
	else
	{
		//0x06054B50, actually 06 05 4B 50
		//hm, if either 06 or 05 appear anywhere, then it's no longer a string.
		//if these values are actually part of 06 05 4B 50, then I found the header
		//if not, then I can go -22, and start looking from there forwards
		
		//the buffer to store the stuff
		char buffer[READ_BUFFER];
		//by this amount the position will advance towards the beginning of the file
		//there is an overlap of 3 bytes so that it cannot happen that the 4 bytes
		//i'm looking for get separated
		unsigned short positionDecrement = 29;

		//set it to -(32+19) from the end, because I know it's not at -22, but it could
		//be at -23, so I need 3 overlap bytes, again
		zipFile.seekg(-(READ_BUFFER+19),ios_base::end);
		bool finished = false;
		while(!finished)
		{
			//fill my buffer
			zipFile.read(reinterpret_cast<char *>(buffer),READ_BUFFER);
			//now look for the header
			for(unsigned short i = 0;i<29;i++)
			{
				char cur = buffer[i];
				if(cur == 0x50)
				{
					if(buffer[i+1] == 0x4B && buffer[i+2] == 0x05 && buffer[i+3] == 0x06)
					{
						//found ya!
						finished = true;
						footerPos = (int)zipFile.tellg() + i - READ_BUFFER;
						break;
					}
				}
			}
			if(finished)
				break;
			zipFile.seekg(-(positionDecrement+READ_BUFFER),ios_base::cur);
		}

	}

	//now we have the position of the footer
	uint_2b nrOfDisk;
//uint_2b nrOfDiskCentralDir;
	uint_2b numFiles;
	uint_4b centralDirSize;
	uint_4b centralDirOffset;
	uint_2b commentLength;
	std::string comment;

	zipFile.seekg(footerPos+4,ios_base::beg);//go to nrOfDisk
	zipFile.read(reinterpret_cast<char *>(&nrOfDisk),2);
	if(nrOfDisk != 0)
	{
		//not supported
		lastState = zsUnsupported;
		
		return false;
	}
	//skip next 2 bytes
	zipFile.seekg(2,ios_base::cur);
	zipFile.read(reinterpret_cast<char *>(&numFiles),2);

	zipFile.seekg(2,ios_base::cur);
	zipFile.read(reinterpret_cast<char *>(&centralDirSize),4);
	zipFile.read(reinterpret_cast<char *>(&centralDirOffset),4);

	zipFile.read(reinterpret_cast<char *>(&commentLength),2);
	if(commentLength != 0)
	{
		char *tempString = new char[commentLength+1];
		tempString[commentLength] = 0;
		zipFile.read(tempString,commentLength);

		comment = std::string(tempString);

		delete tempString;
	}
	mFooter.centralDirOffset = centralDirOffset;
	mFooter.centralDirSize = centralDirSize;
	mFooter.fileComment = comment;
	mFooter.numFiles = numFiles;

	//now loop through the central dir
	//bool done = false;
	uint_2b curIndex = 0; 
	uint_4b nextHeader = centralDirOffset;
	while(true)//while(!done)
	{
		zipFile.seekg(nextHeader,ios_base::beg);
		uint_4b curSig;
		zipFile.read(reinterpret_cast<char *>(&curSig),4);
		if(curSig != centralDirHeader)
		{
			//done = true;
			break;
		}
		FileEntry curEntry;
		curEntry.cdOffset = nextHeader;
		//go to bitflag
		zipFile.seekg(4,ios_base::cur);
		zipFile.read(reinterpret_cast<char *>(&curEntry.bitflag),2);
		zipFile.read(reinterpret_cast<char *>(&curEntry.method),2);
		zipFile.read(reinterpret_cast<char *>(&curEntry.time),2);
		zipFile.read(reinterpret_cast<char *>(&curEntry.date),2);

		zipFile.read(reinterpret_cast<char *>(&curEntry.crc),4);
		zipFile.read(reinterpret_cast<char *>(&curEntry.compSize),4);
		zipFile.read(reinterpret_cast<char *>(&curEntry.uncompSize),4);
		
		uint_2b filenameLength;
		uint_2b extraLength;
		uint_2b commentLength;

		zipFile.read(reinterpret_cast<char *>(&filenameLength),2);
		zipFile.read(reinterpret_cast<char *>(&extraLength),2);
		zipFile.read(reinterpret_cast<char *>(&commentLength),2);
		//skip the next 2 bytes
		zipFile.seekg(2,ios_base::cur);

		uint_2b intAbb = 0;
		zipFile.read(reinterpret_cast<char *>(&intAbb),2);
		uint_2b temp = intAbb & 0x0001;
		curEntry.isBinary = (temp == 0);

		zipFile.seekg(4,ios_base::cur);

		zipFile.read(reinterpret_cast<char *>(&curEntry.headerOffset),4);

		//next is filename
		char *tempBuffer = new char[filenameLength+1];
		tempBuffer[filenameLength] = 0;
		zipFile.read(tempBuffer,filenameLength);
		curEntry.filename = std::string(tempBuffer);
		delete tempBuffer;

		curEntry.cdSize = 46+filenameLength+extraLength+commentLength;

		zipFile.seekg(extraLength+commentLength,ios_base::cur);

		//I still need headerSize
		curEntry.headerSize = 30 + filenameLength + extraLength;

		nextHeader = zipFile.tellg();

		

		curEntry.index = curIndex;
		//filesInZip.insert((std::string)curEntry.filename,curEntry);
		filesInZip.push_back(curEntry);
		filePointers[curEntry.filename] = curIndex;//&filesInZip[curIndex];
		//filesInZip[curEntry.filename] = curEntry;
		//fileEntries.push_back(curEntry);
		curIndex++;
		//cdEntry size is 46+filenameLength+extraLength+commentLength
	}
	return true;
}

void ZipArchive::writeEmptyZip()
{
	//write finalHeader (4b) and 12 zero bytes
	zipFile.write(reinterpret_cast<char*>(const_cast<uint_4b*>(&finalHeader)),4);
	for(unsigned short i=0;i<12;i++)
	{
		zipFile.write("\0",1);
	}
}

bool ZipArchive::open_truncate()
{
	if(zipFile.is_open())
	{
		close();
	}
	//open for everything AND TRUNCATE
	zipFile.open(mFileName.c_str(),std::ios_base::binary|std::ios_base::in|std::ios_base::out|std::ios_base::trunc);
	 
	if(zipFile.fail())
		return false;
	return true;
}

bool ZipArchive::open()
{
	if(zipFile.is_open())
		return true;
	//open for everything
	zipFile.open(mFileName.c_str(),std::ios_base::binary|std::ios_base::in|std::ios_base::out);
	 
	if(zipFile.fail())
		return false;
	return true;
}

void ZipArchive::close()
{
	if(!zipFile.is_open())
		return;
	zipFile.close();
}


void ZipArchive::truncateZipFile(unsigned long newSize)
{
	using namespace std;
	if(newSize == fileSize)
		return;
	if ( fileSize > newSize )
	{
		//MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,fileSize.c_str(),-1,
		//this is the difficult part. it seems there is no "standard" way of file truncating
		HANDLE winFile = CreateFileA(mFileName.c_str(),GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		SetFilePointer(winFile,newSize,NULL,FILE_BEGIN);
		SetEndOfFile(winFile);
		CloseHandle(winFile); 
	}
	else
	{
		//this is easy.
		zipFile.open(mFileName.c_str(),ios::out | ios::binary | ios::app);

		for ( unsigned int i = fileSize; i < newSize; i++ )
		{
			zipFile.write("\0",1);
		}
		zipFile.close();
	}
	fileSize = newSize;
}

Archive::Buffer ZipArchive::getFile(std::string filename,Buffer::AllocType allocWith, bool nullTerminated)
{
	int index = getFileIndex(filename);
	if(index == -1)
	{
		lastState = zsFileNotFound;
		return Buffer();
	}
	return getFile_index(index,allocWith,nullTerminated);
}
Archive::Buffer ZipArchive::getFile_index(size_t index,Buffer::AllocType allocWith, bool nullTerminated)
{
	using namespace std;
	open();
	Buffer result;
	
	//now try to extract the file
	FileEntry curEntry = filesInZip[index];
	

	//from testzip
	lastState = zsOk;
    uInt iRead = 0;
   

	

	z_stream stream;
	//Buffer buf;

	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;
	stream.opaque = (voidpf)0;
	stream.next_in = 0;
	stream.avail_in = (uInt)0;
	stream.total_out = (uLong)0;
	stream.total_in = (uLong)0;
	stream.msg = 0;
	stream.state = 0;


	//const unsigned int UNZ_BUFSIZE = 16384;
//	const unsigned int MAX_WBITS = 15;


	//unzip INIT stuff
	lastState = inflateInit2(&stream, -MAX_WBITS);
	if (lastState != zsOk)		
	{
		return Buffer();
	}
	/* windowBits is passed < 0 to tell that there is no zlib header.
	 * Note that in this case inflate *requires* an extra "dummy" byte
	 * after the compressed stream in order to complete decompression and
	 * return Z_STREAM_END.
	 * In unzip, i don't wait absolutely Z_STREAM_END because I known the
	 * size of both compressed and uncompressed data
	 */
	
	unsigned int rest_read_compressed = curEntry.compSize;
	unsigned int rest_read_uncompressed = curEntry.uncompSize;


	unsigned int pos_in_zipfile = 0; //im original, das ist die position vom anfang der zipdatei	
	//INIT stuff end


	Buffer readBuffer;
	readBuffer.allocate(bufSize,allocWith);
	unsigned long curCrc32 = 0;

	
	
	
	uint_4b resBufSize = curEntry.uncompSize;
	if(nullTerminated)
		resBufSize++;
	result.allocate(resBufSize,allocWith);

	stream.next_out = result.asUCharPtr();//pointer wohin schreiben
	stream.avail_out = result.getLength();//wie viel dorthin schreiben

	////das sind nun die z_stream sachen!
 


	bool needDummyByte = false;
	zipFile.seekg(curEntry.headerOffset+curEntry.headerSize,std::ios_base::beg);
	while (stream.avail_out>0)
    {
		//das kümmert sich wohl darum, dass next_in gefüllt ist
        if ((stream.avail_in==0) &&     //nothing more available at next_in
            (rest_read_compressed>0))   //but still rest stuff to be read
        {
            uInt uReadThis = bufSize; //uReadThis scheint anzahl der zu lesenden bytes zu sein
            if (rest_read_compressed<uReadThis)
			{
				needDummyByte = true;
                uReadThis = (uInt)rest_read_compressed; //wenn rest < als buffergröße, dann nur so viel wie rest
			}
            if (uReadThis == 0)
                //return UNZ_EOF;//nichts mehr zu lesen, fertig
				break;
			

			zipFile.read(readBuffer.asCharPtr(),uReadThis);
			if(needDummyByte)
			{
				readBuffer.asUCharPtr()[uReadThis+1] = 0;//[uReadThis+1] = 0;
			}



            //position in der datei advancen
            pos_in_zipfile += uReadThis;

            //rest verringern
            rest_read_compressed-=uReadThis;

            //next_in an das ding setzen, welches vorhin gelesen wurde
			stream.next_in = readBuffer.asUCharPtr();
                
            //avail_in ist die anzahl der bytes in next_in
			if(needDummyByte)
			{
				stream.avail_in = (uInt)uReadThis+1;
			}
			else
				stream.avail_in = (uInt)uReadThis;


			//ACTUAL UNZIPPING BEGIN
            unsigned long uTotalOutBefore,uTotalOutAfter;
            const Bytef *bufBefore;
            unsigned long uOutThis;
            int flush=Z_SYNC_FLUSH;

            
            //so viel output
            uTotalOutBefore = stream.total_out;
            //da wird output reingeschrieben
            bufBefore = stream.next_out;

			//yay, inflate!
            lastState=inflate(&stream,flush);

            if ((lastState >= 0) && (stream.msg!=NULL))
				lastState = zsDataError;

			//uTotalOutAfter so viel output haben wir nun
            uTotalOutAfter = stream.total_out; // total_out so much output so far
            uOutThis = uTotalOutAfter-uTotalOutBefore; //uOutThis = so viele bytes haben wir grad gelesen

            //pfile_in_zip_read_info->total_out_64 = pfile_in_zip_read_info->total_out_64 + uOutThis; //da drin it das wie viel wir nun insgs. haben?

            curCrc32 = crc32(curCrc32,bufBefore,
                        (uInt)(uOutThis));//das berechnet das crc32, aus der vorherigen crc und den nächsten daten. cool dass sowas geht

            rest_read_uncompressed -= uOutThis; //anz. noch zu lesender bytes decrementieren

            iRead += uOutThis;//(uInt)(uTotalOutAfter - uTotalOutBefore);// eigentlich += uOutThis. wtf tun sie hier?

			if (lastState==zsStreamEnd)
				break;
                //return (iRead==0) ? UNZ_EOF : iRead;
           /* if (err!=Z_OK)
                break;*/


        }
		
	}

	readBuffer.deallocate();
	
	if(nullTerminated)
		result[resBufSize-1]='\0';
	
	close();
	return result;//Buffer();
}

bool ZipArchive::hasFile(std::string filename)
{
	return getFileIndex(filename) != -1;
}

int ZipArchive::getFileIndex(std::string filename)
{
	Archive::Buffer result;
	FileEntryPtrMap::iterator cur = filePointers.find(filename);

	if(cur == filePointers.end())
	{
		lastState = zsFileNotFound;
		return -1;
	}

	return cur->second;
}



void ZipArchive::getCurrentDate(uint_2b &dosDate, uint_2b &dosTime)
{
	time_t seconds = time(NULL);
	//okay, i'll do this myself
	
	convertTimestampToDos(seconds,dosDate,dosTime);
}

void ZipArchive::convertTimestampToDos(time_t timestamp, uint_2b &dosDate, uint_2b &dosTime)
{
	tm *timeinfo = localtime(&timestamp);

	dosTime = 0;
	dosDate = 0;

	//it seems i have to divide seconds by 2
	if(timeinfo->tm_sec > 59)
		timeinfo->tm_sec = 59;
	uint_2b temp = timeinfo->tm_sec/2;

	dosTime |= temp;

	//now minutes
	temp = timeinfo->tm_min;
	//shift it
	temp = temp << 5;
	dosTime |= temp;

	//hours
	temp = timeinfo->tm_hour;
	//shift it
	temp = temp << 11;
	dosTime |= temp;

	//day
	temp = timeinfo->tm_mday;
	dosDate |= temp;

	//month
	temp = timeinfo->tm_mon+1;
	temp = temp << 5;
	dosDate |= temp;

	//year
	temp = (timeinfo->tm_year-80);
	temp = temp << 9;
	dosDate |= temp;
}

time_t ZipArchive::convertDosToTimestamp(uint_2b dosDate, uint_2b dosTime)
{
	/*
	seems to be like:
	time:
		 0 -  4: second
		 5 - 10: minute
		11 - 15: hour
	date:
		0 -  4: day
		5 -  8: month
		9 - 15: year
	*/

	//mask for second:	           11111 = 31
	//mask for minute:       11111100000 = 2016
	//mask for hour:	1111100000000000 = 63488

	//mask for day:                11111 = 31
	//mask for month:          111100000 = 480
	//mask for year:	1111111000000000 = 65024
	uint_2b secMask		= 31;
	uint_2b minMask		= 2016;
	uint_2b hourMask	= 63488;

	uint_2b dayMask		= 31;
	uint_2b monthMask	= 480;
	uint_2b yearMask	= 65024;


	time_t rawtime;
    tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	timeinfo->tm_sec  = (dosTime & secMask)*2;//no shifting here
	timeinfo->tm_min  = (dosTime & minMask) >> 5;
	timeinfo->tm_hour = (dosTime & hourMask) >> 11;

	timeinfo->tm_mday = dosDate & dayMask;
	timeinfo->tm_mon = ((dosDate & monthMask) >> 5) - 1;
	timeinfo->tm_year = ((dosDate & yearMask) >> 9) + 80;	

	return mktime (timeinfo);
}

bool ZipArchive::addFileWithTime(std::string filename, Buffer data, bool overwrite, time_t filetime)
{
	if(!open())
		return false;
	
	int index = getFileIndex(filename);
	if(index != -1)
	{
		if(overwrite)
		{
			if(!removeFile_index(index))
				return false;
		}
		else
			return false;
	}


	//before I do anything, I need to fill some stuff in the FileEntry
	FileEntry newFile;
	newFile.filename = filename;
	newFile.crc = crc32(0,data.asUCharPtr(),data.getLength());
	newFile.isBinary = true;
	newFile.method = 8;
	newFile.bitflag = 0;
	newFile.uncompSize = data.getLength();
	newFile.headerOffset = mFooter.centralDirOffset;
	if(filetime == -1)
	{
		getCurrentDate(newFile.date,newFile.time);
	}
	else
	{
		convertTimestampToDos(filetime,newFile.date,newFile.time);
	}
	updateEntrySizes(newFile);
	


	//const int MAX_WBITS = 15 ;
	const int DEF_MEM_LEVEL = 8;

	//FROM ZPIPE
	int ret, flush;
    unsigned have;
    z_stream strm;
    //unsigned char in[bufSize];
    //unsigned char out[bufSize];
	Buffer compressedBuffer;
	compressedBuffer.allocate(bufSize,defaultAllocType);
	int level = Z_DEFAULT_COMPRESSION;

	//prepare writing. seek to begin of central dir
	
	zipFile.seekp(mFooter.centralDirOffset);

	//write the header
	appendEntryHeader(newFile);


	//std::fstream tempFile;
	//tempFile.open("temp",std::ios_base::binary|std::ios_base::in|std::ios_base::out|std::ios_base::trunc);

    /* allocate deflate state */
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
	ret = deflateInit2(&strm, level, Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);
	
	if (ret != zsOk)
        return ret;

    /* compress until end of file */
	//i have all in the buffer, I don't need the loop
    //do {
		//nr of bytes
	strm.avail_in = data.getLength();//fread(in, 1, CHUNK, source);
        /*if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }*/
    flush = Z_FINISH;//feof(source) ? Z_FINISH : Z_NO_FLUSH;
	//ptr
	strm.next_in = data.asUCharPtr();//in;

	uint_4b bytesWritten = 0;
    /* run deflate() on input until output buffer not full, finish
       compression if all of source has been read in */
    do {
        strm.avail_out = bufSize;
        strm.next_out = compressedBuffer.asUCharPtr();//out;
        ret = deflate(&strm, flush);    /* no bad return value */
        assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
        have = bufSize - strm.avail_out;
		zipFile.write(compressedBuffer.asCharPtr(),have);
		bytesWritten += have;
		//tempFile.write(compressedBuffer.asCharPtr(),have);
        /*if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }*/
    } while (strm.avail_out == 0);
    assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    //} while (flush != Z_FINISH);
	assert(ret == zsStreamEnd);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);

	////DO TEST:
	//uint_4b newSize2 = zipFile.tellp();
	//close();
	//truncateZipFile(newSize2);
	//return true;

	//and now, rebuild CD and write footer
	newFile.compSize = bytesWritten;
	//now I have all except cdoffset and index
	newFile.index = filesInZip.size();
	filesInZip.push_back(newFile);//
	//filesInZip[newFile.index] = newFile;
	filePointers[newFile.filename] = newFile.index;

	mFooter.centralDirOffset = zipFile.tellp();
	mFooter.centralDirSize = 0;
	for(size_t i=0;i<filesInZip.size();i++)
	{
		/*if(index == newFile.index)
		{*/
			//update the cdOffset. the size should be correct
			filesInZip[i].cdOffset = zipFile.tellp();
		//}
		appendCentDirEntry(filesInZip[i]);
	}
	mFooter.centralDirSize = (int)zipFile.tellp() - mFooter.centralDirOffset;



	mFooter.numFiles++;

	//and now, the footer has to be rebuilt
	appendFooter();

	uint_4b newSize = zipFile.tellp();
	close();
	if(fileSize > newSize)
		truncateZipFile(newSize);
	fileSize = newSize;
	



	return true;
}

bool ZipArchive::addFile(std::string filename, Buffer data, bool overwrite)
{
	return addFileWithTime(filename,data,overwrite,-1);
}


void ZipArchive::updateEntrySizes(FileEntry &entry)
{
	entry.headerSize = 30 + entry.filename.size();
	entry.cdSize = 46 + entry.filename.size();
	//header size = 1E+filename length
	//CD size: 2E + filename length
}


bool ZipArchive::removeFile(std::string filename)
{	
	int index = getFileIndex(filename);
	if(index == -1)
	{
		lastState = zsFileNotFound;
		return false;
	}
	return removeFile_index(index);
}

bool ZipArchive::removeFile_index(size_t ind)
{
	using namespace std;
	if(!open())
		return false;

	FileEntry curEntry = filesInZip[ind];




	//saving the position here, since it seems you have to seek every time when read/write changes
	uint_4b getPos = 0;
	uint_4b putPos = 0;

	Buffer result;
	Buffer tempBuffer;
	//i have no idea what is a good size for a temp buffer, just take bufSize
	//const unsigned int UNZ_BUFSIZE = 16384;
	tempBuffer.allocate(bufSize,defaultAllocType);
	
	//this is the gap which needs filling
	uint_4b gap = curEntry.headerSize+curEntry.compSize;

	//zipFile.seekp(curEntry.headerOffset,ios_base::beg);
	putPos = curEntry.headerOffset;//zipFile.tellp();	
	//seek the get ptr to after the gap
	//zipFile.seekg(curEntry.headerOffset+gap,ios_base::beg);
	getPos = curEntry.headerOffset+gap;//zipFile.tellg();
	//this much bytes are moved in one loop iteration
	uint_4b readPerItr = bufSize;	
	//I cant move more bytes than 'gap' at a time
	if(readPerItr > gap)
		readPerItr = gap;

	uint_4b bytesLeft = mFooter.centralDirOffset - (curEntry.headerOffset+gap);
	bool test;
	while(bytesLeft > 0)
	{
		if(readPerItr > bytesLeft)
			readPerItr = bytesLeft;
		test = zipFile.fail();
		//seek to saved position
		zipFile.seekg(getPos,ios_base::beg);
		//read into the buffer
		zipFile.read(tempBuffer.asCharPtr(),readPerItr);

		test = zipFile.fail();
		//seek to saved position
		zipFile.seekp(putPos,ios_base::beg);

		//zipFile.seekp(curEntry.headerOffset,ios_base::beg);
		//write it into the tempfile
		zipFile.write(tempBuffer.asCharPtr(),readPerItr);


		test = zipFile.fail();

		bytesLeft -= readPerItr;
		//advance the positions
		getPos += readPerItr;
		putPos += readPerItr;
	}
	//now, get should be at the old centralDirOffset, and put where the new one should be created

	//update the positions again
	zipFile.seekg(getPos,ios_base::beg);
	zipFile.seekp(putPos,ios_base::beg);



	//
	////now, the part curEntry.headerOffset to curEntry.headerOffset+bytesToRemove is to be left out
	////this is the header+data of the file
	uint_4b bytesToRemove = curEntry.headerSize+curEntry.compSize;


	////now we are at the end of the data which should be removed. 
	////now copy again until mFooter.centralDirOffset
	//

	uint_4b newCentralDirSize = mFooter.centralDirSize - curEntry.cdSize;
	uint_4b newCentralDirOffset = mFooter.centralDirOffset - bytesToRemove;


	//now rebuilding the CD, and also assigning new indices
	//maybe combine it with the upper loop?
	FileEntryVector::iterator curEntryItr;//iterator of the current entry, in order to erase it
	for(FileEntryVector::iterator itr = filesInZip.begin();itr!=filesInZip.end();itr++)
	{		
		//FileEntry *entryPtr = &(*itr);//&filesInZip[i];

		uint_2b curIndex = itr->index;

		if ( curIndex < curEntry.index )
		{
			itr->cdOffset -= bytesToRemove;
		}
		else if ( curIndex > curEntry.index )
		{
			itr->headerOffset -= bytesToRemove;
			itr->cdOffset -= (bytesToRemove+curEntry.cdSize);
		}
		else
		{
			curEntryItr = itr;
		}
	}
	//also, remove the current entry
	filesInZip.erase(curEntryItr);
	filePointers.erase(curEntry.filename);

	
	uint_2b index = 0;
	for(FileEntryVector::iterator itr = filesInZip.begin();itr!=filesInZip.end();itr++)
	{
		itr->index = index;
		appendCentDirEntry(*itr);
		////signature
		//zipFile.write(reinterpret_cast<char*>(const_cast<uint_4b*>(&centralDirHeader)),4);
		////version made by. no idea what this is...
		////winrar says 0x14
		//uint_1b tempByte = 0;
		//uint_2b tempTwoBytes = 0;
		//uint_4b tempFourBytes = 0;
		//zipFile.write(reinterpret_cast<char*>(&tempByte),1);
		////host OS
		//zipFile.write(reinterpret_cast<char*>(&tempByte),1);
		////Version needed to extract. 20 seems to be a good value
		//tempByte = 20;
		//zipFile.write(reinterpret_cast<char*>(&tempByte),1);
		////O/S of version needed for extraction 
		//tempByte = 0;
		//zipFile.write(reinterpret_cast<char*>(&tempByte),1);
		////General purpose bit flag. the documentation says it has to be 0 if method != 6
		////I encountered it to be 2, but winrar writes 0 here 
		//zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
		////method
		//zipFile.write(reinterpret_cast<char*>(&itr->method),2);
		////time
		//zipFile.write(reinterpret_cast<char*>(&itr->time),2);
		////date
		//zipFile.write(reinterpret_cast<char*>(&itr->date),2);
		////CRC
		//zipFile.write(reinterpret_cast<char*>(&itr->crc),4);
		////comp. size
		//zipFile.write(reinterpret_cast<char*>(&itr->compSize),4);
		////unc size
		//zipFile.write(reinterpret_cast<char*>(&itr->uncompSize),4);
		////filename length
		//uint_2b fnSize = itr->filename.size();
		//zipFile.write(reinterpret_cast<char*>(&fnSize),2);
		////extra length
		//tempTwoBytes = 0;
		//zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
		////comment length?
		//zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
		////Disk number start
		//zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
		////internal attribs
		//uint_2b internalAttributes = 0;
		//if(!itr->isBinary)
		//{
		//	//is ASCII
		//	internalAttributes = 1;
		//}
		//zipFile.write(reinterpret_cast<char*>(&internalAttributes),2);

		////external os-dependant attributes. absolutely no idea what to write here, so i'll just write 4 zeroes
		////winrar writes 20 00 FF 81
		//tempFourBytes = 0;
		//zipFile.write(reinterpret_cast<char*>(&tempFourBytes),4);
		////offset of file entry
		//zipFile.write(reinterpret_cast<char*>(&itr->headerOffset),4);
		////filename
		//zipFile.write(itr->filename.c_str(),fnSize);
		////now would come extra and comment, but I just won't write them, as I set their sizes to 0

		index++;
	}


	

	//update the footer values
	mFooter.centralDirSize = newCentralDirSize;
	mFooter.centralDirOffset = newCentralDirOffset;
	mFooter.numFiles--;

	//and now, the footer has to be rebuilt
	appendFooter();
	////signature, 4 bytes
	//zipFile.write(reinterpret_cast<char*>(const_cast<uint_4b*>(&finalHeader)),4);
	////nr of disk and nr of disk with start central dir or so, each 2 bytes
	////just write 4 zeroes
	//zipFile.write("\0\0\0\0",4);
	////nr of files, 2 bytes
	//
	//zipFile.write(reinterpret_cast<char*>(&mFooter.numFiles),2);
	////nr of total entries in CD. the same, I guess
	//zipFile.write(reinterpret_cast<char*>(&mFooter.numFiles),2);
	////CD size, 4 bytes
	//zipFile.write(reinterpret_cast<char*>(&newCentralDirSize),4);
	////CD offset, 4 bytes
	//zipFile.write(reinterpret_cast<char*>(&newCentralDirOffset),4);
	////comment length, 2 bytes
	//if(mFooter.fileComment.size() > 0)
	//{
	//	uint_2b length = mFooter.fileComment.size();
	//	zipFile.write(reinterpret_cast<char*>(&length),2);//and nothing more
	//	zipFile.write(mFooter.fileComment.c_str(),length);
	//}
	//else
	//{
	//	zipFile.write("\0\0",2);//and nothing more
	//}


	//and now, truncate the file
	uint_4b newSize = zipFile.tellp();
	close();
	truncateZipFile(newSize);

	tempBuffer.deallocate();
	
	return true;
}

void ZipArchive::updateCompressedSize(FileEntry entry)
{
	//seek to comp.size in header
	zipFile.seekp(entry.headerOffset + 0x12,std::ios_base::beg);
	//write the number
	zipFile.write(reinterpret_cast<char*>(&entry.compSize),4);

	//seek to comp.size of centraldir
	zipFile.seekp(entry.cdOffset + 0x14,std::ios_base::beg);
	//write the number, again
	zipFile.write(reinterpret_cast<char*>(&entry.compSize),4);

}

void ZipArchive::appendEntryHeader(FileEntry entry)
{
	uint_1b tempByte = 0;
	uint_2b temp2Bytes = 0;
	uint_4b tempFourBytes = 0;
	//signature, 4 bytes
	zipFile.write(reinterpret_cast<char*>(const_cast<uint_4b*>(&fileEntryHeader)),4);
	//Version needed to extract. 20 seems to be a good value
	//here suddenly two bytes?!
	temp2Bytes = 20;
	zipFile.write(reinterpret_cast<char*>(&temp2Bytes),2);
	//general purpose bit flag
	temp2Bytes = 0;
	zipFile.write(reinterpret_cast<char*>(&temp2Bytes),2);
	//method
	zipFile.write(reinterpret_cast<char*>(&entry.method),2);
	//time
	zipFile.write(reinterpret_cast<char*>(&entry.time),2);
	//date
	zipFile.write(reinterpret_cast<char*>(&entry.date),2);
	//CRC
	zipFile.write(reinterpret_cast<char*>(&entry.crc),4);
	//comp. size
	//unfortunately, it seems I cannot know that yet. I'll guess I'll have to update it afterwards
	zipFile.write(reinterpret_cast<char*>(&entry.compSize),4);
	//unc size
	zipFile.write(reinterpret_cast<char*>(&entry.uncompSize),4);
	//filename length
	uint_2b fnSize = entry.filename.size();
	zipFile.write(reinterpret_cast<char*>(&fnSize),2);
	//extra length
	temp2Bytes = 0;
	zipFile.write(reinterpret_cast<char*>(&temp2Bytes),2);
	//filename
	zipFile.write(entry.filename.c_str(),fnSize);

}

void ZipArchive::appendFooter()
{
	//signature, 4 bytes
	zipFile.write(reinterpret_cast<char*>(const_cast<uint_4b*>(&finalHeader)),4);
	//nr of disk and nr of disk with start central dir or so, each 2 bytes
	//just write 4 zeroes
	zipFile.write("\0\0\0\0",4);
	//nr of files, 2 bytes
	
	zipFile.write(reinterpret_cast<char*>(&mFooter.numFiles),2);
	//nr of total entries in CD. the same, I guess
	zipFile.write(reinterpret_cast<char*>(&mFooter.numFiles),2);
	//CD size, 4 bytes
	zipFile.write(reinterpret_cast<char*>(&mFooter.centralDirSize),4);
	//CD offset, 4 bytes
	zipFile.write(reinterpret_cast<char*>(&mFooter.centralDirOffset),4);
	//comment length, 2 bytes
	if(mFooter.fileComment.size() > 0)
	{
		uint_2b length = mFooter.fileComment.size();
		zipFile.write(reinterpret_cast<char*>(&length),2);//and nothing more
		zipFile.write(mFooter.fileComment.c_str(),length);
	}
	else
	{
		zipFile.write("\0\0",2);//and nothing more
	}
}

void ZipArchive::appendCentDirEntry(FileEntry entry)
{
	//signature
	zipFile.write(reinterpret_cast<char*>(const_cast<uint_4b*>(&centralDirHeader)),4);
	//version made by. no idea what this is...
	//winrar says 0x14
	uint_1b tempByte = 0;
	uint_2b tempTwoBytes = 0;
	uint_4b tempFourBytes = 0;
	zipFile.write(reinterpret_cast<char*>(&tempByte),1);
	//host OS
	zipFile.write(reinterpret_cast<char*>(&tempByte),1);
	//Version needed to extract. 20 seems to be a good value
	tempByte = 20;
	zipFile.write(reinterpret_cast<char*>(&tempByte),1);
	//O/S of version needed for extraction 
	tempByte = 0;
	zipFile.write(reinterpret_cast<char*>(&tempByte),1);
	//General purpose bit flag. the documentation says it has to be 0 if method != 6
	//I encountered it to be 2, but winrar writes 0 here 
	zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
	//method
	zipFile.write(reinterpret_cast<char*>(&entry.method),2);
	//time
	zipFile.write(reinterpret_cast<char*>(&entry.time),2);
	//date
	zipFile.write(reinterpret_cast<char*>(&entry.date),2);
	//CRC
	zipFile.write(reinterpret_cast<char*>(&entry.crc),4);
	//comp. size
	zipFile.write(reinterpret_cast<char*>(&entry.compSize),4);
	//unc size
	zipFile.write(reinterpret_cast<char*>(&entry.uncompSize),4);
	//filename length
	uint_2b fnSize = entry.filename.size();
	zipFile.write(reinterpret_cast<char*>(&fnSize),2);
	//extra length
	tempTwoBytes = 0;
	zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
	//comment length?
	zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
	//Disk number start
	zipFile.write(reinterpret_cast<char*>(&tempTwoBytes),2);
	//internal attribs
	uint_2b internalAttributes = 0;
	if(!entry.isBinary)
	{
		//is ASCII
		internalAttributes = 1;	
	}
	zipFile.write(reinterpret_cast<char*>(&internalAttributes),2);

	//external os-dependant attributes. absolutely no idea what to write here, so i'll just write 4 zeroes
	//winrar writes 20 00 FF 81
	tempFourBytes = 0;
	zipFile.write(reinterpret_cast<char*>(&tempFourBytes),4);
	//offset of file entry
	zipFile.write(reinterpret_cast<char*>(&entry.headerOffset),4);
	//filename
	zipFile.write(entry.filename.c_str(),fnSize);
	//now would come extra and comment, but I just won't write them, as I set their sizes to 0
}

void ZipArchive::doDebugComparison(ZipArchive *other)
{
	bool dummy = true;
	dummy = mFooter.centralDirOffset == other->mFooter.centralDirOffset;
	dummy = mFooter.centralDirSize == other->mFooter.centralDirSize;
	dummy = mFooter.fileComment == other->mFooter.fileComment;
	dummy = mFooter.numFiles == other->mFooter.numFiles;
	
	size_t numFiles = filesInZip.size();
	dummy = numFiles == other->filesInZip.size();
	for(size_t i=0;i<numFiles;i++)
	{
		FileEntry my = filesInZip[i];
		FileEntry his= other->filesInZip[i];
		//the index of the entry, 0 = first file in archive
		dummy = (my.index == his.index);
		//offset from the beginning of the file to this entry
		dummy = (my.headerOffset == his.headerOffset);
		//size of the stuff before the actual data = the relative offset of the data = 30+filenameLength+extraLength
		dummy = (my.headerSize == his.headerSize);
		////the offset of the compressed data
		//uint_4b dataOffset;

		//the filename as stdstring
		dummy = (my.filename == his.filename);	
		//method. 0 and 8 are the only ones we can use
		dummy = (my.method == his.method);
		//time @todo find out how it works
		dummy = (my.time == his.time);
		//date @todo find out how it works
		dummy = (my.date == his.date);
		//CRC
		dummy = (my.crc == his.crc);
		//compressed size = size of the data written into the file
		//total size = compSize + headerSize
		dummy = (my.compSize == his.compSize);
		//uncompressed size
		dummy = (my.uncompSize == his.uncompSize);

		//CENTRAL DIR STUFF HERE

		//offset of the central dir entry
		dummy = (my.cdOffset == his.cdOffset);

		//size of the central dir entry
		dummy = (my.cdSize == his.cdSize);

		//how to set ZIPINT
		dummy = (my.isBinary == his.isBinary);

		dummy = (my.bitflag == his.bitflag);
	}
	
	/*
	FileEntryVector filesInZip;
	//and this links filenames to pointers of FileEntry, for faster access by name
	FileEntryPtrMap filePointers;

	Footer mFooter;
	*/
}