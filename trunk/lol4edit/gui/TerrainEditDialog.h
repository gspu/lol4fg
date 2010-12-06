#ifndef __terrainEdit
#define __terrainEdit

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
//#include <QtGui/QListViewItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QMdiSubWindow>

namespace Ui {
    class TerrainEditDialog;//fwdec
} 

class TerrainEditDialog: public QDialog
{
	Q_OBJECT
public:
	//default constructor
	TerrainEditDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);



	Ui::TerrainEditDialog *ui;



	
	//QMdiArea *parent;//!!
	//QMdiSubWindow *asSubWindow;//
public slots:

	//void selectEntry(QTreeWidgetItem *item, int column);
	//void addButtonClicked();
	
protected:
	void fillTextureList();

	void fillBrushList();
	

	

};


#endif