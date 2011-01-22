#include "QtEditorApp.h"



#include "EventHandlingOgreWidget.h"
#include "FPSDialog.h"
#include "GameLogic.h"
#include "GraphicsSettingsWidget.h"
#include "LogManager.h"
#include "SettingsDialog.h"
#include "EditorApp.h"
#include <OgreRenderSystem.h>
#include <OgreRoot.h>

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QtCore\QTimer>
#include <QtCore\QSettings>
#include "EditGameLogic.h"
#include <QtGui/QMainWindow>
#include <QtGui/QMdiSubWindow>
#include "MainWindow.h"
#include "EditorApp.h"
//#include "FakeLogManager.h"
//Q_INIT_RESOURCE cannot be called from within a namespace, so we provide
//this function. See the Q_INIT_RESOURCE documentation for an explanation.


inline void initQtResources() { Q_INIT_RESOURCE(resources); }


	void qtMessageHandler(QtMsgType type, const char* msg)
     {
         switch (type)
		 {
         case QtDebugMsg:
             static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance())->_systemLog()->logMessage(msg, QtOgre::LL_DEBUG);
             break;
         case QtWarningMsg:
             static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance())->_systemLog()->logMessage(msg, QtOgre::LL_WARNING);
             break;
         case QtCriticalMsg:
             static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance())->_systemLog()->logMessage(msg, QtOgre::LL_ERROR);
             break;
         case QtFatalMsg:
			 //We don't override this one as we are dying and wouldn't see it.
             fprintf(stderr, "Fatal: %s\n", msg);
             abort();
         }
     }
namespace QtOgre
{
	QtEditorApp::QtEditorApp(int& argc, char** argv, QtOgre::EditGameLogic* gameLogic)
	:QApplication(argc, argv)
	,mOpenGLRenderSystem(0)
	,mDirect3D9RenderSystem(0)
	,mFrameCounter(0)
	{
		mGameLogic = gameLogic;
		if(mGameLogic != 0)
		{
            mGameLogic->mRealApp = this;//Application = static_cast<QtOgre::Application*>(const_cast<QtEditorApp*>(this));
		}

		//Initialise the resources.
		initQtResources();
		mUpdateTimer = new QTimer;
		QObject::connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
		//On the test system, a value of one here gives a high frame rate and still allows
		//event prcessing to take place. A value of 0 doubles the frame rate but the mouse
		//becomes jumpy. This property should probably be configurable.
		mUpdateTimer->setInterval(1);

		//Load the settings file. If it doesn't exist it is created.
		
        /*test = new QMainWindow();
        Ui::MainWindow *asd = new Ui::MainWindow();
        asd->setupUi(test);*/
        //asd->subwindow_4->resize(300,600);
        
		mOgreWidget = new QtOgre::EventHandlingOgreWidget(0, 0);
		mOgreWidget->setMouseTracking(true);
       // mOgreWidget->resize(500,500);
        //asd->mdiArea->addSubWindow(mOgreWidget);
		//Logging should be initialised ASAP, and before Ogre::Root is created.
		initialiseLogging();

		//Create the root and load render system plugins. We do that here so that we know
		//what render systems are available by the time we show the settings dialog.
		//Note that the render system is not initialised until the user selects one.
		mRoot = new Ogre::Root();
		//loadRenderSystemsFromPlugins();
		EditorApp::getSingletonPtr()->setupFromQtApp();

        mSettings = new QSettings("Pra","qtogretest"); //("settings.ini", QSettings::IniFormat);
        //erzeuge das mainwnd
        mainWnd = new EditorMainWindow(this);

  
        QStringList windowModeList;
        windowModeList.clear();
        int windowModeIndex = -1;

		Ogre::RenderSystemList testasd = mRoot->getAvailableRenderers();
        Ogre::RenderSystem *rs = *mRoot->getAvailableRenderers().begin();
        //eines suchen
        for (Ogre::RenderSystemList::const_iterator pRend = mRoot->getAvailableRenderers().begin(); pRend != mRoot->getAvailableRenderers().end(); ++pRend)
        {
            Ogre::RenderSystem* cur = *pRend;
            if(Ogre::StringUtil::match(cur->getName(),"*Direct3D*"))
            {
                rs = cur;
               
                break;
            }            
        }
        
      
        mSettings->setValue("RenderSystem",rs->getName().c_str());
        QStringList test;
        test.push_back("800x600");
        test.push_back("1024x768");
        test.push_back("1280x1024");
        test.push_back("FullScreen");
       
        mSettings->setValue("Graphics/WindowModes",test);//"800x600, 1024x768, 1280x1024, FullScreen");
        mSettings->setValue("Graphics/SelectedWindowMode",0);
        //showWarningMessageBox(mSettings->value("Graphics/SelectedWindowMode", "asd").toString());
        mSettings->setValue("AllowPerfHUD","false");
        mSettings->setValue("EnableGammaCorrection","false");
        mSettings->setValue("FSSAFactor","2");
        mSettings->setValue("EnableVerticalSync","false");
        

        //Ogre::ConfigOptionMap::iterator pOpt;
        //for(pOpt = rs->getConfigOptions().begin(); pOpt != rs->getConfigOptions().end;pOpt++)
        //{
        //    Ogre::ConfigOption cur = pOpt->second;
        //    if(cur.name == "Full Screen")
        //    {
        //        windowModeList.append("FullScreen");
        //        if(cur.currentValue == "Yes");
        //        //windowModeIndex++
        //        continue;
        //        //mSettings->setValue("
        //    }
        //}
        //TiXmlElement *option = rsElem->FirstChildElement("option");
        //ConfigOptionMap cfg = rs->getConfigOptions();
        //while(option)
        //{
        //    Ogre::String optName = option->Attribute("name");
        //    Ogre::String optValue= option->Attribute("value");
        //    ConfigOption curOption = cfg[optName];
        //    //schauen, ob es das ausgewählte überhaupt gibt
        //    bool valueFound = false;
        //    for(Ogre::StringVector::iterator itr = curOption.possibleValues.begin();itr!=curOption.possibleValues.end();itr++)
        //    {
        //        if(*itr == optValue)
        //        {
        //            rs->setConfigOption(optName,optValue);
        //            valueFound = true;
        //            break;
        //        }
        //    }           
        //    if(!valueFound)
        //        LogManager::getSingletonPtr()->logMessage("Warning: Invalid value '"+optValue+
        //        "' for property '"+optName+"' of Render System '"+rsName+"'! Default value '"+curOption.currentValue+"' is used instead");
        //    //ansonsten ignorieren
        //    option = option->NextSiblingElement("option");
        //}

		//mSettingsDialog = new QtOgre::SettingsDialog(mSettings, mOgreWidget);
		//mGraphicsSettingsWidget = new QtOgre::GraphicsSettingsWidget;
		//mSettingsDialog->addSettingsWidget("Graphics", mGraphicsSettingsWidget);
	}

