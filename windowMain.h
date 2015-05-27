/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QtCore>
#include <QtGui>
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

	QSettings* mySettings;
	QAction* actionExit;
	QAction* separatorRecentFiles;
	QSignalMapper* mySignalMapper;
	QActionGroup* myActionGroupZoom;
	QActionGroup* myActionGroupFeatures;
	QActionGroup* myActionGroupWindow;
	enum { maxRecentFiles=8 };
	QAction* actionRecentFiles[maxRecentFiles];
	Ui::formFeaturesHarris myUIHarris;
	Ui::formFeaturesFAST myUIFAST;
	Ui::formFeaturesSIFT myUISIFT;
	Ui::formFeaturesSURF myUISURF;
	QWidget* myWidgetHarris;
	QWidget* myWidgetSIFT;
	QWidget* myWidgetSURF;
	QWidget* myWidgetFAST;

	QMdiSubWindow* myActiveWindow;
	WindowImage* myActiveWindowImage;
	QIcon* icon16Harris;
	QIcon* icon16FAST;
	QIcon* icon16SIFT;
	QIcon* icon16SURF;
	QLabel* myStatusBarLabelZoom;
	QLabel* myStatusBarLabelDimensions;
	QLabel* myStatusBarLabelSize;
	QLabel* myStatusBarLabelTime;
	QLabel* myStatusBarLabelIcon;
	QLabel* myStatusBarLabelKeypoints;
	QLabel* myStatusBarLabelSpaceRight;
	QLabel* myStatusBarLabelSpaceLeft;
	QFrame* myStatusBarLine;
	QFrame* myStatusBarLine2;
	QFrame* myStatusBarLine3;

  private slots:
	void saveCopyAs();
	void exit();
	void copy();
	void preferences();
	void startupDialog();
	void setToolBar();
	void zoom();
	void harris();
	void harrisApplyParams();
	void harrisSaveParams();
	void harrisReset();
	void harrisClose();
	void fast();
	void fastApplyParams();
	void fastSaveParams();
	void fastReset();
	void fastClose();
	void sift();
	void siftApplyParams();
	void siftSaveParams();
	void siftReset();
	void siftClose();
	void surf();
	void surfApplyParams();
	void surfSaveParams();
	void surfReset();
	void surfClose();
	void resetImage();
	void do4();
	void fastRT();
	void tile();
	void cascade();
	void duplicate();
	void closeActiveSubWindow();
	void closeAllSubWindows();
	void officialWebsite();
	void about();

	void updateMenus(QMdiSubWindow*);
	void openRecentFile();
	void setActiveSubWindow(QWidget*);
};

#endif