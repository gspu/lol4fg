#ifndef __levelObjectList
#define __levelObjectList

#include <QDialog>
#include <QListWidget>
#include <FwDec.h>

namespace Ui
{
	class ObjectListDialog;
}

//helper class
class ObjectListItem: public QListWidgetItem
{
public:
	ObjectListItem ( QListWidget * parent = 0, int type = Type ):
		QListWidgetItem(parent,type){}

	ObjectListItem ( const QString & text, QListWidget * parent = 0, int type = Type ):
		QListWidgetItem(text,parent,type){}

	ObjectListItem ( GameObject *obj, const QString & text, QListWidget * parent = 0, int type = Type ):
		QListWidgetItem(text,parent,type)
		{
			setGameObject(obj);
		}
	/*ObjectListItem ( const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = Type ):
		QListWidgetItem(icon,text,parent,type);*/

	void setGameObject(GameObject *obj);
	inline GameObject *getGameObject()
	{
		return mObject;
	}
private:
	

	GameObject *mObject;
};

class LevelObjectList: public QDialog
{
	Q_OBJECT
public:
	Ui::ObjectListDialog *ui;

	LevelObjectList(QWidget *parent = 0, Qt::WindowFlags f = 0);

	void setLevel(Level *lvl);

	void updateList(GameObject *changed = NULL,bool added = true);
public slots:
	void itemDoubleClicked ( QListWidgetItem * item );
private:
	Level *mLevel;
	//QMenu *mMenu;

	void initPopupMenu();
	//void fillList();
	
};


#endif