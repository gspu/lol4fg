#ifndef __praTestApp
#define __praTestApp

#include <OgreLog.h>
#include <Qt\QSettings.h>
#include <QtGui\QApplication>
#include <QtGui\QMainWindow>
#include <MainWindow.h>
//#include "FakeLogManager.h"
//making my own application class, which is pretty much a copy of QtOgre::Application,
//because QtOgre::Application is much too anal about too much stuff
//for example the config ini created in the constructor
//



class Ui_MainWindow;

namespace QtOgre
{
	class EventHandlingOgreWidget;
	class FPSDialog;
	class GameLogic;
	class GraphicsSettingsWidget;
	class Log;
	class LogManager;
	class SettingsDialog;

    class EditGameLogic;



class QtEditorApp : public QApplication, public Ogre::LogListener
	{
        
		Q_OBJECT
		
		//friend class EditorApp;
	public:
		/**
		 * Creates an instance of the QtEditorApp class.
		 */
        QtEditorApp(int & argc, char ** argv, QtOgre::EditGameLogic* gameLogic = 0);
		/**
		 * Destroys an instance of the QtEditorApp class
		 */
		~QtEditorApp();

		///\name Getters
		//@{
		/**
		 * The total number of frames rendered
		 * \return the number of frames rendered since the application started.
		 */
		unsigned int frameCount(void) const;
		/**
		 * Get the OGRE Ogre::RenderWindow for adding viewports
		 * \return a pointer to the Ogre Ogre::RenderWindow
		 */   
		Ogre::RenderWindow* ogreRenderWindow(void) const;
		/**
		 * Get the main window widget
		 * \return a pointer to the applications main window.
		 */
		QWidget* mainWidget(void) const;
		/**
		 * Access the application settings
		 * \return a pointer to the application settings
		 */
        QSettings* settings(void) const;
		/**
		 * \return the log used by the QtOgre framework.
		 */
		QtOgre::Log* _systemLog(void) const;
		//@}

		///\name Setters
		//@{
		/**
		 * Sets the period between sucessive updates.
		 * \param intervalInMilliseconds the period between sucessive updates
		 */
		void setUpdateInterval(int intervalInMilliseconds);
		//@}

		///\name Testers
		//@{
		///\return whether the OpenGL render system is available.
		bool isOpenGLAvailable(void) const;
		///\return whether the Direct3D9 render system is available.
		bool isDirect3D9Available(void) const;
		//@}
		
		///\name Other
		//@{
		/**
		 * Creates a new log with a given name.
		 * \param name the name of the log
		 * \return a pointer to the log
		 */
		QtOgre::Log* createLog(const QString& name);

		void logMessage(QString msg);
		void logMessage(Ogre::String msg)
		{
			logMessage(QString(msg.c_str()));
		}

		///Hides the LogManager window.
		void hideLogManager(void);
		///Hides the settings dialog.
		void hideSettingsDialog(void);
		///Shows the LogManager window.
		void showLogManager(void);
		///Shows the settings dialog.
		int showSettingsDialog(void);
		//@}
		
		//Static functions
		/**
		 * Start the main event loop.
		 * \param displaySettingsDialog should the settings dialog be displayed
		 * \return the application return code
		 * \todo Make the bool a self-documenting enum (http://doc.trolltech.com/qq/qq13-apis.html#thebooleanparametertrap)
		 */
		static int exec(bool displaySettingsDialog = true); //Use 'displaySettingsDialog' because there is already a function called 'showSettingsDialog'.

		/**
		 * Utility function to center a widget.
		 * \param widgetToCenter the widget to centre
		 * \param parent the parent of the widget
		 */
		static void centerWidget(QWidget* widgetToCenter, QWidget* parent = 0);

		/**
		 * Shows a message box with an 'Info' icon and 'Information' in the title bar.
		 * \param text the text to display
		 */
		static void showInfoMessageBox(const QString& text);
		/**
		 * Shows a message box with a 'Warning' icon and 'Warning' in the title bar.
		 * \param text the text to display
		 */
		static void showWarningMessageBox(const QString& text);
		/**
		 * Shows a message box with an 'Error' icon and 'Error' in the title bar.
		 * \param text the text to display
		 */
		static void showErrorMessageBox(const QString& text);
        
        inline QtOgre::EditGameLogic* getGameLogic()
        {
            return mGameLogic;
        }

