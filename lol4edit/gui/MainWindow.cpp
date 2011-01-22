#include "MainWindow.h"
#include "QtEditorApp.h"
#include <QCloseEvent>
#include <QtGui/QMdiSubWindow>
#include "ObjectSelectDialog.h"
#include "LevelSelectDialog.h"
#include "ObjectDetailsDialog.h"
#include "TerrainEditDialog.h"
#include "InputDialog.h"
#include "ConfirmDialog.h"
#include "LevelObjectList.h"
#include "PageStatusWindow.h"
#include <EditorApp.h>
#include <Level.h>
#include <functions.h>
#include "h/MainWindow.h"
#include "EventHandlingOgreWidget.h"
#include "LogManager.h"
#include <TypeConverter.h>


	EditorMainWindow::EditorMainWindow(QtOgre::QtEditorApp *_app, QWidget *parent, Qt::WindowFlags flags):
            QMainWindow(parent,flags)
	{
		
        this->qtapp = _app;
		app = EditorApp::getSingletonPtr();
		ui = new Ui::MainWindow();
		ui->setupUi(this);
		setupUi();
	}

	void EditorMainWindow::setStatusText(Ogre::String text)
	{
		ui->statusbar->showMessage(TypeConverter::qt_str(text));
	}

   void EditorMainWindow::setupActions()
   {
		//NEW LEVEL ACTION		
		connect(ui->actionNew_Level, SIGNAL(triggered()), this, SLOT(newLevel()));

		//OPEN ACTION
		connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(showOpenLevel()));

		//SAVE ACTION
		connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveLevel()));


		//SAVE AS ACTION
		connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(showSaveAs()));

		//EXIT ACTION
		connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

		//TERRAIN EDIT MODE ACTION
		//ui->actionEdit_Terrain 

		//OBJECT EDIT MODE ACTION <-- prüfen ob nötig
		//ui->actionEdit_Objects

		////SAVE GROUPED MESHES
		//ui->actionSave_grouped_Meshes

		////SAVE FROUPED OBJECTS
		//ui->actionSave_grouped_Objects

		////SAVE COLLISION <-- wozu war das nochmal?
		//ui->actionSave_Collision

		////EDITOR SETTINGS
		//ui->actionEditor_Settings

		////LEVEL SETTINGS
		//ui->actionLevel_Settings

		//SNAP ON GRID
		connect(ui->actionSnap_on_Grid, SIGNAL(triggered(bool)), this, SLOT(setGridSnap(bool)));

		//SNAP ON ANGLE		
		connect(ui->actionSnap_on_Angle, SIGNAL(triggered(bool)), this, SLOT(setAngleSnap(bool)));

		//SNAP ON SCALE		
		connect(ui->actionSnap_on_Scale, SIGNAL(triggered(bool)), this, SLOT(setScaleSnap(bool)));

		//ENABLE PHYSICS
		connect(ui->actionPhysics, SIGNAL(triggered(bool)), this, SLOT(setPhysicsEnabled(bool)));
		//ui->actionPhysics
		
		//ENABLE FOG
		//ui->actionFog

		//

		//MOVE MODE		
		connect(ui->actionMove_Mode, SIGNAL(toggled(bool)), this, SLOT(setMoveMode(bool)));

		//ROTATE MODE  
		connect(ui->actionRotate_Mode, SIGNAL(toggled(bool)), this, SLOT(setRotateMode(bool)));

		//SCALE MODE
		connect(ui->actionScale_Mode, SIGNAL(toggled(bool)), this, SLOT(setScaleMode(bool)));

		/*
		//terrain editing
		void setTerrainEditing(bool checked = false);
		//objects editing
		void setObjectsEditing(bool checked = false);
		
		*/
		connect(ui->actionEdit_Terrain, SIGNAL(toggled(bool)), this, SLOT(setTerrainEditing(bool)));
		connect(ui->actionEdit_Objects, SIGNAL(toggled(bool)), this, SLOT(setObjectsEditing(bool)));

		
	
		bool test = connect(ui->actionDebugTest, SIGNAL(triggered(bool)), this, SLOT(performDebugTest(bool)));

		
		connect(ui->actionShow_Log_Window,SIGNAL(toggled(bool)), qtapp->getLogManager(), SLOT(setVisible(bool)));


		//CREATING ACTION GROUPS
		//TEST
		manipulationMode = new QActionGroup(this);
		
		manipulationMode->addAction(ui->actionMove_Mode);
		manipulationMode->addAction(ui->actionRotate_Mode);
		manipulationMode->addAction(ui->actionScale_Mode);
		manipulationMode->setExclusive(true);


		editingMode = new QActionGroup(this);
		
		editingMode->addAction(ui->actionEdit_Terrain);
		editingMode->addAction(ui->actionEdit_Objects);
		editingMode->setExclusive(true);
		//ui->actionEdit_Objects->setChecked(true);
   }

	void EditorMainWindow::performDebugTest(bool dummy)
	{
		/*ogreSubWindow->repaint();
		ogreSubWindow->update();*/
		//ogreSubWindow->
		//qtapp->mOgreWidget->update();
		qtapp->mOgreWidget->m_pOgreRenderWindow->update();
		qtapp->mOgreWidget->m_pOgreRenderWindow->windowMovedOrResized();

	}

    void EditorMainWindow::setupUi()
    {      
 
		setupActions();


        mdiArea = new QMdiArea();
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
		
		

        this->setCentralWidget(mdiArea);
		this->ui->mdiArea = mdiArea;
 
    } // setupUi


    void EditorMainWindow::setOgreWidget(QWidget* widget)
    {
        ogreSubWindow = mdiArea->addSubWindow(widget,Qt::SubWindow);
		//widget->setAttribute(Qt::WA_PaintOnScreen);


		//ogreSubWindow->setAttribute(Qt::WA_NoSystemBackground);
		//ogreSubWindow->setAttribute(Qt::WA_PaintOnScreen);//Qt::WA_OpaquePaintEvent);

		//widget->setAttribute(Qt::WA_NoSystemBackground);
		//widget->setAttribute(Qt::WA_OpaquePaintEvent);

		//mdiArea->setAttribute(Qt::WA_NoSystemBackground);
		//mdiArea->setAttribute(Qt::WA_OpaquePaintEvent);


		//widget->setAttribute(Qt::WA_OpaquePaintEvent);
		//Qt::WA_OpaquePaintEvent 
		//bool test = QObject::connect(ogreSubWindow,SIGNAL(aboutToActivate()),widget,SLOT(update()));
    }
    void EditorMainWindow::setLogWidget(QWidget* widget)
    {
         logSubWindow = mdiArea->addSubWindow(widget);
    }
    void EditorMainWindow::resyncSpecialWidgets()
    {
        //ogreSubWindow->
    }

	void EditorMainWindow::alignMdiWindows(QMdiSubWindow *leftTop,QMdiSubWindow *leftBottom,QMdiSubWindow *rightBig)
	{
		QSize mdiSize = mdiArea->size();
		int halfHeight = mdiSize.rheight()/2;
		int thirdWidth = mdiSize.rwidth()/3;
		int twoThirdWidth = 2*mdiSize.rwidth()/3;
		//first window. position: 0,0. size: width/3,height/2
		leftTop->move(0,0);
		leftTop->resize(thirdWidth,halfHeight);

		//second window. position: 0,height/2. size: width/3,height/2
		leftBottom->move(0,halfHeight);
		leftBottom->resize(thirdWidth,halfHeight);

		//third window. position: width/3, 0. size: width*2/3, height
		rightBig->move(thirdWidth,0);
		rightBig->resize(twoThirdWidth,mdiSize.rheight());
	}

	void EditorMainWindow::initGuiElements()
	{
		objectSelectDialog = new ObjectSelectDialog();		
		objectSelectSubWindow = mdiArea->addSubWindow(objectSelectDialog);
		objectSelectSubWindow->setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint));		
		
		objectSelectDialog->show();
		

		terrainEditDialog = new TerrainEditDialog();		
		terrainEditSubWindow = mdiArea->addSubWindow(terrainEditDialog);
		terrainEditSubWindow->setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint));		
		

		objectListDialog = new LevelObjectList();
		pageStatusSubWindow = mdiArea->addSubWindow(objectListDialog);
		pageStatusSubWindow->setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint));
		pageStatusSubWindow->show();
		


		pageStatusWindow = new PageStatusWindow();
		objectListSubWindow = mdiArea->addSubWindow(pageStatusWindow);
		objectListSubWindow->setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint));
		objectListSubWindow->show();
		
		
		//terrainEditDialog->show();

		

		openLevelDialog  = new LevelSelectDialog(this);
		
		//mdiArea->addSubWindow(openLevelDialog);
		//openLevelSubWindow->show();