	QtEditorApp::~QtEditorApp()
	{
		if(mOgreWidget)
		{
			delete mOgreWidget;
			mOgreWidget = 0;
		}
		if (mRoot)
		{
            delete mRoot;
			mRoot = 0;
		}
	}

	bool QtEditorApp::isOpenGLAvailable(void) const
	{
		return mOpenGLRenderSystem != 0;
	}

	bool QtEditorApp::isDirect3D9Available(void) const
	{
		return mDirect3D9RenderSystem != 0;
	}

	unsigned int QtEditorApp::frameCount(void) const
	{
		return mFrameCounter;
	}

	/**This function is an implementation detail, and should not really be exposed.
	It return the log which the QtOgre framework uses for its messages, whereas
	users are expected to instead create their own log with createLog(). The reason
	it is exposed is that the Qt debugging system (qtMessageHandler()) also redirects
	to this log, and that cannot be made a member function.*/
    QtOgre::Log* QtEditorApp::_systemLog(void) const
	{
		return mSystemLog;
	}

	QWidget* QtEditorApp::mainWidget(void) const
	{
		return mOgreWidget;
	}

	void QtEditorApp::applySettings(void)
	{
		//Eventually QtEditorApp settings might be applied here.
		//Until then, we just pass the settings on the the MainWindow and GameLogic
        
		if(!mOgreWidget->applySettings(mSettings))
		{
			showWarningMessageBox("Unable to apply desired settings to the window.\nPlease consult the system log for details");
		}
        //GNAAAAARGHJ!!!
        
        //mOgreWidget->resize( mainWnd->ogreSubWindow->size()   );

        mOgreWidget->updateGeometry();
        mLogManager->updateGeometry();
        //mainWnd->ogreSubWindow->updateGeometry();
		/*if(mGameLogic != 0)
		{
			mGameLogic->applySettings(mSettings);
		}*/
        //nun das EditorApp benachrichtigen, dass es weitergeht
        static_cast<EditorApp*>(StandardApplication::getSingletonPtr())->setup_qt();
		//nun mainwnd
		
	}


	void QtEditorApp::postInit(void)
	{
		
		mainWnd->initGuiElements();

		mainWnd->updateAfterLoading();
		
	}

	void QtEditorApp::initialise(void)
	{
        //test->show();
      
		
		//centerWidget(mOgreWidget);
        
		mainWnd->showMaximized();
       
		/*QMdiSubWindow *ogreSubWindow = mainWnd->mdiArea->addSubWindow(mOgreWidget);  
        QMdiSubWindow *logSubWindow = mainWnd->mdiArea->addSubWindow(mLogManager);   */
       
        //mainWnd->ogreSubWindow->setMinimumSize(800,600);
		
		mainWnd->setOgreWidget(mOgreWidget);
        mainWnd->setLogWidget(mLogManager);
        
        /*QList<QMdiSubWindow *> list = mainWnd->mdiArea->subWindowList();
        QMdiSubWindow * tes = list[0];
        tes->setMinimumSize(800,600);
        tes->resize(800,600);
        mOgreWidget->resize(800,600);
        mOgreWidget->updateGeometry();*/
        //mainWnd->mdiArea->
		//mLogManagerDockWidget = mMainWindow->addAsDockWidget(mLogManager, "Logs", Qt::AllDockWidgetAreas);
		//mLogManager->setParent(mMainWindow);
		//centerWidget(mLogManager, mOgreWidget);

        mOgreWidget->show();
		//mLogManager->show();
        
        //ogreSubWindow->res


		mLogManager->setForceProcessEvents(true);
		initialiseOgre();
		Ogre::NameValuePairList ogreWindowParams;
		//ogreWindowParams["FSAA"] = "8";
		mOgreWidget->initialise(&ogreWindowParams);
		mGameLogic->initialise();
		mLogManager->setForceProcessEvents(false);

		//mLogManager->hide();




		mOgreWidget->setEventHandler(mGameLogic);

		//This is a bit of a hack, necessary because we want to use the settings dialog in two different
		//ways. The first time it is shown (by QtEditorApp::exec()) no slot is connected - the Accepted
		//event is handled explicitly because the system is not initialised at that point. But now (and
		//when the dialog is shown in future) we are ready for it, so we connect the accepted() signal.
		//We also call accept, to do the initial setup. See also QtEditorApp::exec().
		//connect(mSettingsDialog, SIGNAL(accepted()), this, SLOT(applySettings()));
		//mSettingsDialog->accept();
		applySettings();

       
        //asd->setupUi(asd
        /*QMainWindow *asd = new QMainWindow();
        test = new Ui_MainWindow();
        asd->show();*/

        mFPSDialog = new QtOgre::FPSDialog(mOgreWidget, Qt::FramelessWindowHint);
		mFPSDialog->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());
		mFPSDialog->move(mainWidget()->geometry().topLeft() + QPoint(10,10));
		//mFPSDialog->show();//den sollt ich wirklich ausbauen

		mLogManager->move(mainWidget()->geometry().left() + 10, mainWidget()->geometry().top() + mainWidget()->geometry().height() - mLogManager->frameGeometry().height() - 10);