		inline Ogre::Root* getRoot()
		{
			return mRoot;
		}

		inline QtOgre::LogManager* getLogManager()
		{
			return mLogManager;
		}

		//workaround, der das Ding als QWidget zur?ckgibt, und zwar ohne dass
		//die .h des eventhandlingogrewidgets included werden muss
		QWidget* getOgreWidget();

		//holt position der maus auf dem ogrewidget
		QPoint getOgreWidgetMousePosition();
							

				//Widgets
		EditorMainWindow *mainWnd;
		QtOgre::FPSDialog* mFPSDialog;
		QtOgre::GraphicsSettingsWidget* mGraphicsSettingsWidget;
		QtOgre::EventHandlingOgreWidget* mOgreWidget;
		QtOgre::SettingsDialog* mSettingsDialog;
		
	public slots:
		void applySettings(void);
		void initialise(void);
		//das wird danach aufgerufen, wenn ogre ganz online ist. es erstellt noch paar fenster und so
		void postInit(void);
		void shutdown(void);
		void update(void);		

		void enablePageAutoloading(bool enable);

	private:
		//Private functions
		void initialiseLogging(void);
		void initialiseOgre(void);
		void loadRenderSystemsFromPlugins(void);
		///Implemented from Ogre::LogListener to redirect logging 
		void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName);

        //TERST
        //QMainWindow *test;
        
        ////subwindows
        /*QMdiSubWindow *ogreSubWindow;
        QMdiSubWindow *logSubWindow; */  
      



		//Logging
		Ogre::Log* mInternalOgreLog;
		Ogre::LogManager* mInternalOgreLogManager;
		QtOgre::LogManager* mLogManager;
		QtOgre::Log* mOgreLog;
		QtOgre::Log* mSystemLog;
		QtOgre::Log* mAppLog;

		//Ogre Stuff
		Ogre::RenderSystem* mActiveRenderSystem;
		Ogre::RenderSystem* mOpenGLRenderSystem;
		Ogre::RenderSystem* mDirect3D9RenderSystem;
		Ogre::Root* mRoot;

		//Misc
		unsigned int mFrameCounter;
        QtOgre::EditGameLogic* mGameLogic;
		QTimer* mUpdateTimer;
		QSettings* mSettings;

		//FENSTER
	};

}
//This redefines qApp, causing it to return an QtEditorApp pointer instead of a QApplication one.
//This is useful, for example, to access the logging system. This is done in the same way that
//Qt does it to get a QApplication rather than a QCoreApplication, so it should be legitimate.
#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance()))


#endif

