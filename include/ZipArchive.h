#include "Archive.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

class ZipArchive: public Archive
{
public:
	//a 4 byte unsigned int
	typedef unsigned long	uint_4b;
	//a 2 byte unsigned int
	typedef unsigned short	uint_2b;
	//a 1 byte unsigned int
	typedef unsigned char	uint_1b;

	/*
	#define Z_OK            0
	#define Z_STREAM_END    1
	#define Z_NEED_DICT     2
	#define Z_ERRNO        (-1)
	#define Z_STREAM_ERROR (-2)
	#define Z_DATA_ERROR   (-3)
	#define Z_MEM_ERROR    (-4)
	#define Z_BUF_ERROR    (-5)
	#define Z_VERSION_ERROR (-6)
	*/
	enum ZipState
	{
		//original zlib states
		zsOk			=  0,
		zsStreamEnd		=  1,
		zsNeedDict		=  2,
		zsErrNo			= -1,
		zsStreamError	= -2,
		zsDataError		= -3,
		zsMemError		= -4,
		zsBufError		= -5,
		zsVersionError  = -6,
		//my own additions
		//this means, the file is probably not a zip file
		zsHeaderNotFound= -7,
		//requested file was not found in the zip
		zsFileNotFound	= -8,
		//some stuff in the zip is not supported, like, compression != deflated
		zsUnsupported	= -9,
		//the file could not be opened for reading and/or writing
		zsCannotOpen	= -10

	};

	struct FileEntry
	{
		//the index of the entry, 0 = first file in archive
		uint_2b index;
		//offset from the beginning of the file to this entry
		uint_4b headerOffset;
		//size of the stuff before the actual data = the relative offset of the data = 30+filenameLength+extraLength
		uint_2b headerSize;
		////the offset of the compressed data
		//uint_4b dataOffset;

		//the filename as stdstring
		Ogre::String filename;	
		//method. 0 and 8 are the only ones we can use
		uint_2b method;
		//time @todo find out how it works
		uint_2b time;
		//date @todo find out how it works
		uint_2b date;
		//CRC
		uint_4b crc;
		//compressed size = size of the data written into the file
		//total size = compSize + headerSize
		uint_4b compSize;
		//uncompressed size
		uint_4b uncompSize;

		//CENTRAL DIR STUFF HERE

		//offset of the central dir entry
		uint_4b cdOffset;

		//size of the central dir entry
		uint_2b cdSize;

		//how to set ZIPINT
		bool isBinary;

		uint_2b bitflag;

		
	};

	//struct for containing the data at the end of the zip file
	struct Footer
	{
		uint_2b numFiles;
		uint_4b centralDirSize;
		uint_4b centralDirOffset;
		Ogre::String fileComment;
	};


	typedef std::vector<FileEntry> FileEntryVector;
	//typedef std::map<Ogre::String,FileEntry> FileEntryMap; 
	typedef std::map<Ogre::String,size_t> FileEntryPtrMap; 
	//FileEntryMap filesInZip;
	//in here the actual FileEntry objects are stored, in original order
	FileEntryVector filesInZip;
	//and this links filenames to pointers of FileEntry, for faster access by name
	FileEntryPtrMap filePointers;

	Footer mFooter;
	

	static const uint_4b fileEntryHeader;
	static const uint_4b centralDirHeader;
	static const uint_4b finalHeader;
	static const uint_4b bufSize;

	//opens an existing zip file and reads all the necessary stuff into the structs
	ZipArchive(Ogre::String filename,bool createIfNotExists = false,bool overwriteWithEmptyZip=false)
	{
		init(filename,createIfNotExists,overwriteWithEmptyZip);
	}

	bool open();
	void close();
	//bool openRead();
	//bool openWrite();

	bool hasFile(Ogre::String filename);

	Buffer getFile(Ogre::String filename,Buffer::AllocType allocWith = Buffer::atNew, bool nullTerminated = false);

	bool addFileWithTime(Ogre::String filename, Buffer data, bool overwrite = true, time_t filetime = -1);

	bool addFile(Ogre::String filename, Buffer data, bool overwrite = true);

	bool removeFile(Ogre::String filename);

	void doDebugComparison(ZipArchive *other);

protected:
	Ogre::String mFileName;

	//protected constructor that does nothing
	ZipArchive(){}

	/*
	 * actual "constructor"
	 * string filename: the filename (duh)
	 * bool createIfNotExists: whenever the file should be created if it does not exist. if set to false and no file, the function will fail
	 * bool overwriteWithEmptyZip: if this is true and the file exists, it will be overwritten with an empty zip file = all files inside will be deleted
	 * does not return anything, just throws stuff
	 */
	void init(Ogre::String filename,bool createIfNotExists,bool overwriteWithEmptyZip);

	/*
	 * opens the file AND REMOVES FILE CONTENTS
	 * protected, since the class has afterwards no idea that it has become invalidated
	 */
	bool open_truncate();

	//writes content for an empty zip file
	//does not set the file size or the put ptr, just writes
	//also, does not open or close
	void writeEmptyZip();

	bool initZipStructure();

	//should return something from ZipState
	//but since zlib stuff returns ints, it would not work with this member being ZipState
	int lastState;
	
private:
	

	//gets current date as dos time/date
	void getCurrentDate(uint_2b &dosDate, uint_2b &dosTime);
	//converts a time_t to dos time/date
	void convertTimestampToDos(time_t timestamp, uint_2b &dosDate, uint_2b &dosTime);
	//converts dos time/date to a time_t
	time_t convertDosToTimestamp(uint_2b dosDate, uint_2b dosTime);

	//calculated the sizes for the header and CD entry, and writes them into entry
	void updateEntrySizes(FileEntry &entry);

	//removes a file by index
	bool removeFile_index(size_t index);

	Buffer getFile_index(size_t index,Buffer::AllocType allocWith,bool nullTerminated);

	//retrieves the file's index, or -1 if file does not exist
	int getFileIndex(Ogre::String filename);

	
	uint_4b fileSize;

	std::fstream zipFile;

	

	//truncates the file to given size
	//most probably the fstream has to be closed for this to work
	void truncateZipFile(unsigned long newSize);

	//writes a central dir entry for given file entry
	//the zip file has to be open, and it will not be closed by this function
	//writing begins where the put pointer is right now, and it's advanced regularily
	//the data in entry has to be accurate, of course
	void appendCentDirEntry(FileEntry entry);

	//writes the "footer" 
	//the zip file has to be open, and it will not be closed by this function
	//writing begins where the put pointer is right now, and it's advanced regularily
	//the data in mFooter has to be accurate, of course
	void appendFooter();

	//writes the header for given file entry
	//the zip file has to be open, and it will not be closed by this function
	//writing begins where the put pointer is right now, and it's advanced regularily
	//the data in entry has to be accurate, of course
	void appendEntryHeader(FileEntry entry);

	//updates the values for the compressed data size in entry header and central dir
	//necessary, since i can't predict how much data it will be
	//the zip file has to be open, and it will not be closed by this function
	//the put pointer will be repositioned, and left somewhere at the central dir entry
	void updateCompressedSize(FileEntry entry);

	

	//bool openForRead;
	//bool openForWrite;
};