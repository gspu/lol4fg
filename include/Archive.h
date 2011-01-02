#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <Ogre.h>
class Archive
{
public:	
	class Buffer
	{
	public: 
		enum AllocType
		{
			atDoNotTouch = 0,	//will refuse to deallocate
			atMalloc = 1,		//using malloc()
			atNew = 2,			//using new char[x]
			atOgreMalloc = 3//,	//using OGRE_MALLOC (category MEMCATEGORY_RESOURCE)
			//atOgreNew = 4,		//using OGRE_NEW
		};
		//default constructor with everything empty
		Buffer()
			{
				data = NULL;
				length = 0;
				aType = atDoNotTouch;
			}
		/*Buffer(size_t length, AllocType aType)
			{
				allocate(length,aType);
			}*/

		//returns length
		inline size_t getLength()
		{
			return length;
		}

		//returns the data as a char*
		char *asCharPtr()
		{
			return static_cast<char*>(data);
		}

		//returns the data as a unsigned char*
		unsigned char *asUCharPtr()
		{
			return static_cast<unsigned char*>(data);
		}

		//returns data as a void*
		void *asVoidPtr()
		{
			return data;
		}

		//just sets generic data, for when some function wants the data to be wrapped in a Buffer object
		//whereever the data comes from, it has to take care of deallocation itself, Buffer will not be able to do so
		void setData(void *newData, size_t length)
		{
			this->length = length;
			data = newData;
			aType = atDoNotTouch;
		}

		//like the one which takes a void*, but takes a char*
		void setData(char *newData, size_t length)
		{
			this->length = length;
			data = static_cast<void*>(newData);
			aType = atDoNotTouch;
		}

		//like the one which takes a void*, but takes a unsigned char*
		void setData(unsigned char *newData, size_t length)
		{
			this->length = length;
			data = static_cast<void*>(newData);
			aType = atDoNotTouch;
		}

		//allocate length bytes with the specified allocation method
		bool allocate(size_t length, AllocType at = atNew)
		{
			this->length = length;
			aType = at;
			switch(at)
			{
			case atMalloc:
				data = malloc(length);
				if(!data)
					return false;
				
			
				return true;
			case atNew:
				{
					char *tempData = new char[length];
					data = static_cast<void*>(tempData);
					
				
					return true;
				}
			case atOgreMalloc://using OGRE_MALLOC (category MEMCATEGORY_RESOURCE)
				{
					data = OGRE_MALLOC(length,Ogre::MEMCATEGORY_RESOURCE);
					return true;
				}
			//case atOgreNew:		//using OGRE_NEW
			//	{
			//		char *tempData = OGRE_NEW (char[length]);
			//		data = static_cast<void*>(tempData);				
			//		return true;
			//	}
			}
			this->length = 0;
			aType = atDoNotTouch;
			return false;
		}

		//deallocates the buffer
		bool deallocate()
		{
			switch(aType)
			{
			case atDoNotTouch:
				return false;
			case atMalloc:
				free(data);
				break;	
			case atNew:
				{
					char *tempData = asCharPtr();
					delete tempData;
				}
				break;
			case atOgreMalloc://using OGRE_MALLOC (category MEMCATEGORY_RESOURCE)
				{
					OGRE_FREE(data,Ogre::MEMCATEGORY_RESOURCE);
					//data = OGRE_MALLOC(length,MEMCATEGORY_RESOURCE);					
				}
			//case atOgreNew:		//using OGRE_NEW
			//	{
			//		char *tempData = asCharPtr();
			//		OGRE_DELETE tempData;
			//	}
			default:
				return false;//and no reset
			}
			data = NULL;
			length = 0;
			aType = atDoNotTouch;
			return true;
		}

		inline unsigned char operator [] (const size_t index) const
		{
			assert(index < length);
			return (static_cast<unsigned char*>(data)[index]);
		}

		inline unsigned char & operator [] (const size_t index) 
		{
			assert(index < length);
			return (asUCharPtr()[index]);
		}
	
		inline bool isEmpty()
		{
			return data == NULL;
		}

		
	private:
		AllocType aType;
		void *data;
		size_t length;

	};
public:
	Archive()
	{
		defaultAllocType = Buffer::atNew;
	}

	/*
	virtual bool openRead();
	virtual bool openWrite();
	*/
	/*virtual void addFile();
	virtual void removeFile();*/

	virtual bool hasFile(std::string filename) = 0;

	virtual Buffer getFile(std::string filename,Buffer::AllocType allocWith = Buffer::atNew, bool nullTerminated = false) = 0;

	virtual bool addFile(std::string filename, Buffer data, bool overwrite = true) = 0;

	virtual bool removeFile(std::string filename) = 0;

	inline void setDefaultAllocType(Buffer::AllocType type)
	{
		defaultAllocType = type;
	}

	inline Buffer::AllocType getDefaultAllocType()
	{
		return defaultAllocType;
	}

protected:
	Buffer::AllocType defaultAllocType;

	
};