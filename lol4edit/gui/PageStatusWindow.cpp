#include "PageStatusWindow.h"
#include "h/PageStatusDialog.h"
#include <TypeConverter.h>


PageStatusWindow::PageItem::PageItem(long x, long y, PageStatus status, bool isSaved, QTreeWidget *view, int type):
	QTreeWidgetItem(view,type)
{
	/*
	0 -> x
	1 -> y
	2 -> status
	3 -> saved
	*/
	setCoordinates(x,y);
	setSaved(isSaved);
	setStatus(status);
	/*setText(0, qt_str(x));
	setText(1, qt_str(y));
	setText(2, PageStatusWindow::pageStatusToStr(status));
    setText(3, );*/
    
    

}

void PageStatusWindow::PageItem::setCoordinates(long x, long y)
{	
	using namespace TypeConverter;
		
	setText(0, qt_str(x));
	setText(1, qt_str(y));
	mXcoord = x;
	mYcoord = y;
}

void PageStatusWindow::PageItem::setStatus(PageStatus status)
{
	switch(status)
	{
	case psNotLoaded:
		setText(2, QApplication::translate("PageStatusDialog", "not loaded", 0, QApplication::UnicodeUTF8));
		break;
	case psLoading:
		setText(2, QApplication::translate("PageStatusDialog", "loading", 0, QApplication::UnicodeUTF8));
		break;
	case psLoaded:
		setText(2, QApplication::translate("PageStatusDialog", "loaded", 0, QApplication::UnicodeUTF8));
		break;
	case psUnloading:
		setText(2, QApplication::translate("PageStatusDialog", "unloading", 0, QApplication::UnicodeUTF8));
		break;
	default:
		setText(2, QApplication::translate("PageStatusDialog", "invalid", 0, QApplication::UnicodeUTF8));
	};
	mStatus = status;
}

void PageStatusWindow::PageItem::setSaved(bool set)
{
	if(set)
		setText(3, QApplication::translate("PageStatusDialog", "yes", 0, QApplication::UnicodeUTF8));
	else
		setText(3, QApplication::translate("PageStatusDialog", "no", 0, QApplication::UnicodeUTF8));

	isSaved = set;
}

void PageStatusWindow::setAutoLoad(bool set)
{
}
	
void PageStatusWindow::loadPageClick()
{
}

void PageStatusWindow::unloadPageClick()
{
}

void PageStatusWindow::notifyPageStatus(long x, long y, PageStatus status)
{
}