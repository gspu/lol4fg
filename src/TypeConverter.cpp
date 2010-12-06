#include <TypeConverter.h>
#include <Ogre.h>
#include <QImage>
namespace TypeConverter
{
	/*QImage ogreImageToQt(Ogre::Image *img)
	{
		QImage::Format mFormat = ogreFormatToQt(img->getFormat());
		if(mFormat != QImage::Format_Invalid)
		{
			return QImage(img->getData(),img->getWidth(),img->getHeight(),mFormat);
		}
		else
		{
			return QImage();
		}
	}*/
	
	QImage::Format ogreFormatToQt(Ogre::PixelFormat format)
	{
		switch(format)
		{
		case Ogre::PF_UNKNOWN:
			return QImage::Format_Invalid;
			/*Format_Mono,
			Format_MonoLSB, <- we haven't got these, it seems. */
			//Format_Indexed8, <- haven't found this in ogre, either
		case Ogre::PF_BYTE_RGB:
			return QImage::Format_RGB32;
		case Ogre::PF_A8R8G8B8:
			return QImage::Format_ARGB32;
			//Format_ARGB32_Premultiplied, does not exist in ogre, i guess
		case Ogre::PF_R5G6B5:
			return QImage::Format_RGB16;
			//Format_ARGB8565_Premultiplied,
		/*case Ogre::PF_R8G8B8:
			return QImage::Format_RGB666;*/
			//Format_ARGB6666_Premultiplied,
		/*case Ogre::PF_R5G6B5:
			return QImage::Format_RGB555;*/
			//Format_ARGB8555_Premultiplied,
		case Ogre::PF_R8G8B8:
			return QImage::Format_RGB888;
			//Format_RGB444,
			//Format_ARGB4444_Premultiplied,
		};
		return QImage::Format_Invalid;
	}
}