/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QtCore>
#include <QtWidgets>
#include "ui_windowMain.h"
#include "ui_barFeaturesHarris.h"
#include "ui_barFeaturesFast.h"
#include "ui_barFeaturesSift.h"
#include "ui_barFeaturesSurf.h"
#include "windowImage.h"
#include "windowCaptureWebcam.h"
#include "windowPreferences.h"
#include "windowAbout.h"
#include "windowStartup.h"
#include "windowFastRealTime.h"
#include "windowDo4.h"

class WindowMain : public QMainWindow, public Ui::windowMain {
  Q_OBJECT
  public:
	WindowMain();
	void updateRecentFilesMenu();
	void showWindowImage(WindowImage*);

	QToolButton* mToolButtonOpenRecent;
	QMenu* mMenuRecentFiles;
	int mCapturedWebcamImages;
	int mTotalImages;

  public slots:
	void open();
	void captureWebcam();
	void openFastRT();

  private:
	void applyCommonTasks();
	void loadFile(QString);
	void closeEvent(QCloseEvent*);
	void saveSettings();
	void setRecentFile(QString);
	void removeRecentFile(QString);

	bool mSeparatorOpenWindowsAdded; // Adding the separator on Qt Designer doesn't work
	QSettings* mSettings;
	QAction* mActionExit;
	QAction* mActionSeparatorRecentFiles;
	QSignalMapper* mSignalMapper;
	QActionGroup* mActionGroupZoom;
	QActionGroup* mActionGroupFeatures;
	QActionGroup* mActionGroupWindow;
	enum { maxRecentFiles=8 };
	QAction* mActionRecentFiles[maxRecentFiles];
	QAction* mHarrisAction;
	QAction* mFastAction;
	QAction* mSiftAction;
	QAction* mSurfAction;
	QAction* mCurrentFeatureAction;
	QList<QAction*>* mSubwindowActions;
	Ui::barFeaturesHarris mUIHarris;
	Ui::barFeaturesFast mUIFast;
	Ui::barFeaturesSift mUISift;
	Ui::barFeaturesSurf mUISurf;
	QWidget* mHarrisToolBar;
	QWidget* mSiftToolBar;
	QWidget* mSurfToolBar;
	QWidget* mFastToolBar;

	QMdiSubWindow* mActiveWindow;
	WindowImage* mActiveWindowImage;
	QIcon* mIconHarris;
	QIcon* mIconFAST;
	QIcon* mIconSIFT;
	QIcon* mIconSURF;
	QLabel* mStatusBarLabelZoom;
	QLabel* mStatusBarLabelDimensions;
	QLabel* mStatusBarLabelSize;
	QLabel* mStatusBarLabelTime;
	QLabel* mStatusBarLabelIcon;
	QLabel* mStatusBarLabelKeypoints;
	QLabel* mStatusBarLabelSpaceRight;
	QLabel* mStatusBarLabelSpaceLeft;
	QFrame* mStatusBarLine;
	QFrame* mStatusBarLine2;
	QFrame* mStatusBarLine3;

  private slots:
	void saveCopyAs();
	void preferences();
	void exit();
	
	void copy();
	void resetImage();
	
	void startupDialog();
	void zoom();
	
	void showHarrisToolBar();
	void applyHarris();
	void saveHarrisParams();
	void resetHarrisParams();
	
	void showFastToolBar();
	void applyFast();
	void saveFastParams();
	void restFastParams();
	
	void showSiftToolBar();
	void applySift();
	void saveSiftParams();
	void resetSiftParams();
	
	void showSurfToolBar();
	void applySurf();
	void saveSurfParams();
	void resetSurfParams();
	
	void do4();
	
	void tile();
	void cascade();
	void duplicate();
	void closeActiveSubWindow();
	void closeAllSubWindows();
	
	void website();
	void about();

	void updateWindowMenu(QMdiSubWindow*);
	void openRecentFile();
	void setActiveSubWindow(QWidget*);
};

#endif