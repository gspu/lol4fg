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
		std::string filename;	
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
		std::string fileComment;
	};


	typedef std::vector<FileEntry> FileEntryVector;
	//typedef std::map<std::string,FileEntry> FileEntryMap; 
	typedef std::map<std::string,size_t> FileEntryPtrMap; 
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
	ZipArchive(std::string filename);

	bool open();
	void close();
	//bool openRead();
	//bool openWrite();

	bool hasFile(std::string filename);

	Buffer getFile(std::string filename);

	bool addFileWithTime(std::string filename, Buffer data, bool overwrite = true, time_t filetime = -1);

	bool addFile(std::string filename, Buffer data, bool overwrite = true);

	bool removeFile(std::string filename);

	
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

	Buffer getFile_index(size_t index);

	//retrieves the file's index, or -1 if file does not exist
	int getFileIndex(std::string filename);

	std::string mFileName;
	uint_4b fileSize;

	std::fstream zipFile;

	bool initZipStructure();

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

	//should return something from ZipState
	//but since zlib stuff returns ints, it would not work with this member being ZipState
	int lastState;

	bool openForRead;
	bool openForWrite;
};