//class Ui_MainWindow;
//
//namespace QtOgre
//{
//	class EventHandlingOgreWidget;
//	class FPSDialog;
//	class GameLogic;
//	class GraphicsSettingsWidget;
//	class Log;
//	class LogManager;
//	class SettingsDialog;
//
//    class EditGameLogic;
//
//
//
//class QtEditorApp : public QApplication, public Ogre::LogListener
//	{
//        
//		Q_OBJECT
//
//	public:
//		/**
//		 * Creates an instance of the QtEditorApp class.
//		 */
//        QtEditorApp(int & argc, char ** argv, QtOgre::EditGameLogic* gameLogic = 0);
//		/**
//		 * Destroys an instance of the QtEditorApp class
//		 */
//		~QtEditorApp();
//
//		///\name Getters
//		//@{
//		/**
//		 * The total number of frames rendered
//		 * \return the number of frames rendered since the application started.
//		 */
//		unsigned int frameCount(void) const;
//		/**
//		 * Get the OGRE Ogre::RenderWindow for adding viewports
//		 * \return a pointer to the Ogre Ogre::RenderWindow
//		 */   
//		Ogre::RenderWindow* ogreRenderWindow(void) const;
//		/**
//		 * Get the main window widget
//		 * \return a pointer to the applications main window.
//		 */
//		QWidget* mainWidget(void) const;
//		/**
//		 * Access the application settings
//		 * \return a pointer to the application settings
//		 */
//        QSettings* settings(void) const;
//		/**
//		 * \return the log used by the QtOgre framework.
//		 */
//		QtOgre::Log* _systemLog(void) const;
//		//@}
//
//        
//        void preShutdown();
//		///\name Setters
//		//@{
//		/**
//		 * Sets the period between sucessive updates.
//		 * \param intervalInMilliseconds the period between sucessive updates
//		 */
//		void setUpdateInterval(int intervalInMilliseconds);
//		//@}
//
//		///\name Testers
//		//@{
//		///\return whether the OpenGL render system is available.
//		bool isOpenGLAvailable(void) const;
//		///\return whether the Direct3D9 render system is available.
//		bool isDirect3D9Available(void) const;
//		//@}
//		
//		///\name Other
//		//@{
//		/**
//		 * Creates a new log with a given name.
//		 * \param name the name of the log
//		 * \return a pointer to the log
//		 */
//		QtOgre::Log* createLog(const QString& name);
//		///Hides the LogManager window.
//		void hideLogManager(void);
//		///Hides the settings dialog.
//		void hideSettingsDialog(void);
//		///Shows the LogManager window.
//		void showLogManager(void);
//		///Shows the settings dialog.
//		int showSettingsDialog(void);
//		//@}
//		
//		//Static functions
//		/**
//		 * Start the main event loop.
//		 * \param displaySettingsDialog should the settings dialog be displayed
//		 * \return the application return code
//		 * \todo Make the bool a self-documenting enum (http://doc.trolltech.com/qq/qq13-apis.html#thebooleanparametertrap)
//		 */
//		static int exec(bool displaySettingsDialog = true); //Use 'displaySettingsDialog' because there is already a function called 'showSettingsDialog'.
//
//		/**
//		 * Utility function to center a widget.
//		 * \param widgetToCenter the widget to centre
//		 * \param parent the parent of the widget
//		 */
//		static void centerWidget(QWidget* widgetToCenter, QWidget* parent = 0);
//
//		/**
//		 * Shows a message box with an 'Info' icon and 'Information' in the title bar.
//		 * \param text the text to display
//		 */
//		static void showInfoMessageBox(const QString& text);
//		/**
//		 * Shows a message box with a 'Warning' icon and 'Warning' in the title bar.
//		 * \param text the text to display
//		 */
//		static void showWarningMessageBox(const QString& text);
//		/**
//		 * Shows a message box with an 'Error' icon and 'Error' in the title bar.
//		 * \param text the text to display
//		 */
//		static void showErrorMessageBox(const QString& text);
//        
//	public slots:
//		void applySettings(void);
//		void initialise(void);
//		void shutdown(void);
//		void update(void);	
//        void doTest(bool checked = false);
//
//	private:
//        //
//        bool preShutdownDone;
//		//Private functions
//		void initialiseLogging(void);
//		void initialiseOgre(void);
//		void loadRenderSystemsFromPlugins(void);
//		///Implemented from Ogre::LogListener to redirect logging 
//		void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName);
//
//        //TERST
//        //QMainWindow *mainWindow;
//        Ui::MainWindow *mainWnd;
//		//Widgets
//		QtOgre::FPSDialog* mFPSDialog;
//		QtOgre::GraphicsSettingsWidget* mGraphicsSettingsWidget;
//		QtOgre::EventHandlingOgreWidget* mOgreWidget;
//		QtOgre::SettingsDialog* mSettingsDialog;
//
//		//Logging
//		Ogre::Log* mInternalOgreLog;
//		Ogre::LogManager* mInternalOgreLogManager;
//		QtOgre::LogManager* mLogManager;
//		QtOgre::Log* mOgreLog;
//		QtOgre::Log* mSystemLog;
//
//		//Ogre Stuff
//		Ogre::RenderSystem* mActiveRenderSystem;
//		Ogre::RenderSystem* mOpenGLRenderSystem;
//		Ogre::RenderSystem* mDirect3D9RenderSystem;
//		Ogre::Root* mRoot;
//
//		//Misc
//		unsigned int mFrameCounter;
//        QtOgre::EditGameLogic* mGameLogic;
//		QTimer* mUpdateTimer;
//		QSettings* mSettings;
//	};
//
//}
////This redefines qApp, causing it to return an QtEditorApp pointer instead of a QApplication one.
////This is useful, for example, to access the logging system. This is done in the same way that
////Qt does it to get a QApplication rather than a QCoreApplication, so it should be legitimate.
//#if defined(qApp)
//#undef qApp
//#endif
//#define qApp (static_cast<QtOgre::QtEditorApp *>(QCoreApplication::instance()))
//
//
//#endif
