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
#include <Ogre.h>
#include <FwDec.h>


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



	//returns selected brush size in world size stuff
	int getBrushSize();

	//returns selected intensity
	int getIntensity();

	float getIntensityFactor();

	Ogre::Image getSelectedBrush();

	float getScaleFactor();

	void updateTextureList(Level *lvl);
	int getCurrentTexture();

	QListWidgetItem *selectedItem;
	int selectedScale;
	Ogre::Image selectedBrush;
public slots:

	void brushSelectionChanged();

	void editModeChanged(int index);
	//void selectEntry(QTreeWidgetItem *item, int column);
	//void addButtonClicked();
	
protected:
	//shortcut to some stuff
	QListWidgetItem *getSelectedItem(QListWidget *list);

	void fillTextureList();

	void fillBrushList();
	

	

};


#endif