#ifndef __pageStatusWnd
#define __pageStatusWnd

#include <Ogre.h>
#include <FwDec.h>
#include <QtGui/QTreeWidget>
#include <QtGui/QDialog>

namespace Ui {
    class PageStatusDialog;//fwdec
} 




class PageStatusWindow: public QDialog
{
	Q_OBJECT
public:
	enum PageStatus
	{
		//the page is not loaded
		psNotLoaded	= 0,
		//the page is being loaded
		psLoading	= 1,
		//page is loaded
		psLoaded	= 2,
		//page is being unloaded
		psUnloading	= 3
	};


	class PageItem: public QTreeWidgetItem
	{
	public:
		/*
		x, y: coordinates of the page
		status: the status the entry should display
		isSaved: whenever the page is alerady in the level file or not
				 pages which are neiter loaded nor in level file should
				 not be present in the list
		view: the parent QTreeWidget
		*/
		PageItem(long x, long y, PageStatus status, bool isSaved, QTreeWidget *view, int type = Type);
			

		inline void getCoordinates(long &x, long &y)
		{
			x = mXcoord;
			y = mYcoord;
		}

		void setCoordinates(long x, long y);

		void setStatus(PageStatus status);
		inline PageStatus getStatus()
		{
			return mStatus;
		}

		void setSaved(bool set);

		inline bool getSaved()
		{
			return isSaved;
		}
	protected:
		long mXcoord;
		long mYcoord;
		PageStatus mStatus;
		bool isSaved;
	    
	};

	
	//default constructor
	PageStatusWindow(QWidget *parent = 0, Qt::WindowFlags f = 0):
		QDialog(parent,f)
	{
	}



	Ui::PageStatusDialog *ui;


	void notifyPageStatus(long x, long y, PageStatus status);


	
public slots:

	void setAutoLoad(bool set);
	
	void loadPageClick();

	void unloadPageClick();


	
protected:
	//shortcut to some stuff
	//QListWidgetItem *getSelectedItem(QListWidget *list);


	

	

};


#endif