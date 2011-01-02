#include "ZipSaveFile.h"
#include "functions.h"
#include "StandardApplication.h"
//#include "zip.h"
//#include "unzip.h"

using namespace Ogre;

#ifndef NONEWTON
//hilfsklasse
class TC_Helper:public OgreNewt::CollisionPrimitives::TreeCollision
{
public:
    void setNewtonCollision(NewtonCollision* col)
    {
        m_col = col;
    }
};



bool ZipSaveFile::addTreeCollision(OgreNewt::CollisionPrimitives::TreeCollision *col,Ogre::String fileName)
{
	mLog("Error while adding file "+fileName+" to "+mFileName+": serialized Tree Collisions not yet supported");
	return false;
    //if(hasFile(fileName))
    //    return false;
    ////MemoryDataStreamPtr stream;
    //std::stringstream stream;
    //
	

    //NewtonTreeCollisionSerialize(col->getNewtonCollision(),&ZipSaveFile::_tcSerialiseCallback,&stream);
    //
    //addTextFile(stream.str(),fileName);
    //return true;
}
OgreNewt::CollisionPrimitives::TreeCollision *ZipSaveFile::getTreeCollision(Ogre::String fileName,OgreNewt::World *world)
{
	mLog("Error while retrieving file "+fileName+" from "+mFileName+": serialized Tree Collisions not yet supported");
	return NULL;   
	/*Ogre::DataStreamPtr stream;
    if(!getFileAsDataStreamPtr(stream,fileName,true,true))
        return NULL;

    OgreNewt::CollisionPrimitives::TreeCollision *dest = new OgreNewt::CollisionPrimitives::TreeCollision(world);
    
    NewtonCollision* col=NewtonCreateTreeCollisionFromSerialization(world->getNewtonWorld(), NULL, &ZipSaveFile::_tcDeserialiseCallback, &stream);
	
	
	stream->close();
	static_cast<TC_Helper*>(dest)->setNewtonCollision(col);
    return dest;*/
}

void ZipSaveFile::_tcSerialiseCallback(void* serializeHandle, const void* buffer, size_t size)
{
    std::fstream *stream = static_cast<std::fstream*>(serializeHandle);
    stream->write(static_cast<char*>(const_cast<void*>(buffer)),size);

}
void ZipSaveFile::_tcDeserialiseCallback(void* deserializeHandle, void* buffer, size_t size)
{
    Ogre::DataStreamPtr stream = *(static_cast<Ogre::DataStreamPtr*>(deserializeHandle));
    stream->read(buffer,size);
    
}
#endif
//bool ZipSaveFile::openZipFile(bool write)
//{
//	return mArch->open();
//	//if(write)
//	//{
//	//	if(mZip)
//	//	{
//	//		if(openForWrite)
//	//			return true;
//	//		closeZipFile();
//	//	}
//	//	mZip = zipOpen(mFileName.c_str(),APPEND_STATUS_CREATE);
//	//	//mZip = CreateZip(mFileName.c_str(),NULL);
//	//}
//	//else
//	//{
//	//	if(mZip)
//	//	{
//	//		if(!openForWrite)
//	//			return true;
//	//		closeZipFile();
//	//	}
//	//	//mZip = OpenZip(mFileName.c_str(),NULL);
//	//	mZip = unzOpen(mFileName.c_str());
//	//}
//	//openForWrite = write;
//	//return mZip != NULL;
//}