//		createDialogs();//die anderen da rein verschieben!
	}

	void EditorMainWindow::showOpenLevel(bool checked)
	{
		openLevelDialog->fillList();
		//if(openLevelDialog->exec()
		openLevelDialog->show();
	}
	void EditorMainWindow::updateAfterLoading()
	{
		//die listen von den ganzen teilen mit gamedata füllen
		objectSelectDialog->fillLists();

		//nun alles tilen
		//mdiArea->tileSubWindows();
		alignMdiWindows(objectSelectSubWindow,objectListSubWindow,ogreSubWindow);
		//ogreSubWindow->activateWindow();
		ogreSubWindow->setFocus();
	}

	void EditorMainWindow::showObjectDetails(GameObject *obj)
	{
		ObjectDetailsDialog *details = new ObjectDetailsDialog(obj);
		//details->setupUi();

		QMdiSubWindow *detailsSub = mdiArea->addSubWindow(details);
		detailsSub->setAttribute(Qt::WA_DeleteOnClose);
		
		details->show();
	}

	void EditorMainWindow::newLevel(bool checked)
	{
		app->loadLevel("");
	}

	void EditorMainWindow::saveLevel(bool checked)
	{
		Ogre::String lvlname = app->getCurrentLevel()->getFileName();
		
		//Ogre::String saveTo = app->levelPath+"/"+base+ZLEVELEXT;

		if(lvlname != "")
		{
			saveAs(lvlname);
			//app->getCurrentLevel()->saveToFile(lvlname);
		}
		else
		{
			showSaveAs();
		}

	}

	void EditorMainWindow::showSaveAs(bool checked)
	{
		//InputDialog *input = new InputDialog(this);
		Ogre::String filename;

		if(prompt(filename,"Save Level as","Input Level Name"))
		{
			
			saveAs(filename,true);
		}		
		
	/*	else
		{
			dummy = 0;
		}*/
		//input->setAttribute(Qt::WA_DeleteOnClose);
		//bool test = connect(input, SIGNAL(returnValue(Ogre::String)), this, SLOT(saveAs(Ogre::String)));	
		//input->show(QString::fromUtf8("Save Level As..."),QString::fromUtf8("Input the new filename"),this,SLOT(saveAs(Ogre::String)));

	}
	void EditorMainWindow::saveAs(Ogre::String name, bool confirmOverwrite)
	{
		Level *currentLevel = app->getCurrentLevel();
		if(currentLevel)
		{
			Ogre::String filename;
			if(name == "")
				filename = currentLevel->getFileName();
			else
				filename = name;
			Ogre::String base,ext;
			Ogre::StringUtil::splitBaseFilename(filename,base,ext);
			if(base != "")
			{
				Ogre::String saveTo = app->levelPath+"/"+base+ZLEVELEXT;
				/*if(!Ogre::StringUtil::endsWith(saveTo,LEVELEXT))
				{
					saveTo += LEVELEXT;
				}*/
				if(confirmOverwrite && FileExists(saveTo))
				{
					if(!confirm("Confirm Overwriting","The file \""+saveTo+"\" alerady exists. Overwrite?"))
						return;
				}
				currentLevel->saveToFile(saveTo);
				//updateLevelList();
			}
			else
			{
				//mLog("saveCurrentLevel(\""+name+"\") failed: basename \""+base+"\" is empty");
			}
		}	
    }

	void EditorMainWindow::replaceSubWindow(QMdiSubWindow *oldWnd, QMdiSubWindow* newWnd)
	{
		QSize oldSize = oldWnd->size();
		QPoint oldPos = oldWnd->pos();
		oldWnd->hide();
		newWnd->show();
		newWnd->resize(oldSize);
		newWnd->move(oldPos);
	}

	//terrain editing
	void EditorMainWindow::setTerrainEditing(bool checked)
	{
		/*showTerrainEditingWindows(checked);
		showObjectEditingWindows(!checked);*/
		if(checked)
		{
			replaceSubWindow(objectSelectSubWindow,terrainEditSubWindow);
			EditorApp::getSingletonPtr()->setEditorMode(EditorApp::emTerrain);
		}
	}
	//objects editing
	void EditorMainWindow::setObjectsEditing(bool checked)
	{
		/*showTerrainEditingWindows(!checked);
		showObjectEditingWindows(checked);*/
		if(checked)
		{
			replaceSubWindow(terrainEditSubWindow,objectSelectSubWindow);
			EditorApp::getSingletonPtr()->setEditorMode(EditorApp::emLevel);
		}
	}

	void EditorMainWindow::showTerrainEditingWindows(bool show)
	{
		terrainEditSubWindow->setVisible(show);
	}
	void EditorMainWindow::showObjectEditingWindows(bool show)
	{
		objectSelectSubWindow->setVisible(show);
	}

	//void EditorMainWindow::createDialogs()
	//{
	//	inputDialog = new InputDialog(this);
	//	confirmDialog = new ConfirmDialog(this);
	//}

	//shortcut zum inputdialog. fordert den User zur Texteingabe auf.
	//gibt bei accept true zurück und schreibt den String ins result
	//gibt bei reject false zurück und schreibt einen Leerstring ins result
	bool EditorMainWindow::prompt(Ogre::String &result,Ogre::String title,Ogre::String text)
	{
		InputDialog *inputDialog = new InputDialog(this);		
	
		if(inputDialog->exec(title.c_str(),text.c_str()) == QDialog::Accepted)
		{
			result = inputDialog->getInputValue();
			return true;
		}
		else
		{
			result = "";
			return false;
		}
	}

	//shortcut zum confirmdialog. Fordert den User auf, etwas zu bestätigen
	//gibt bei accept true zurück, bei reject false
	bool EditorMainWindow::confirm(Ogre::String title,Ogre::String text)
	{
		ConfirmDialog *confirmDialog = new ConfirmDialog(this);
		return confirmDialog->exec(title.c_str(),text.c_str()) == QDialog::Accepted;		
	}

	//gridsnap
	void EditorMainWindow::setGridSnap(bool checked)
	{
		app->snapOnGrid = checked;
	}
	//angle snap
	void EditorMainWindow::setAngleSnap(bool checked)
	{
		app->snapOnAngle = checked;
	}
	//scale snap
	void EditorMainWindow::setScaleSnap(bool checked)
	{
		app->snapOnScale = checked;
	}

	//move mode
	void EditorMainWindow::setMoveMode(bool checked)
	{
		if(checked)
		{
			app->getQtOgreGameLogic()->setTransformModeDirect(QtOgre::EditGameLogic::tmMove);
		}
		//actionMove_Mode->setChecked(false);
        //actionRotate_Mode->setChecked(false);
        //actionScale_Mode->setChecked(false);
	}
	//rotate mode
	void EditorMainWindow::setRotateMode(bool checked)
	{
		if(checked)
		{
			app->getQtOgreGameLogic()->setTransformModeDirect(QtOgre::EditGameLogic::tmRotate);
		}
		//actionMove_Mode->setChecked(false);
        //actionRotate_Mode->setChecked(false);
        //actionScale_Mode->setChecked(false);
	}
	//scale mode
	void EditorMainWindow::setScaleMode(bool checked)
	{
		if(checked)
		{
			app->getQtOgreGameLogic()->setTransformModeDirect(QtOgre::EditGameLogic::tmScale);
		}
		//actionMove_Mode->setChecked(false);
        //actionRotate_Mode->setChecked(false);
        //actionScale_Mode->setChecked(false);
	}

	void EditorMainWindow::setPhysicsEnabled(bool checked)
	{
		((EditorApp*)StandardApplication::getSingletonPtr())->paused = !checked;
		/*if(checked)
		{
		}
		else
		{
		}*/
	}

	
	//QString EditorMainWindow::getObjListName(GameObject *cur)
	//{
	//	
	//	Ogre::String name = cur->getID().c_str();
	//	switch(cur->getType())
	//	{
	//	case otDynamic:
	//		name = "(sp)"+name;
	//		break;
	//	case otItem:
	//		name = "(it)"+name;
	//		break;
	//	case otChar:
	//		name = "(ch)"+name;
	//		break;
	//	case otDoor:
	//		name = "(do)"+name;
	//		break;
	//	case otLight:
	//		{
	//			LightObject *li = static_cast<LightObject*>(cur);
	//			name = "(li)";
	//			switch(li->getLightType())
	//			{
	//			case Ogre::Light::LT_DIRECTIONAL:
	//				name += "directional";
	//				break;
	//			case Ogre::Light::LT_POINT:
	//				name += "point";
	//				break;
	//			case Ogre::Light::LT_SPOTLIGHT:
	//				name += "spotlight";
	//				break;
	//			}
	//			
	//			name += ("("+str(li->getLight()->getDiffuseColour())+")").c_str(); //+name;
	//		}
	//		break;
	//	case otEntrance:
	//		name = "(en)"+name;
	//		break;
	//	case otStatic:
	//		{
	//			StaticObject *st = static_cast<StaticObject*>(cur);
	//			name = "(st)";//+st->getWorldArt()->getData().filename;
	//			WorldArtData wd = st->getWorldArt()->getData();
	//			switch(wd.type)
	//			{
	//			case WT_BILLBOARD:
	//				name += ("billboard:"+wd.material).c_str();
	//				break;
	//			case WT_DOTSCENE:
	//				name += ("dotscene:"+wd.filename).c_str();
	//				break;
	//			case WT_MESH:
	//				name += ("mesh:"+wd.filename).c_str();
	//				break;
	//			case WT_PLANE:
	//				name += ("plane:"+wd.material).c_str();
	//				break;
	//			case WT_PARTICLESYSTEM:
	//				name += ("particlesys:"+wd.material).c_str();
	//				break;
	//			}
	//		}
	//		break;
	//	case otMissile:
	//		name = "(mi)"+name;
	//		break;
	//	case otContainer:
	//		name = "(co)"+name;
	//		break;
	//	case otExplosion:
	//		name = "(ex)"+name;
	//	case otDeleted:
	//		name = "DELETED";
	//	case otWater:
	//		{
	//			WaterPlane *w = static_cast<WaterPlane*>(cur);

	//			name = "(wa)"+w->getMaterial();
	//		}
	//	}
	//	
	//	
	//	return QString(name.c_str());
	//}
