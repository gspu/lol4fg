#ifndef __saveGame
#define __saveGame


#include <Ogre.h>
//#include "zip.h"
//#include "unzip.h"
#include "OgreImageCodec.h"
#include "OgreNewt.h"
#include "ZipArchive.h"

class ZipSaveFile: public ZipArchive
{
public:
	/*struct SaveGameFile
	{
		void *buffer;
		unsigned int size;
	};*/
	//typedef Archive::Buffer
	//mFileName = path+"/"+name+"."+ext;
	ZipSaveFile(Ogre::String name,Ogre::String path,Ogre::String ext,bool createIfNotExists=true,bool overwriteWithEmptyZip=false)
		//:ZipArchive(path+"/"+name+"."+ext,true)
	{
		//openForWrite = true;
		//mZip = NULL;
		mFileName = path+"/"+name+"."+ext;
		mName = name;
		init(mFileName,createIfNotExists,overwriteWithEmptyZip);
		
	}
    ZipSaveFile(Ogre::String fullPath,bool createIfNotExists=true,bool overwriteWithEmptyZip=false)
		//:ZipArchive(fullPath,true)
	{
		//openForWrite = true;
		//mZip = NULL;
		mFileName = fullPath;
        Ogre::String name,ext,path;
        Ogre::StringUtil::splitFullFilename(fullPath,name,ext,path);
		mName = name;
		init(mFileName,createIfNotExists,overwriteWithEmptyZip);
		
	}
	~ZipSaveFile()
	{
		//closeZipFile();
	}

	/*
	re-inits this instance 
	if no new file name given, the one given on construction will be used
	effects are:
	- if the file does not exist, it will be created
	- the file is re-read
	returns: true on success, false on failure (duh)
	*/
	bool reInit(Ogre::String newFileName="");

	bool reInit(Ogre::String name,Ogre::String path,Ogre::String ext)
	{
		reInit(path+"/"+name+"."+ext);
	}
	
	
	
	//adds a text file
	bool addTextFile(Ogre::String fileContents,Ogre::String fileName,bool overwrite = false);

	//adds an image as a file
    bool addImageFile(Ogre::Image img,Ogre::String fileName);
    
	//adds the content of a DataStreamPtr as a file
    //does not close the stream
    bool addFileFromDataStreamPtr(Ogre::DataStreamPtr stream,Ogre::String fileName);

	//gets a file as a DataStreamPtr.
	//if freeOnClose is set to false, then the data must be manually deallocated somewhen 
	bool getFileAsDataStreamPtr(Ogre::DataStreamPtr &stream,Ogre::String fileName,bool freeOnClose = true);
    
	//retrieves a file as a Ogre::String
	Ogre::String getFileAsString(Ogre::String fileName);

	//retrieves a file and tries to process it as an image.
    Ogre::Image getFileAsImage(Ogre::String filename);

	//erases the zip file
	inline void eraseArchive()
	{
		close();
		//closeZipFile();
		int err = remove(mFileName.c_str());
	}
	//renames the zip file
	inline void renameArchive(Ogre::String newFileName)
	{
		close();
		if(MoveFileA(mFileName.c_str(),newFileName.c_str()))
		{
			mFileName = newFileName;
		}
	}

	//renames the zip file
	inline void renameArchive(Ogre::String newName,Ogre::String newPath,Ogre::String newExt)
	{
		renameArchive(newPath+"/"+newName+"."+newExt);
	}

	//copies a file to newFileName
	//bool overwrite: whenever to overwrite if the target file exists
	//bool swichToCopy: if true, this instance will modify the newly-created file after this instead of the one it was constructed with
	inline bool copyArchive(Ogre::String newFileName,bool overwrite,bool swichToCopy = true)
	{
		close();
		
		if(CopyFileA(mFileName.c_str(),newFileName.c_str(),!overwrite))
		{
			if(swichToCopy)
				mFileName = newFileName;
			return true;
		}
		return false;
	}
	inline bool copyArchive(Ogre::String newName,Ogre::String newPath,Ogre::String newExt,bool overwrite,bool swichToCopy = true)
	{
		return copyArchive(newPath+"/"+newName+"."+newExt,overwrite,swichToCopy);
	}
	//wenn write = true, wird die datei zum
	//lesen geöffnet, sonst zum schreiben
	//bool openZipFile(bool write);
	
	//inline void closeZipFile()
	//{
	//	filesAdded.clear();
	//	if(mZip)// && openForWrite)
	//	{
	//		if(openForWrite)
	//			zipClose(mZip,NULL);
	//		else
	//			unzClose(mZip);
	//		//CloseZip(mZip);

	//		mZip = NULL;
	//		
	//	}
	//}

	//copies all files from other. no idea if still needed
	//void copyFilesFrom(ZipSaveFile *other);
	//void readFile(
	Ogre::String getZipFileName()
	{
		return mFileName;
	}

    bool addTreeCollision(OgreNewt::CollisionPrimitives::TreeCollision *col,Ogre::String fileName);
    OgreNewt::CollisionPrimitives::TreeCollision *getTreeCollision(Ogre::String fileName,OgreNewt::World *world);

    static void _tcSerialiseCallback(void* serializeHandle, const void* buffer, size_t size);
    static void _tcDeserialiseCallback(void* deserializeHandle, void* buffer, size_t size);
	
private:
	//bool openForWrite;
	//HZIP mZip;
	//voidp mZip;
	//ZipArchive *mArch;
	//zipFile writeFile;
	//unzFile readFile;//hm wat. die sind doch identisch...
	//Ogre::String mFileName;
	Ogre::String mName;
	//dateien, die per addfile geadded wurden
	//damit man auch im schreibmodus hasFile machen kann
	//Ogre::StringVector filesAdded;
	
protected:
	/*
	 * 
	 */
	//void init(Ogre::String fullPath, bool createIfNotExists = true, bool replaceIfExists = false);
	/*bool addFileNoCheck(void *buf,size_t size,Ogre::String fileName)
	{
		int err = zipOpenNewFileInZip(mZip,fileName.c_str(),0,0,0,0,0,0,0,0);
		if(!err)
		{
			err = zipWriteInFileInZip(mZip,buf,size);
		}		
		return (err == 0);
	}*/




};

#endif