//void ZipSaveFile::copyFilesFrom(ZipSaveFile *other)
//{
//	if(!other)
//		return;
//	other->openZipFile(false);
//	openZipFile(true);
//
//	unz_global_info sourceInfo;
//	int err = unzGetGlobalInfo (other->mZip,&sourceInfo);
//	if (err!=UNZ_OK)
//		return;
//	for (int i=0;i<sourceInfo.number_entry;i++)
//    {
//		char curFileName[256];
//		unz_file_info curFileInfo;
//		err = unzGetCurrentFileInfo(other->mZip,&curFileInfo,curFileName,sizeof(curFileName),0,0,0,0);
//		
//		//roh?
//		
//		err = unzOpenCurrentFile2 (other->mZip,0,0,0);//letzter param = raw
//		int size_buf = curFileInfo.uncompressed_size;
//		void *buf = (void*)malloc(size_buf);
//		err = unzReadCurrentFile(other->mZip,buf,size_buf);
//		
//		
//
//		//nun adden
//		err = zipOpenNewFileInZip2(
//			mZip,
//			curFileName,
//			0,//ach ich kapiers nicht. dann halt nichraw
//			0,0,0,0,0,0,0,0);//roh. der dritte param ist das Zipfi
//		//nun schreiben
//		zipWriteInFileInZip(mZip,buf,size_buf);
//		free(buf);
//		err = unzGoToNextFile(other->mZip);
//        if (err!=UNZ_OK)
//        {
//            return;
//        }
//        
//	}
//
//
//	//ZIPENTRY entry;
//	//int err = GetZipItem(other->mZip, -1, &entry);
//	//int cnt = entry.index;
//	//for(int i=0;i<cnt;i++)
//	//{
//	//	err = GetZipItem(other->mZip, i, &entry);
//	//	if(!err)
//	//	{
//	//		//jetzt prüfen ob ich die schon habe
//	//		//SO GEHT ES NICHT!
//	//		if(!hasFile(entry.name))
//	//		{
//	//			int bufsize = entry.unc_size;
//	//			char *buf = new char[bufsize];
//	//			err = UnzipItem(other->mZip,entry.index,buf,bufsize);
//	//			addFileNoCheck(buf,bufsize,entry.name);
//	//			delete buf;
//	//		}
//	//	}
//	//}
//	other->closeZipFile();
//		
//}

//bool ZipSaveFile::hasFile(Ogre::String fileName)
//{
//	return mArch->hasFile(fileName);
//	//if(mZip && openForWrite)
//	//{
//	//	Ogre::StringUtil::toLowerCase(fileName);
//	//	for(Ogre::StringVector::iterator itr = filesAdded.begin();itr!=filesAdded.end();itr++)
//	//	{	
//	//		Ogre::String cur = *itr;
//	//		Ogre::StringUtil::toLowerCase(cur);
//	//		if(cur == fileName)
//	//		{
//	//			return true;
//	//		}
//	//	}
//	//	return false;
//	//}
//	//else
//	//{
//	//	if (unzLocateFile(mZip,fileName.c_str(),0) != UNZ_OK)
//	//		return false;
//	//	return true;
//	//	/*int index;
//	//	ZIPENTRY info;
//	//	int err = FindZipItem(mZip,fileName.c_str(),true,&index,&info);
//	//	if(err)
//	//		return false;
//	//	return index == -1;*/
//	//}
//}

bool ZipSaveFile::reInit(Ogre::String newFileName)
{
	if(newFileName != "")
		mFileName = newFileName;

	bool shouldWriteEmptyZip = !FileExists(mFileName);
	

	if(!open())
	{
		lastState = zsCannotOpen;//todo: throw something better //return false;
		return false;
	}
	if(shouldWriteEmptyZip)
	{
		writeEmptyZip();
	}

	if(!initZipStructure())
	{
		close();
		return false;
	}
	close();
	return true;
}

bool ZipSaveFile::addTextFile(Ogre::String fileContents,Ogre::String fileName,bool overwrite)
{
	Archive::Buffer temp;
	//temp
	temp.setData(const_cast<char*>(fileContents.c_str()),fileContents.size());
	
	bool success = addFile(fileName,temp,overwrite);		
	//temp.deallocate();
	return success;
	
}

