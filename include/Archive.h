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
		Buffer()
			{
				data = NULL;
				length = 0;
				aType = atDoNotTouch;
			}

		size_t getLength()
		{
			return length;
		}
		char *asCharPtr()
		{
			return static_cast<char*>(data);
		}

		unsigned char *asUCharPtr()
		{
			return static_cast<unsigned char*>(data);
		}

		void *asVoidPtr()
		{
			return data;
		}

		void setData(void *newData, size_t length)
		{
			this->length = length;
			data = newData;
			aType = atDoNotTouch;
		}

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


		/*void setByteAt(size_t index, unsigned char byte)
		{

		}*/

		/*
		unsigned char operator [] (size_t index, unsigned char byte)
		{
			assert(index < length);
			return asUCharPtr()[index];
		}
		*/

		
	private:
		AllocType aType;
		void *data;
		size_t length;

	};
public:
	Archive(){};

	/*
	virtual bool openRead();
	virtual bool openWrite();
	*/
	/*virtual void addFile();
	virtual void removeFile();*/

	virtual bool hasFile(std::string filename) = 0;

	virtual Buffer getFile(std::string filename) = 0;

	virtual bool addFile(std::string filename, Buffer data, bool overwrite = true) = 0;

	virtual bool removeFile(std::string filename) = 0;
	
};