/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QtCore>
#include <QtWidgets>
#include "ui_mainWindow.h"
#include "ui_featureHarrisWindow.h"
#include "ui_featureSURFWindow.h"
#include "ui_featureFASTWindow.h"
#include "ui_featureSIFTWindow.h"
#include "windowImage.h"
#include "windowCaptureWebcam.h"
#include "windowPreferences.h"
#include "windowAbout.h"
#include "windowStartup.h"
#include "windowFastRealTime.h"

class WindowMain : public QMainWindow, public Ui::mainWindow {
  Q_OBJECT
  public:
	WindowMain();
	void updateRecentFilesMenu();
	QSettings* getSettings();

	enum language {english=0, espanol=1, italiano=2};
	QToolButton* toolButtonOpenRecent;
	QMenu* menuRecentFiles;
	int capturedWebcamImages;

  public slots:
	void open();
	void captureWebcam();

  protected:
	void mouseDoubleClickEvent(QMouseEvent* event);

  private:
	void loadFile(QString fileName);
	void closeEvent(QCloseEvent* eventConstr);
	void saveSettings();
	void setRecentFile(QString fileName);

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
	Ui::formFeaturesHarris mUIHarris;
	Ui::formFeaturesFAST mUIFast;
	Ui::formFeaturesSIFT mUISift;
	Ui::formFeaturesSURF mUISurf;
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
	void openFastRT();
	
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