bool ZipSaveFile::addFileFromDataStreamPtr(Ogre::DataStreamPtr stream,Ogre::String fileName)
{
	Buffer buf;
	buf.allocate(stream->size(),Buffer::atMalloc);
    //void *buffer = malloc(stream->size());
    
	stream->read(buf.asVoidPtr(),stream->size());
	bool res = addFile(fileName,buf);
	//delete buffer;
	buf.deallocate();
	return res;
    
}

bool ZipSaveFile::getFileAsDataStreamPtr(Ogre::DataStreamPtr &stream,Ogre::String fileName,bool freeOnClose)
{
    //SaveGameFile res;
	Buffer buff = getFile(fileName,Buffer::atOgreMalloc,false);
	if(buff.isEmpty())
        return false;

	//now memStream is responsible for the data, so buff is NOT being deallocated
	Ogre::MemoryDataStream *memStream = new Ogre::MemoryDataStream(buff.asVoidPtr(),buff.getLength(),freeOnClose);
	//wrap it into a datastreamptr
    Ogre::DataStreamPtr wrapper(memStream);
    stream = wrapper;

    return true;
}

Ogre::String ZipSaveFile::getFileAsString(Ogre::String fileName)
{
	Buffer file = getFile(fileName,Buffer::atNew,true);
	if(file.isEmpty())
		return "";
	
	Ogre::String res(file.asCharPtr());// = (reinterpret_cast<char*>(file.buffer));
	//res += "\0"; <- wat?
	file.deallocate();
	return res;
}

Image ZipSaveFile::getFileAsImage(Ogre::String filename)
{
    /*'__FILE__' and '__LINE__' are predefined macros and part of the C/C++ standard. 
    During preprocessing, they are replaced respectively by a constant string holding the current 
    file name and by a integer representing the current line number.

There are other preprocessor variables including:

    * '__DATE__' -> a string literal of the form "Mmm dd yyyy"

    * '__TIME__' -> a string literal of the form "hh:mm:ss"

    * '__TIMESTAMP__' -> a string literal of the form "Mmm dd yyyy hh:mm:ss"

    * '__FUNCTION__' -> a string literal which contains the function name (this is part of C99, the new C standard and not all C++ compilers support it)*/
    //SaveGameFile file;
	Archive::Buffer file = getFile(filename,Archive::Buffer::atOgreMalloc);
	if(file.isEmpty())
	//if(!getFile(filename,file))
    {
        return Image();
    }
        //throw Ogre::Exception(0,"File \""+filename+"\" not found in zip archive "+mFileName,Ogre::String(__FILE__)+", "+str(__LINE__));
    Image img;
    //size_t test = img.getHeight();

	Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream(file.asVoidPtr(),file.getLength(),false));
    /*Ogre::DataStreamPtr ds(new Ogre::MemoryDataStream(mybuffer, mybuffersize)); */
    Ogre::String base,ext,path;
    Ogre::StringUtil::splitFullFilename(filename,base,ext,path);
    img.load(stream,ext);

    /*stream.
    stream.close();
    stream.setNull();*/
	file.deallocate();
	//free(file.buffer);//versuchen wirs so
	//delete file.buffer;
	return img;
}

 bool ZipSaveFile::addImageFile(Image img,Ogre::String fileName)
{
    Ogre::String base,ext,path;
    Ogre::StringUtil::splitFullFilename(fileName,base,ext,path);
    Codec* pCodec = Codec::getCodec(ext); 
    if(!pCodec)
        return false;
    ImageCodec::ImageData* imgData = new ImageCodec::ImageData(); 
    imgData->format = img.getFormat();
    imgData->height = img.getHeight();
    imgData->width = img.getWidth();
    imgData->depth = img.getDepth(); 
    Codec::CodecDataPtr codeDataPtr(imgData); 
    MemoryDataStreamPtr wrapper(new Ogre::MemoryDataStream(img.getData(), img.getSize(), false)); 
    Ogre::DataStreamPtr stream = pCodec->code(wrapper, codeDataPtr); 
    //stream->
    bool result = addFileFromDataStreamPtr(stream,fileName);
    stream->close();
    return result;
}