		mUpdateTimer->start();
        //test
        //mainWnd->ogreSubWindow->resize(800,600);//ach da
	}

	void QtEditorApp::update(void)
	{
		mGameLogic->update();
		mOgreWidget->update();
		//mainWnd->ogreSubWindow->
		/*mainWnd->ogreSubWindow->update();
		mainWnd->ogreSubWindow->updateGeometry();*/
		++mFrameCounter;
	}

	void QtEditorApp::shutdown(void)
	{
		mUpdateTimer->stop();
		mGameLogic->shutdown();
		mInternalOgreLog->removeListener(this);
		this->exit(0);
	}

	void QtEditorApp::initialiseLogging(void)
	{
		//Initialise our logging system
		mLogManager = new QtOgre::LogManager(mOgreWidget);
		mLogManager->resize(550, 400);
//		mLogManager->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());

		//Redirect Qt's logging system to our own
		mSystemLog = mLogManager->createLog("System");
		qInstallMsgHandler(&qtMessageHandler);
		qDebug("Debug test");
		qWarning("Warning test");
		qCritical("Critical test");

		//Redirect Ogre's logging system to our own
		//This has to all be done before creating the Ogre::Root object.
		mOgreLog = mLogManager->createLog("Ogre");
		mLogManager->setVisibleLog(mOgreLog);
		mInternalOgreLogManager = new Ogre::LogManager();
		mInternalOgreLog = mInternalOgreLogManager->createLog("Ogre.log", false, true, true);
		mInternalOgreLog->addListener(this);

		//erzeuge ein neues
		mAppLog = mLogManager->createLog("Application");
		
		//logMessage
	}

	QWidget* QtEditorApp::getOgreWidget()
	{
		return (QWidget*)mOgreWidget;
	}

	void QtEditorApp::logMessage(QString msg)
	{
		if(mAppLog)
			mAppLog->logMessage(msg,LL_INFO);
	}

	void QtEditorApp::loadRenderSystemsFromPlugins(void)
	{
	#if defined(Q_WS_WIN)
		try
		{
			#if defined(_DEBUG)
				mRoot->loadPlugin("RenderSystem_GL_d");
			#else
				mRoot->loadPlugin("RenderSystem_GL");
			#endif
		}
		catch(...)
		{
			qWarning("Failed to load OpenGL plugin");
		}
		try
		{
			#if defined(_DEBUG)
				mRoot->loadPlugin("RenderSystem_Direct3D9_d");
			#else
				mRoot->loadPlugin("RenderSystem_Direct3D9");
			#endif
		}
		catch(...)
		{
			qWarning("Failed to load Direct3D9 plugin");
		}
	#endif
	#if defined(Q_WS_X11)
		try
		{
			mRoot->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL");
			//mRoot->loadPlugin("/usr/lib64/OGRE/RenderSystem_GL"); //Needed for Matt
		}
		catch(...)
		{
			qWarning("Failed to load OpenGL plugin");
		}
	#endif
	#if defined(Q_WS_MAC)
		try
		{
			mRoot->loadPlugin("RenderSystem_GL");
		}
		catch(...)
		{
			qWarning("Failed to load OpenGL plugin");
		}
	#endif

		Ogre::RenderSystemList list = Ogre::Root::getSingletonPtr()->getAvailableRenderers();
		Ogre::RenderSystemList::iterator i = list.begin();

		while (i != list.end())
		{
			if ((*i)->getName() == "OpenGL Rendering Subsystem")
			{
				mOpenGLRenderSystem = *i;
			}
			if ((*i)->getName() == "Direct3D9 Rendering Subsystem")
			{
				mDirect3D9RenderSystem = *i;
			}
			i++;
		}

		if(!(mOpenGLRenderSystem || mDirect3D9RenderSystem))
		{
			qCritical("No rendering subsystems found");
		}
	}

	void QtEditorApp::initialiseOgre(void)
	{
		//QString renderSystem = mSettingsDialog->mSettings->value("Graphics/RenderSystem").toString();
  //      mActiveRenderSystem = mDirect3D9RenderSystem;
		//if(renderSystem.compare("OpenGL Rendering Subsystem") == 0)
		//{
		//	mActiveRenderSystem = mOpenGLRenderSystem;
		//}
		//if(renderSystem.compare("Direct3D9 Rendering Subsystem") == 0)
		//{
		//	mActiveRenderSystem = mDirect3D9RenderSystem;
		//}

		//Ogre::Root::getSingletonPtr()->setRenderSystem(mActiveRenderSystem);

		Ogre::Root::getSingletonPtr()->initialise(false);
	}

	void QtEditorApp::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName)
	{
		//Convert message to Qt's string type.
		QString messageAsQString = QString::fromStdString(message);

		//Map Ogre's LogMessageLevels to our LogLevels
		switch(lml)
		{
		case Ogre::LML_TRIVIAL:
			mOgreLog->logMessage(messageAsQString, QtOgre::LL_DEBUG);
			break;
		case Ogre::LML_NORMAL:
			mOgreLog->logMessage(messageAsQString, QtOgre::LL_INFO);
			break;
		default: //Should be Ogre::LML_CRITICAL
			mOgreLog->logMessage(messageAsQString, QtOgre::LL_ERROR);
		}
	}

	/*SettingsDialog* QtEditorApp::settingsDialog(void)
	{
		return mSettingsDialog;
	}*/

	int QtEditorApp::exec(bool displaySettingsDialog)
	{
		////If we don't show the setting dialog, or we do show it and it is accepted, then proceed.
		//if((displaySettingsDialog == false) || ( static_cast<QtEditorApp *>(QCoreApplication::instance())->showSettingsDialog() == QDialog::Accepted))
		//{
		//	static_cast<QtEditorApp *>(QCoreApplication::instance())->initialise();
			return QApplication::exec();
		//}
		////Otherwise the user cancelled so exit.
		//else
		//{
		//	return 0;
		//}
	}

	/*Centers a widget inside its parent, or inside the desktop if no
	parent is provided. This requires the widget to be free to move.
	If not (e.g. its in a layout) then the behaviour is undefined*/
	void QtEditorApp::centerWidget(QWidget* widgetToCenter, QWidget* parent)
	{
		QRect parentGeometry;
		if(parent != 0)
		{
			parentGeometry = parent->frameGeometry();
		}
		else
		{
			parentGeometry = desktop()->availableGeometry();
		}

		int xOffset = (parentGeometry.width() - widgetToCenter->frameGeometry().width()) / 2;
		int yOffset = (parentGeometry.height() - widgetToCenter->frameGeometry().height()) / 2;
		widgetToCenter->move(parentGeometry.x() + xOffset, parentGeometry.y() + yOffset);
	}

	void QtEditorApp::showInfoMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Information");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-information.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	void QtEditorApp::showWarningMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Warning");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-warning.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	void QtEditorApp::showErrorMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-error.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	void QtEditorApp::hideSettingsDialog(void)
	{
		mSettingsDialog->reject();
	}

	int QtEditorApp::showSettingsDialog(void)
	{
		return mSettingsDialog->exec();
	}

    QtOgre::Log* QtEditorApp::createLog(const QString& name)
	{
		return mLogManager->createLog(name);
	}

	void QtEditorApp::hideLogManager(void)
	{
		mLogManager->setVisible(false);
	}

	void QtEditorApp::showLogManager(void)
	{
		mLogManager->setVisible(true);
	}

	Ogre::RenderWindow* QtEditorApp::ogreRenderWindow(void) const
	{
		return mOgreWidget->getOgreRenderWindow();
	}

	QSettings* QtEditorApp::settings(void) const
	{
		return mSettings;
	}

	void QtEditorApp::setUpdateInterval(int intervalInMilliseconds)
	{
		/*if(mUpdateTimer->isActive())
		{
			mUpdateTimer->stop();
		}
		mUpdateTimer->start(intervalInMilliseconds);*/
		mUpdateTimer->setInterval(intervalInMilliseconds);
	}
}

