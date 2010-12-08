#ifndef __saveGame
#define __saveGame


#include "Ogre.h"
#include "zip.h"
#include "unzip.h"
#include "OgreImageCodec.h"
#include "OgreNewt.h"

class ZipSaveFile
{
public:
	struct SaveGameFile
	{
		void *buffer;
		unsigned int size;
	};
	//mFileName = path+"/"+name+"."+ext;
	ZipSaveFile(Ogre::String name,Ogre::String path,Ogre::String ext)
	{
		openForWrite = true;
		mZip = NULL;
		mFileName = path+"/"+name+"."+ext;
		mName = name;
		
	}
    ZipSaveFile(Ogre::String fullPath)
	{
		openForWrite = true;
		mZip = NULL;
		mFileName = fullPath;
        Ogre::String name,ext,path;
        Ogre::StringUtil::splitFullFilename(fullPath,name,ext,path);
		mName = name;
		
	}
	~ZipSaveFile()
	{
		closeZipFile();
	}
	
	
	
	
	//methoden zum Datei adden. wenn nicht offen, wird es aufgemacht
	//wenn schon vorhanden, wird nichts geadded
	bool addFile(void *buf,size_t size,Ogre::String fileName)
	{
		if(hasFile(fileName))
			return false;
		if(!openZipFile(true))
			return false;
		return addFileNoCheck(buf,size,fileName);
		
		
	}
	inline bool addTextFile(Ogre::String fileContents,Ogre::String fileName)
	{
		return addFile(reinterpret_cast<void*>(const_cast<char*>(fileContents.c_str())),fileContents.size(),fileName);		
		
	}
    bool addImageFile(Ogre::Image img,Ogre::String fileName);
    //added datei aus einen Ogre::DataStreamPtr
    //schließt den stream nicht!
    bool addFileFromDataStreamPtr(Ogre::DataStreamPtr stream,Ogre::String fileName)
    {
        void *buffer = malloc(stream->size());
        
        stream->read(buffer,stream->size());
		bool res = addFile(buffer,stream->size(),fileName);
		delete buffer;
		return res;
        
    }
    bool getFileAsDataStreamPtr(Ogre::DataStreamPtr &stream,Ogre::String fileName,bool freeOnClose = true,bool allocateWithOAT = true)
    {
        SaveGameFile res;
        if(!getFile(fileName,res,false,allocateWithOAT))
            return false;
        
		//hm. das mach ich mal anders. ich lese es in den datastream ein, anstatt ihn damit zu constructen
		//und den buffer lösch ich sofort

		

		//Ogre::MemoryDataStream *memStream = new Ogre::MemoryDataStream(res.size);//res.buffer,res.size,freeOnClose);
		Ogre::MemoryDataStream *memStream = new Ogre::MemoryDataStream(res.buffer,res.size,freeOnClose);
		//memStream->
		//memStream-> (res.buffer,res.size);
        Ogre::DataStreamPtr str(memStream);
        stream = str;
	/*	stream->close();
		free(res.buffer);*/
        return true;
    }
    
	bool hasFile(Ogre::String fileName);
	//inline bool addFileFromDisk(Ogre::String fileNameSrc,Ogre::String fileNameDest)
	//{
	//	if(hasFile(fileNameDest))
	//		return false;
	//	if(!openZipFile(true))
	//		return false;
	//	bool ok = ZipAdd(mZip,fileNameDest.c_str(),fileNameSrc.c_str()) == ZR_OK;
	//	if(ok)
	//	{
	//		filesAdded.push_back(fileNameDest);
	//	}
	//	return ok;
	//	
	//	//return true;
	//}
	//aufruf zB
	/*
	SaveGameFile file;
	getFile("asdf.muh",file);
	...
	delete file.buffer;
	*/
	bool getFile(Ogre::String fileName,SaveGameFile &resFile,bool nullTerminated = false,bool allocateWithOAT = false);

	Ogre::String getFileAsString(Ogre::String fileName)
	{
		SaveGameFile file;
		if(!getFile(fileName,file,true))
			return "";
		
		Ogre::String res = (reinterpret_cast<char*>(file.buffer));
		res += "\0";
		delete file.buffer;
		return res;
	}
    Ogre::Image getFileAsImage(Ogre::String filename);
	//löscht die Datei
	inline void removeFile()
	{
		closeZipFile();
		int err = remove(mFileName.c_str());
	}
	inline void renameFile(Ogre::String newFileName)
	{
		closeZipFile();
		if(MoveFileA(mFileName.c_str(),newFileName.c_str()))
		{
			mFileName = newFileName;
		}
	}
	inline void renameFile(Ogre::String newName,Ogre::String newPath,Ogre::String newExt)
	{
		renameFile(newPath+"/"+newName+"."+newExt);
	}
	//kopiert die datei nach newFilename, und wechselt auf die neue datei
	//also gemeint ist das Kopieren auf dem Datenträger.
	//hier brauchen wir ne OS-Abfrage...
	inline void copyFile(Ogre::String newFileName,bool failIfExists)
	{
		closeZipFile();
		
		if(CopyFileA(mFileName.c_str(),newFileName.c_str(),failIfExists))
		{
			mFileName = newFileName;
		}
	}
	inline void copyFile(Ogre::String newName,Ogre::String newPath,Ogre::String newExt,bool failIfExists)
	{
		copyFile(newPath+"/"+newName+"."+newExt,failIfExists);
	}
	//wenn write = true, wird die datei zum
	//lesen geöffnet, sonst zum schreiben
	bool openZipFile(bool write);
	
	inline void closeZipFile()
	{
		filesAdded.clear();
		if(mZip)// && openForWrite)
		{
			if(openForWrite)
				zipClose(mZip,NULL);
			else
				unzClose(mZip);
			//CloseZip(mZip);

			mZip = NULL;
			
		}
	}

	void copyFilesFrom(ZipSaveFile *other);
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
	bool openForWrite;
	//HZIP mZip;
	voidp mZip;
	//zipFile writeFile;
	//unzFile readFile;//hm wat. die sind doch identisch...
	Ogre::String mFileName;
	Ogre::String mName;
	//dateien, die per addfile geadded wurden
	//damit man auch im schreibmodus hasFile machen kann
	Ogre::StringVector filesAdded;
	
protected:
	bool addFileNoCheck(void *buf,size_t size,Ogre::String fileName)
	{
		int err = zipOpenNewFileInZip(mZip,fileName.c_str(),0,0,0,0,0,0,0,0);
		if(!err)
		{
			err = zipWriteInFileInZip(mZip,buf,size);
		}		
		return (err == 0);
	}




};

#endif