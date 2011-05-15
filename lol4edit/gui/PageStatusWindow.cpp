#include "PageStatusWindow.h"
#include "h/PageStatusDialog.h"
#include <TypeConverter.h>
#include <EditorApp.h>
#include "Level.h"

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
	//EditorApp::getSingletonPtr()->getCurrentLevel()->setAutoPaging(set);
}
	
void PageStatusWindow::loadPageClick()
{
}

void PageStatusWindow::unloadPageClick()
{
}

void PageStatusWindow::notifyPageStatus(long x, long y, PageStatus status)
{
	PageItem *item = getItemByCoords(x,y);
	if(!item)
		item = new PageItem(x,y,status,false,ui->pageStatusList);
	else
		item->setStatus(status);
	/*switch(status)
	{
	case psNotLoaded:
		break;
	case psLoading:
		break;
	case psLoaded:
		break;
	case psUnloading:
		break;
	};*/
}

PageStatusWindow::PageItem *PageStatusWindow::getItemByCoords(long x, long y)
{
	int cnt = ui->pageStatusList->topLevelItemCount();
	
	for(int i=0;i<cnt;i++)
	{
		long cur_x, cur_y;
		PageItem *cur = static_cast<PageItem *>(ui->pageStatusList->topLevelItem(i));
		cur->getCoordinates(cur_x,cur_y);
		if(cur_x == x && cur_y == y)
		{
			return cur;
		}
	}
	return NULL;
}

PageStatusWindow::PageStatusWindow(QWidget *parent, Qt::WindowFlags f):
	QDialog(parent,f)
{
	ui = new Ui::PageStatusDialog();
	ui->setupUi(this);
	
	
	//connecting
	QObject::connect(ui->loadPageButton, SIGNAL(clicked()), this, SLOT(loadPageClick()));
	QObject::connect(ui->unloadPageButton, SIGNAL(clicked()), this, SLOT(unloadPageClick()));
	QObject::connect(ui->autoLoadPages, SIGNAL(toggled(bool)), this, SLOT(setAutoLoad(bool)));

	ui->pageStatusList->clear();

}