//
//
//inline void initQtResources() { Q_INIT_RESOURCE(resources); }
//
//
//	void qtMessageHandler(QtMsgType type, const char* msg)
//     {
//         switch (type)
//		 {
//         case QtDebugMsg:
//             static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance())->_systemLog()->logMessage(msg, QtOgre::LL_DEBUG);
//             break;
//         case QtWarningMsg:
//             static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance())->_systemLog()->logMessage(msg, QtOgre::LL_WARNING);
//             break;
//         case QtCriticalMsg:
//             static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance())->_systemLog()->logMessage(msg, QtOgre::LL_ERROR);
//             break;
//         case QtFatalMsg:
//			 //We don't override this one as we are dying and wouldn't see it.
//             fprintf(stderr, "Fatal: %s\n", msg);
//             abort();
//         }
//     }
//namespace QtOgre
//{
//	QtEditorApp::QtEditorApp(int& argc, char** argv, QtOgre::EditGameLogic* gameLogic)
//	:QApplication(argc, argv)
//	,mOpenGLRenderSystem(0)
//	,mDirect3D9RenderSystem(0)
//	,mFrameCounter(0),
//    preShutdownDone(false)
//	{
//		mGameLogic = gameLogic;
//		if(mGameLogic != 0)
//		{
//            mGameLogic->mRealApp = this;//Application = static_cast<QtOgre::Application*>(const_cast<TestApplication*>(this));
//		}
//
//		//Initialise the resources.
//		initQtResources();
//
//		mUpdateTimer = new QTimer;
//		QObject::connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
//		//On the test system, a value of one here gives a high frame rate and still allows
//		//event prcessing to take place. A value of 0 doubles the frame rate but the mouse
//		//becomes jumpy. This property should probably be configurable.
//		mUpdateTimer->setInterval(1);
//
//		//Load the settings file. If it doesn't exist it is created.
//		
//        /*test = new QMainWindow();
//        Ui::MainWindow *asd = new Ui::MainWindow();
//        asd->setupUi(test);*/
//        //asd->subwindow_4->resize(300,600);
//        
//		mOgreWidget = new QtOgre::EventHandlingOgreWidget(0, 0);
//        //asd->mdiArea->addSubWindow(mOgreWidget);
//		//Logging should be initialised ASAP, and before Ogre::Root is created.
//		initialiseLogging();
//
//		//Create the root and load render system plugins. We do that here so that we know
//		//what render systems are available by the time we show the settings dialog.
//		//Note that the render system is not initialised until the user selects one.
//		mRoot = new Ogre::Root();
//		loadRenderSystemsFromPlugins();
//
//        mSettings = new QSettings("Pra","qtogretest"); //("settings.ini", QSettings::IniFormat);
//        
//        //erzeuge nun das mSettings basierend auf ogre-standardconfig, weil qtogre zu dumm dafür sit
//        /*[Graphics]
//            RenderSystem=OpenGL Rendering Subsystem
//            WindowModes=800x600, 1024x768, 1280x1024, FullScreen
//            SelectedWindowMode=1
//            AllowPerfHUD=false
//            EnableGammaCorrection=false
//            FSSAFactor=2
//            EnableVerticalSync=false
//        Es ist also alles standard, abgesehen von der windowmode-sache...        
//        */
//         //jetzt die optionen
//        QStringList windowModeList;
//        windowModeList.clear();
//        int windowModeIndex = -1;
//
//        Ogre::RenderSystem *rs = *mRoot->getAvailableRenderers()->begin();
//        //eines suchen
//        for (Ogre::RenderSystemList::const_iterator pRend = mRoot->getAvailableRenderers()->begin(); pRend != mRoot->getAvailableRenderers()->end(); ++pRend)
//        {
//            Ogre::RenderSystem* cur = *pRend;
//            if(Ogre::StringUtil::match(cur->getName(),"*Direct3D*"))
//            {
//                rs = cur;
//               
//                break;
//            }
//            
//        }
//        
//      
//        mSettings->setValue("RenderSystem",rs->getName().c_str());
//        QStringList test;
//        test.push_back("800x600");
//        test.push_back("1024x768");
//        test.push_back("1280x1024");
//        test.push_back("FullScreen");
//       
//        mSettings->setValue("Graphics/WindowModes",test);//"800x600, 1024x768, 1280x1024, FullScreen");
//        mSettings->setValue("Graphics/SelectedWindowMode",1);
//        showWarningMessageBox(mSettings->value("Graphics/SelectedWindowMode", "asd").toString());
//        mSettings->setValue("AllowPerfHUD","false");
//        mSettings->setValue("EnableGammaCorrection","false");
//        mSettings->setValue("FSSAFactor","2");
//        mSettings->setValue("EnableVerticalSync","false");
//        
//
//        //Ogre::ConfigOptionMap::iterator pOpt;
//        //for(pOpt = rs->getConfigOptions().begin(); pOpt != rs->getConfigOptions().end;pOpt++)
//        //{
//        //    Ogre::ConfigOption cur = pOpt->second;
//        //    if(cur.name == "Full Screen")
//        //    {
//        //        windowModeList.append("FullScreen");
//        //        if(cur.currentValue == "Yes");
//        //        //windowModeIndex++
//        //        continue;
//        //        //mSettings->setValue("
//        //    }
//        //}
//        //TiXmlElement *option = rsElem->FirstChildElement("option");
//        //ConfigOptionMap cfg = rs->getConfigOptions();
//        //while(option)
//        //{
//        //    Ogre::String optName = option->Attribute("name");
//        //    Ogre::String optValue= option->Attribute("value");
//        //    ConfigOption curOption = cfg[optName];
//        //    //schauen, ob es das ausgewählte überhaupt gibt
//        //    bool valueFound = false;
//        //    for(Ogre::StringVector::iterator itr = curOption.possibleValues.begin();itr!=curOption.possibleValues.end();itr++)
//        //    {
//        //        if(*itr == optValue)
//        //        {
//        //            rs->setConfigOption(optName,optValue);
//        //            valueFound = true;
//        //            break;
//        //        }
//        //    }           
//        //    if(!valueFound)
//        //        LogManager::getSingletonPtr()->logMessage("Warning: Invalid value '"+optValue+
//        //        "' for property '"+optName+"' of Render System '"+rsName+"'! Default value '"+curOption.currentValue+"' is used instead");
//        //    //ansonsten ignorieren
//        //    option = option->NextSiblingElement("option");
//        //}
//
//		mSettingsDialog = new QtOgre::SettingsDialog(mSettings, mOgreWidget);
//		mGraphicsSettingsWidget = new QtOgre::GraphicsSettingsWidget;
//		mSettingsDialog->addSettingsWidget("Graphics", mGraphicsSettingsWidget);
//
//		////Initialise the resources.
//		//initQtResources();
//
//		//mUpdateTimer = new QTimer;
//		//QObject::connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
//		////On the test system, a value of one here gives a high frame rate and still allows
//		////event prcessing to take place. A value of 0 doubles the frame rate but the mouse
//		////becomes jumpy. This property should probably be configurable.
//		//mUpdateTimer->setInterval(1);
//
//		////Load the settings file. If it doesn't exist it is created.
//		//mSettings = new QSettings("Pra","qtogretest"); //("settings.ini", QSettings::IniFormat);
//  //      
//  //      //mainWindow = new QMainWindow();
//  //    /*  mainWnd = new Ui::MainWindow(this);
//  //      mainWnd->setupUi();
//  //      */
//  //      //bool wat = QObject::connect(asd->actionExit, SIGNAL(triggered(bool)), this, SLOT(doTest(bool)));
//  //      //asd->subwindow_4->resize(300,600);
//  //      //connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
//
//  //      //mainWindow->close
//  //      //connect(
//  //     //Logging should be initialised ASAP, and before Ogre::Root is created.
//  //      //but after mSettings exists
//		//initialiseLogging();
//
//		//mOgreWidget = new QtOgre::EventHandlingOgreWidget(0, 0);
//  //      
//  //      
//  //      
//  //  /*    mainWnd->mdiArea->addSubWindow(mLogManager);        
//  //      mainWnd->mdiArea->addSubWindow(mOgreWidget);*/
//		////Create the root and load render system plugins. We do that here so that we know
//		////what render systems are available by the time we show the settings dialog.
//		////Note that the render system is not initialised until the user selects one.
//		//mRoot = new Ogre::Root();
//		//loadRenderSystemsFromPlugins();
//
//
//  //      
//  //      //erzeuge nun das mSettings basierend auf ogre-standardconfig, weil qtogre zu dumm dafür sit
//  //      /*[Graphics]
//  //          RenderSystem=OpenGL Rendering Subsystem
//  //          WindowModes=800x600, 1024x768, 1280x1024, FullScreen
//  //          SelectedWindowMode=1
//  //          AllowPerfHUD=false
//  //          EnableGammaCorrection=false
//  //          FSSAFactor=2
//  //          EnableVerticalSync=false
//  //      Es ist also alles standard, abgesehen von der windowmode-sache...        
//  //      */
//  //       //jetzt die optionen
//  //      QStringList windowModeList;
//  //      windowModeList.clear();
//  //      int windowModeIndex = -1;
//
//  //      Ogre::RenderSystem *rs = *mRoot->getAvailableRenderers()->begin();
//  //      //eines suchen
//  //      for (Ogre::RenderSystemList::const_iterator pRend = mRoot->getAvailableRenderers()->begin(); pRend != mRoot->getAvailableRenderers()->end(); ++pRend)
//  //      {
//  //          Ogre::RenderSystem* cur = *pRend;
//  //          if(Ogre::StringUtil::match(cur->getName(),"*Direct3D*"))
//  //          {
//  //              rs = cur;
//  //             
//  //              break;
//  //          }
//  //          
//  //      }
//  //      
//  //    
//  //      mSettings->setValue("RenderSystem",rs->getName().c_str());
//  //      QStringList test;
//  //      test.push_back("800x600");
//  //      test.push_back("1024x768");
//  //      test.push_back("1280x1024");
//  //      test.push_back("FullScreen");
//  //     
//  //      mSettings->setValue("Graphics/WindowModes",test);//"800x600, 1024x768, 1280x1024, FullScreen");
//  //      mSettings->setValue("Graphics/SelectedWindowMode",1);
//  //      //showWarningMessageBox(mSettings->value("Graphics/SelectedWindowMode", "asd").toString());
//  //      mSettings->setValue("AllowPerfHUD","false");
//  //      mSettings->setValue("EnableGammaCorrection","false");
//  //      mSettings->setValue("FSSAFactor","2");
//  //      mSettings->setValue("EnableVerticalSync","false");
//  //      
//
//  //      //Ogre::ConfigOptionMap::iterator pOpt;
//  //      //for(pOpt = rs->getConfigOptions().begin(); pOpt != rs->getConfigOptions().end;pOpt++)
//  //      //{
//  //      //    Ogre::ConfigOption cur = pOpt->second;
//  //      //    if(cur.name == "Full Screen")
//  //      //    {
//  //      //        windowModeList.append("FullScreen");
//  //      //        if(cur.currentValue == "Yes");
//  //      //        //windowModeIndex++
//  //      //        continue;
//  //      //        //mSettings->setValue("
//  //      //    }
//  //      //}
//  //      //TiXmlElement *option = rsElem->FirstChildElement("option");
//  //      //ConfigOptionMap cfg = rs->getConfigOptions();
//  //      //while(option)
//  //      //{
//  //      //    Ogre::String optName = option->Attribute("name");
//  //      //    Ogre::String optValue= option->Attribute("value");
//  //      //    ConfigOption curOption = cfg[optName];
//  //      //    //schauen, ob es das ausgewählte überhaupt gibt
//  //      //    bool valueFound = false;
//  //      //    for(Ogre::StringVector::iterator itr = curOption.possibleValues.begin();itr!=curOption.possibleValues.end();itr++)
//  //      //    {
//  //      //        if(*itr == optValue)
//  //      //        {
//  //      //            rs->setConfigOption(optName,optValue);
//  //      //            valueFound = true;
//  //      //            break;
//  //      //        }
//  //      //    }           
//  //      //    if(!valueFound)
//  //      //        LogManager::getSingletonPtr()->logMessage("Warning: Invalid value '"+optValue+
//  //      //        "' for property '"+optName+"' of Render System '"+rsName+"'! Default value '"+curOption.currentValue+"' is used instead");
//  //      //    //ansonsten ignorieren
//  //      //    option = option->NextSiblingElement("option");
//  //      //}
//
//		//mSettingsDialog = new QtOgre::SettingsDialog(mSettings, mOgreWidget);
//		//mGraphicsSettingsWidget = new QtOgre::GraphicsSettingsWidget;
//		//mSettingsDialog->addSettingsWidget("Graphics", mGraphicsSettingsWidget);
//	}
//    void QtEditorApp::doTest(bool checked)
//    {
//        showWarningMessageBox("hm");
//        int test = 2;
//        test += 5;
//    }
//	QtEditorApp::~QtEditorApp()
//	{
//        preShutdown();
//		if(mOgreWidget)
//		{
//			delete mOgreWidget;
//			mOgreWidget = 0;
//		}
//		if (mRoot)
//		{
//            delete mRoot;
//			mRoot = 0;
//		}
//	}
//
//	bool QtEditorApp::isOpenGLAvailable(void) const
//	{
//		return mOpenGLRenderSystem != 0;
//	}
//
//	bool QtEditorApp::isDirect3D9Available(void) const
//	{
//		return mDirect3D9RenderSystem != 0;
//	}
//
//	unsigned int QtEditorApp::frameCount(void) const
//	{
//		return mFrameCounter;
//	}
//
//	/**This function is an implementation detail, and should not really be exposed.
//	It return the log which the QtOgre framework uses for its messages, whereas
//	users are expected to instead create their own log with createLog(). The reason
//	it is exposed is that the Qt debugging system (qtMessageHandler()) also redirects
//	to this log, and that cannot be made a member function.*/
//    QtOgre::Log* QtEditorApp::_systemLog(void) const
//	{
//		return mSystemLog;
//	}
//
//	QWidget* QtEditorApp::mainWidget(void) const
//	{
//		return mOgreWidget;
//	}
//
//	void QtEditorApp::applySettings(void)
//	{
//		//Eventually QtEditorApp settings might be applied here.
//		//Until then, we just pass the settings on the the MainWindow and GameLogic
//        
//		if(!mOgreWidget->applySettings(mSettings))
//		{
//			showWarningMessageBox("Unable to apply desired settings to the window.\nPlease consult the system log for details");
//		}
//
//		/*if(mGameLogic != 0)
//		{
//			mGameLogic->applySettings(mSettings);
//		}*/
//	}
//
//	void QtEditorApp::initialise(void)
//	{
//        mOgreWidget->show();
//		mOgreWidget->resize(800,600);
//		centerWidget(mOgreWidget);
//
//
//
//		//mLogManagerDockWidget = mMainWindow->addAsDockWidget(mLogManager, "Logs", Qt::AllDockWidgetAreas);
//		//mLogManager->setParent(mMainWindow);
//		//centerWidget(mLogManager, mOgreWidget);
//
//
//		mLogManager->show();
//
//
//
//		mLogManager->setForceProcessEvents(true);
//		initialiseOgre();
//		Ogre::NameValuePairList ogreWindowParams;
//		//ogreWindowParams["FSAA"] = "8";
//		mOgreWidget->initialise(&ogreWindowParams);
//		mGameLogic->initialise();
//		mLogManager->setForceProcessEvents(false);
//
//		//mLogManager->hide();
//
//
//
//
//		mOgreWidget->setEventHandler(mGameLogic);
//
//		//This is a bit of a hack, necessary because we want to use the settings dialog in two different
//		//ways. The first time it is shown (by TestApplication::exec()) no slot is connected - the Accepted
//		//event is handled explicitly because the system is not initialised at that point. But now (and
//		//when the dialog is shown in future) we are ready for it, so we connect the accepted() signal.
//		//We also call accept, to do the initial setup. See also TestApplication::exec().
//		connect(mSettingsDialog, SIGNAL(accepted()), this, SLOT(applySettings()));
//		mSettingsDialog->accept();
//
//       
//        //asd->setupUi(asd
//        /*QMainWindow *asd = new QMainWindow();
//        test = new Ui_MainWindow();
//        asd->show();*/
//
//        mFPSDialog = new QtOgre::FPSDialog(mOgreWidget, Qt::FramelessWindowHint);
//		mFPSDialog->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());
//		mFPSDialog->move(mainWidget()->geometry().topLeft() + QPoint(10,10));
//		mFPSDialog->show();
//
//		mLogManager->move(mainWidget()->geometry().left() + 10, mainWidget()->geometry().top() + mainWidget()->geometry().height() - mLogManager->frameGeometry().height() - 10);
//
//		mUpdateTimer->start();
////        mainWnd->show();
//        
//     
//  //    
//		//mOgreWidget->show();
//		//mOgreWidget->resize(800,600);
//		//centerWidget(mOgreWidget);
//
//
//
//		////mLogManagerDockWidget = mMainWindow->addAsDockWidget(mLogManager, "Logs", Qt::AllDockWidgetAreas);
//		////mLogManager->setParent(mMainWindow);
//		////centerWidget(mLogManager, mOgreWidget);
//
//
//		//mLogManager->show();
//
//
//
//		//mLogManager->setForceProcessEvents(true);
//		//initialiseOgre();
//		//Ogre::NameValuePairList ogreWindowParams;
//		////ogreWindowParams["FSAA"] = "8";
//		//mOgreWidget->initialise(&ogreWindowParams);
//		//mGameLogic->initialise();
//		//mLogManager->setForceProcessEvents(false);
//
//		////mLogManager->hide();
//
//
//
//
//		//mOgreWidget->setEventHandler(mGameLogic);
//
//		////This is a bit of a hack, necessary because we want to use the settings dialog in two different
//		////ways. The first time it is shown (by QtEditorApp::exec()) no slot is connected - the Accepted
//		////event is handled explicitly because the system is not initialised at that point. But now (and
//		////when the dialog is shown in future) we are ready for it, so we connect the accepted() signal.
//		////We also call accept, to do the initial setup. See also QtEditorApp::exec().
//		//connect(mSettingsDialog, SIGNAL(accepted()), this, SLOT(applySettings()));
//		//mSettingsDialog->accept();
//
//  //     
//  //      //asd->setupUi(asd
//  //      /*QMainWindow *asd = new QMainWindow();
//  //      test = new Ui_MainWindow();
//  //      asd->show();*/
//
//  //    /*  mFPSDialog = new QtOgre::FPSDialog(mOgreWidget, Qt::FramelessWindowHint);
//		//mFPSDialog->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());
//		//mFPSDialog->move(mainWidget()->geometry().topLeft() + QPoint(10,10));
//		//mFPSDialog->show();*/
//
//		//mLogManager->move(mainWidget()->geometry().left() + 10, mainWidget()->geometry().top() + mainWidget()->geometry().height() - mLogManager->frameGeometry().height() - 10);
//
//		//mUpdateTimer->start();
//	}
//
//	void QtEditorApp::update(void)
//	{
//		mGameLogic->update();
//		mOgreWidget->update();
//		++mFrameCounter;
//	}
//    void QtEditorApp::preShutdown(){
//        if(preShutdownDone)
//            return;
//        mUpdateTimer->stop();
//		mGameLogic->shutdown();
//        
//		mInternalOgreLog->removeListener(this);
//		delete mLogManager;
//        preShutdownDone = true;
//    }
//
//	void QtEditorApp::shutdown(void)
//	{
//		preShutdown();
//        this->exit(0);
//	}
//
//	void QtEditorApp::initialiseLogging(void)
//	{
//        //Initialise our logging system
//		mLogManager = new QtOgre::LogManager(mOgreWidget);
//		mLogManager->resize(550, 400);
////		mLogManager->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());
//
//		//Redirect Qt's logging system to our own
//		mSystemLog = mLogManager->createLog("System");
//		qInstallMsgHandler(&qtMessageHandler);
//		qDebug("Debug test");
//		qWarning("Warning test");
//		qCritical("Critical test");
//
//		//Redirect Ogre's logging system to our own
//		//This has to all be done before creating the Ogre::Root object.
//		mOgreLog = mLogManager->createLog("Ogre");
//		mLogManager->setVisibleLog(mOgreLog);
//		mInternalOgreLogManager = new Ogre::LogManager();
//		mInternalOgreLog = mInternalOgreLogManager->createLog("Ogre.log", false, true, true);
//		mInternalOgreLog->addListener(this);
////		//Initialise our logging system
////		mLogManager = new QtOgre::LogManager(mOgreWidget);
////		mLogManager->resize(550, 400);
//////		mLogManager->setWindowOpacity(settings()->value("System/DefaultWindowOpacity", 1.0).toDouble());
////
////		//Redirect Qt's logging system to our own
////		mSystemLog = mLogManager->createLog("System");
//
//        //mSystemLog = new FakeLog("sys");
//        //mOgreLog = new FakeLog("ogre");
////		qInstallMsgHandler(&qtMessageHandler);
////		qDebug("Debug test");
////		qWarning("Warning test");
////		qCritical("Critical test");
////
////		//Redirect Ogre's logging system to our own
////		//This has to all be done before creating the Ogre::Root object.
////		mOgreLog = mLogManager->createLog("Ogre");
////		mLogManager->setVisibleLog(mOgreLog);
////		mInternalOgreLogManager = new Ogre::LogManager();
////		mInternalOgreLog = mInternalOgreLogManager->createLog("Ogre.log", false, true, true);
////		mInternalOgreLog->addListener(this);
//	}
//
//	void QtEditorApp::loadRenderSystemsFromPlugins(void)
//	{
//	#if defined(Q_WS_WIN)
//		try
//		{
//			#if defined(_DEBUG)
//				mRoot->loadPlugin("RenderSystem_GL_d");
//			#else
//				mRoot->loadPlugin("RenderSystem_GL");
//			#endif
//		}
//		catch(...)
//		{
//			qWarning("Failed to load OpenGL plugin");
//		}
//		try
//		{
//			#if defined(_DEBUG)
//				mRoot->loadPlugin("RenderSystem_Direct3D9_d");
//			#else
//				mRoot->loadPlugin("RenderSystem_Direct3D9");
//			#endif
//		}
//		catch(...)
//		{
//			qWarning("Failed to load Direct3D9 plugin");
//		}
//	#endif
//	#if defined(Q_WS_X11)
//		try
//		{
//			mRoot->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL");
//			//mRoot->loadPlugin("/usr/lib64/OGRE/RenderSystem_GL"); //Needed for Matt
//		}
//		catch(...)
//		{
//			qWarning("Failed to load OpenGL plugin");
//		}
//	#endif
//	#if defined(Q_WS_MAC)
//		try
//		{
//			mRoot->loadPlugin("RenderSystem_GL");
//		}
//		catch(...)
//		{
//			qWarning("Failed to load OpenGL plugin");
//		}
//	#endif
//
//		Ogre::RenderSystemList *list = Ogre::Root::getSingletonPtr()->getAvailableRenderers();
//		Ogre::RenderSystemList::iterator i = list->begin();
//
//		while (i != list->end())
//		{
//			if ((*i)->getName() == "OpenGL Rendering Subsystem")
//			{
//				mOpenGLRenderSystem = *i;
//			}
//			if ((*i)->getName() == "Direct3D9 Rendering Subsystem")
//			{
//				mDirect3D9RenderSystem = *i;
//			}
//			i++;
//		}
//
//		if(!(mOpenGLRenderSystem || mDirect3D9RenderSystem))
//		{
//			qCritical("No rendering subsystems found");
//		}
//	}
//
//	void QtEditorApp::initialiseOgre(void)
//	{
//		QString renderSystem = mSettingsDialog->mSettings->value("Graphics/RenderSystem").toString();
//        mActiveRenderSystem = mDirect3D9RenderSystem;
//		if(renderSystem.compare("OpenGL Rendering Subsystem") == 0)
//		{
//			mActiveRenderSystem = mOpenGLRenderSystem;
//		}
//		if(renderSystem.compare("Direct3D9 Rendering Subsystem") == 0)
//		{
//			mActiveRenderSystem = mDirect3D9RenderSystem;
//		}
//
//		Ogre::Root::getSingletonPtr()->setRenderSystem(mActiveRenderSystem);
//
//		Ogre::Root::getSingletonPtr()->initialise(false);
//	}
//
//	void QtEditorApp::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName)
//	{
//		//Convert message to Qt's string type.
//		QString messageAsQString = QString::fromStdString(message);
//
//		//Map Ogre's LogMessageLevels to our LogLevels
//		switch(lml)
//		{
//		case Ogre::LML_TRIVIAL:
//			mOgreLog->logMessage(messageAsQString, QtOgre::LL_DEBUG);
//			break;
//		case Ogre::LML_NORMAL:
//			mOgreLog->logMessage(messageAsQString, QtOgre::LL_INFO);
//			break;
//		default: //Should be Ogre::LML_CRITICAL
//			mOgreLog->logMessage(messageAsQString, QtOgre::LL_ERROR);
//		}
//	}
//
//	/*SettingsDialog* QtEditorApp::settingsDialog(void)
//	{
//		return mSettingsDialog;
//	}*/
//
//	int QtEditorApp::exec(bool displaySettingsDialog)
//	{
//		//If we don't show the setting dialog, or we do show it and it is accepted, then proceed.
//		if((displaySettingsDialog == false) || ( static_cast<QtEditorApp *>(QCoreApplication::instance())->showSettingsDialog() == QDialog::Accepted))
//		{
//			 static_cast<QtEditorApp *>(QCoreApplication::instance())->initialise();
//			return QApplication::exec();
//		}
//		//Otherwise the user cancelled so exit.
//		else
//		{
//			return 0;
//		}
//	}
//
//	/*Centers a widget inside its parent, or inside the desktop if no
//	parent is provided. This requires the widget to be free to move.
//	If not (e.g. its in a layout) then the behaviour is undefined*/
//	void QtEditorApp::centerWidget(QWidget* widgetToCenter, QWidget* parent)
//	{
//		QRect parentGeometry;
//		if(parent != 0)
//		{
//			parentGeometry = parent->frameGeometry();
//		}
//		else
//		{
//			parentGeometry = desktop()->availableGeometry();
//		}
//
//		int xOffset = (parentGeometry.width() - widgetToCenter->frameGeometry().width()) / 2;
//		int yOffset = (parentGeometry.height() - widgetToCenter->frameGeometry().height()) / 2;
//		widgetToCenter->move(parentGeometry.x() + xOffset, parentGeometry.y() + yOffset);
//	}
//
//	void QtEditorApp::showInfoMessageBox(const QString& text)
//	{
//		QMessageBox msgBox;
//		msgBox.setWindowTitle("Information");
//		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-information.svg"));
//		msgBox.setStandardButtons(QMessageBox::Ok);
//		msgBox.setText(text);
//		msgBox.exec();
//	}
//
//	void QtEditorApp::showWarningMessageBox(const QString& text)
//	{
//		QMessageBox msgBox;
//		msgBox.setWindowTitle("Warning");
//		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-warning.svg"));
//		msgBox.setStandardButtons(QMessageBox::Ok);
//		msgBox.setText(text);
//		msgBox.exec();
//	}
//
//	void QtEditorApp::showErrorMessageBox(const QString& text)
//	{
//		QMessageBox msgBox;
//		msgBox.setWindowTitle("Error");
//		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-error.svg"));
//		msgBox.setStandardButtons(QMessageBox::Ok);
//		msgBox.setText(text);
//		msgBox.exec();
//	}
//
//	void QtEditorApp::hideSettingsDialog(void)
//	{
//		mSettingsDialog->reject();
//	}
//
//	int QtEditorApp::showSettingsDialog(void)
//	{
//		return mSettingsDialog->exec();
//	}
//
//    QtOgre::Log* QtEditorApp::createLog(const QString& name)
//	{
//		return mLogManager->createLog(name);
//	}
//
//	void QtEditorApp::hideLogManager(void)
//	{
//		mLogManager->setVisible(false);
//	}
//
//	void QtEditorApp::showLogManager(void)
//	{
//		mLogManager->setVisible(true);
//	}
//
//	Ogre::RenderWindow* QtEditorApp::ogreRenderWindow(void) const
//	{
//		return mOgreWidget->getOgreRenderWindow();
//	}
//
//	QSettings* QtEditorApp::settings(void) const
//	{
//		return mSettings;
//	}
//
//	void QtEditorApp::setUpdateInterval(int intervalInMilliseconds)
//	{
//		/*if(mUpdateTimer->isActive())
//		{
//			mUpdateTimer->stop();
//		}
//		mUpdateTimer->start(intervalInMilliseconds);*/
//		mUpdateTimer->setInterval(intervalInMilliseconds);
//	}
//}