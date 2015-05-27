#include "windowMain.h"

WindowMain::WindowMain() {
	setupUi(this);

	QApplication::setWindowIcon(QIcon("IFD128.png"));
	icon16Harris = new QIcon("icons16/Harris16.png");
	icon16FAST = new QIcon("icons16/FAST16.png");
	icon16SIFT = new QIcon("icons16/SIFT16.png");
	icon16SURF = new QIcon("icons16/SURF16.png");
	mySettings = new QSettings("imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	resize(mySettings->value("size", QSize(700, 480)).toSize());
	move(mySettings->value("pos", QPoint(150, 40)).toPoint());
	toolBarFile->setVisible(mySettings->value("toolBarFile", true).toBool());
	toolBarZoom->setVisible(mySettings->value("toolBarZoom", true).toBool());
	toolBarFeatures->setVisible(mySettings->value("toolBarFeatures", true).toBool());

	menuRecentFiles = new QMenu(this);
	for(int n=0; n<maxRecentFiles; ++n) {
		actionRecentFiles[n] = new QAction(this);
		actionRecentFiles[n]->setVisible(false);
		connect(actionRecentFiles[n], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}
	for (int n=0; n<maxRecentFiles; ++n) menuFile->addAction(actionRecentFiles[n]);
	separatorRecentFiles = menuFile->addSeparator();
	separatorRecentFiles->setVisible(false);
	updateRecentFilesMenu();
	actionExit = new QAction(this);
	actionExit->setObjectName(QString::fromUtf8("actionExit"));
	actionExit->setIcon(QIcon("icons16/window-close.png"));
	actionExit->setText(QApplication::translate("mainWindow", "Exit", 0, QApplication::UnicodeUTF8));
	actionExit->setShortcut(QApplication::translate("mainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
	menuFile->addAction(actionExit);

	toolButtonOpenRecent = new QToolButton(this);
	toolButtonOpenRecent->setFocusPolicy(Qt::NoFocus);
	toolButtonOpenRecent->setIcon(QIcon("icons24/01 document-open.png"));
	toolButtonOpenRecent->setPopupMode(QToolButton::MenuButtonPopup);
	toolButtonOpenRecent->setMenu(menuRecentFiles);
	toolButtonOpenRecent->setToolButtonStyle(Qt::ToolButtonIconOnly);
	toolButtonOpenRecent->setAutoRaise(true);
	toolButtonOpenRecent->setDefaultAction(actionOpen);
	toolBarFile->insertWidget(actionCaptureWebcam, toolButtonOpenRecent);

	myActionGroupZoom = new QActionGroup(this);
	myActionGroupZoom->setEnabled(false);
	myActionGroupZoom->addAction(actionZoomIn);
	myActionGroupZoom->addAction(actionZoomOut);
	myActionGroupZoom->addAction(actionZoomOriginal);
	myActionGroupZoom->addAction(actionZoomBestFit);
	
	myActionGroupFeatures = new QActionGroup(this);
	myActionGroupFeatures->setEnabled(false);
	myActionGroupFeatures->addAction(actionSIFT);
	myActionGroupFeatures->addAction(actionSURF);
	myActionGroupFeatures->addAction(actionHarris);
	myActionGroupFeatures->addAction(actionFAST);
	myActionGroupFeatures->addAction(actionDo4);

	myActionGroupWindow = new QActionGroup(this);
	myActionGroupWindow->setEnabled(false);
	myActionGroupWindow->addAction(actionTile);
	myActionGroupWindow->addAction(actionCascade);
	myActionGroupWindow->addAction(actionNext);
	myActionGroupWindow->addAction(actionPrevious);
	myActionGroupWindow->addAction(actionDuplicate);
	myActionGroupWindow->addAction(actionClose);
	myActionGroupWindow->addAction(actionCloseAll);

	myStatusBarLabelZoom = new QLabel(statusbar);
	myStatusBarLabelZoom->setAlignment(Qt::AlignHCenter);
	myStatusBarLabelDimensions = new QLabel(statusbar);
	myStatusBarLabelDimensions->setAlignment(Qt::AlignHCenter);
	myStatusBarLabelSize = new QLabel(statusbar);
	myStatusBarLabelSize->setAlignment(Qt::AlignHCenter);
	myStatusBarLabelTime = new QLabel(statusbar);
	myStatusBarLabelTime->setAlignment(Qt::AlignHCenter);
	myStatusBarLabelIcon = new QLabel(statusbar);
	myStatusBarLabelIcon->setAlignment(Qt::AlignHCenter);
	myStatusBarLabelIcon->setVisible(false);
	myStatusBarLabelKeypoints = new QLabel(statusbar);
	myStatusBarLabelKeypoints->setAlignment(Qt::AlignHCenter);
	myStatusBarLabelSpaceRight = new QLabel(statusbar);
	myStatusBarLabelSpaceRight->setMinimumSize(0, 1);
	myStatusBarLabelSpaceLeft = new QLabel(statusbar);
	myStatusBarLabelSpaceLeft->setMinimumSize(0, 1);
	myStatusBarLine = new QFrame(statusbar);
	myStatusBarLine->setVisible(false);
	myStatusBarLine->setFrameShape(QFrame::VLine);
	myStatusBarLine->setFrameShadow(QFrame::Sunken);
	myStatusBarLine2 = new QFrame(myStatusBarLabelTime);
	myStatusBarLine2->setFrameShape(QFrame::VLine);
	myStatusBarLine2->setFrameShadow(QFrame::Sunken);
	myStatusBarLine2->setVisible(false);
	myStatusBarLine3 = new QFrame(myStatusBarLabelTime);
	myStatusBarLine3->setFrameShape(QFrame::VLine);
	myStatusBarLine3->setFrameShadow(QFrame::Sunken);
	myStatusBarLine3->setVisible(false);
	statusbar->addWidget(myStatusBarLabelSpaceLeft);
	statusbar->addWidget(myStatusBarLabelZoom);
	statusbar->addPermanentWidget(myStatusBarLabelTime);
	statusbar->addPermanentWidget(myStatusBarLine2);
	statusbar->addPermanentWidget(myStatusBarLabelIcon);
	statusbar->addPermanentWidget(myStatusBarLabelKeypoints);
	statusbar->addPermanentWidget(myStatusBarLine3);
	statusbar->addPermanentWidget(myStatusBarLabelDimensions);
	statusbar->addPermanentWidget(myStatusBarLine);
	statusbar->addPermanentWidget(myStatusBarLabelSize);
	statusbar->addPermanentWidget(myStatusBarLabelSpaceRight);
	
	myWidgetHarris = new QWidget();
	myWidgetHarris->setObjectName(QString::fromUtf8("Harris Features"));
	myUIHarris.setupUi(myWidgetHarris);
	myUIHarris.comboBoxSobelApertureSize->setCurrentIndex(mySettings->value("harris/sobelApertureSize", 2).toInt());
	myUIHarris.spinBoxHarrisApertureSize->setValue(mySettings->value("harris/harrisApertureSize", 1).toInt());
	myUIHarris.doubleSpinBoxKValue->setValue(mySettings->value("harris/kValue", 0.01).toDouble());
	connect(myUIHarris.comboBoxSobelApertureSize, SIGNAL(currentIndexChanged(int)), this, SLOT(harrisSaveParams()));
	connect(myUIHarris.spinBoxHarrisApertureSize, SIGNAL(editingFinished()), this, SLOT(harrisSaveParams()));
	connect(myUIHarris.doubleSpinBoxKValue, SIGNAL(editingFinished()), this, SLOT(harrisSaveParams()));
	connect(myUIHarris.buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(harrisApplyParams()));
	connect(myUIHarris.buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(harrisReset()));
	connect(myUIHarris.toolButtonClose, SIGNAL(clicked()), this, SLOT(harrisClose()));
	
	myWidgetFAST = new QWidget();
	myWidgetFAST->setObjectName(QString::fromUtf8("FAST Features"));
	myUIFAST.setupUi(myWidgetFAST);
	myUIFAST.spinBoxThreshold->setValue(mySettings->value("fast/threshold", 50).toInt());
	myUIFAST.pushButtonNonMax->setChecked(mySettings->value("fast/nonMaxSuppression", true).toBool());
	connect(myUIFAST.spinBoxThreshold, SIGNAL(editingFinished()), this, SLOT(fastSaveParams()));
	connect(myUIFAST.pushButtonNonMax, SIGNAL(toggled(bool)), this, SLOT(fastSaveParams()));
	connect(myUIFAST.buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(fastApplyParams()));
	connect(myUIFAST.buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(fastReset()));
	connect(myUIFAST.toolButtonClose, SIGNAL(clicked()), this, SLOT(fastClose()));
	
	myWidgetSIFT = new QWidget();
	myWidgetFAST->setObjectName(QString::fromUtf8("SIFT Features"));
	myUISIFT.setupUi(myWidgetSIFT);
	myUISIFT.doubleSpinBoxThreshold->setValue(mySettings->value("sift/threshold", 0.014).toDouble());
	myUISIFT.doubleSpinBoxEdgeThreshold->setValue(mySettings->value("sift/edgeThreshold", 10.0).toDouble());
	myUISIFT.spinBoxOctaves->setValue(mySettings->value("sift/octaves", 3).toInt());
	myUISIFT.spinBoxLayers->setValue(mySettings->value("sift/layers", 1).toInt());
	myUISURF.pushButtonShowOrientation->setChecked(mySettings->value("sift/showOrientation", true).toBool());
	connect(myUISIFT.doubleSpinBoxThreshold, SIGNAL(editingFinished()), this, SLOT(siftSaveParams()));
	connect(myUISIFT.doubleSpinBoxEdgeThreshold, SIGNAL(editingFinished()), this, SLOT(siftSaveParams()));
	connect(myUISIFT.spinBoxOctaves, SIGNAL(editingFinished()), this, SLOT(siftSaveParams()));
	connect(myUISIFT.spinBoxLayers, SIGNAL(editingFinished()), this, SLOT(siftSaveParams()));
	connect(myUISIFT.pushButtonShowOrientation, SIGNAL(toggled(bool)), this, SLOT(siftSaveParams()));
	connect(myUISIFT.buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(siftApplyParams()));
	connect(myUISIFT.buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(siftReset()));
	connect(myUISIFT.toolButtonClose, SIGNAL(clicked()), this, SLOT(siftClose()));
	
	myWidgetSURF = new QWidget();
	myWidgetFAST->setObjectName(QString::fromUtf8("SURF Features"));
	myUISURF.setupUi(myWidgetSURF);
	myUISURF.spinBoxThreshold->setValue(mySettings->value("surf/threshold", 4000).toInt());
	myUISURF.spinBoxOctaves->setValue(mySettings->value("surf/octaves", 3).toInt());
	myUISURF.spinBoxLayers->setValue(mySettings->value("surf/layers", 1).toInt());
	myUISURF.pushButtonShowOrientation->setChecked(mySettings->value("surf/showOrientation", true).toBool());
	connect(myUISURF.spinBoxThreshold, SIGNAL(editingFinished()), this, SLOT(surfSaveParams()));
	connect(myUISURF.spinBoxOctaves, SIGNAL(editingFinished()), this, SLOT(surfSaveParams()));
	connect(myUISURF.spinBoxLayers, SIGNAL(editingFinished()), this, SLOT(surfSaveParams()));
	connect(myUISURF.pushButtonShowOrientation, SIGNAL(toggled(bool)), this, SLOT(surfSaveParams()));
	connect(myUISURF.buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(surfApplyParams()));
	connect(myUISURF.buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(surfReset()));
	connect(myUISURF.toolButtonClose, SIGNAL(clicked()), this, SLOT(surfClose()));
	
	switch (mySettings->value("startupParameters", 1).toInt()) {
	    case 1: toolBarParameters->addWidget(myWidgetHarris); break;
	    case 2: toolBarParameters->addWidget(myWidgetFAST); break;
	    case 3: toolBarParameters->addWidget(myWidgetSIFT); break;
	    case 4: toolBarParameters->addWidget(myWidgetSURF); break;
	}
	toolBarParameters->setEnabled(false);
	

	connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(actionCaptureWebcam, SIGNAL(triggered()), this, SLOT(captureWebcam()));
	connect(actionSaveCopyAs, SIGNAL(triggered()), this, SLOT(saveCopyAs()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));
	connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
	connect(actionPreferences, SIGNAL(triggered()), this, SLOT(preferences()));
	connect(actionStartupDialog, SIGNAL(triggered()), this, SLOT(startupDialog()));
	connect(actionToolBarFile, SIGNAL(triggered()), this, SLOT(setToolBar()));
	connect(actionToolBarZoom, SIGNAL(triggered()), this, SLOT(setToolBar()));
	connect(actionToolBarFeatures, SIGNAL(triggered()), this, SLOT(setToolBar()));
	connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoom()));
	connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoom()));
	connect(actionZoomOriginal, SIGNAL(triggered()), this, SLOT(zoom()));
	connect(actionZoomBestFit, SIGNAL(triggered()), this, SLOT(zoom()));
	connect(actionSIFT, SIGNAL(triggered()), this, SLOT(sift()));
	connect(actionSURF, SIGNAL(triggered()), this, SLOT(surf()));
	connect(actionHarris, SIGNAL(triggered()), this, SLOT(harris()));
	connect(actionFAST, SIGNAL(triggered()), this, SLOT(fast()));
	connect(actionResetImage, SIGNAL(triggered()), this, SLOT(resetImage()));
	connect(actionDo4, SIGNAL(triggered()), this, SLOT(do4()));
	connect(actionFastRT, SIGNAL(triggered()), this, SLOT(fastRT()));
	connect(actionTile, SIGNAL(triggered()), this, SLOT(tile()));
	connect(actionCascade, SIGNAL(triggered()), this, SLOT(cascade()));
	connect(actionNext, SIGNAL(triggered()), myMdiArea, SLOT(activateNextSubWindow()));
	connect(actionPrevious, SIGNAL(triggered()), myMdiArea, SLOT(activatePreviousSubWindow()));
	connect(actionDuplicate, SIGNAL(triggered()), this, SLOT(duplicate()));
	connect(actionClose, SIGNAL(triggered()), this, SLOT(closeActiveSubWindow()));
	connect(actionCloseAll, SIGNAL(triggered()), this, SLOT(closeAllSubWindows()));
	connect(actionOfficialWebsite, SIGNAL(triggered()), this, SLOT(officialWebsite()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(myMdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus(QMdiSubWindow*)));
	mySignalMapper = new QSignalMapper(this);
	connect(mySignalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

	if (mySettings->value("maximized", true).toBool()) showMaximized();
	else show();
	if (mySettings->value("startupDialog", true).toBool()) startupDialog();

	actionToolBarFile->setChecked(toolBarFile->isVisible());
	actionToolBarZoom->setChecked(toolBarZoom->isVisible());
	actionToolBarFeatures->setChecked(toolBarFeatures->isVisible());
}




void WindowMain::open() {
	loadFile(QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.bmp *.jpg)")));
}




void WindowMain::captureWebcam() {
  WindowCaptureWebcam* myWindowCaptureWebcam = new WindowCaptureWebcam(this);
}




void WindowMain::saveCopyAs() {
	QString myFileName = QFileDialog::getSaveFileName(0, tr("Save Copy As"), QFileInfo(myActiveWindowImage->windowImageTitle).baseName().append(".png"), tr("Images (*.bmp *.png)"));
	if (!myFileName.isEmpty()) myActiveWindowImage->myPixmap.save(myFileName);
}




void WindowMain::exit() {
	saveSettings();
	close();
}




void WindowMain::copy() {
	QApplication::clipboard()->setPixmap(myActiveWindowImage->myPixmap);
}




void WindowMain::preferences() {
	WindowPreferences* myWindowPreferences = new WindowPreferences(this);
}




void WindowMain::startupDialog() {
	 new WindowStartup(this);
}




void WindowMain::setToolBar() {
	if (sender()==actionToolBarFile) {
		toolBarFile->setVisible(!(toolBarFile->isVisible()));
		actionToolBarFile->setChecked(toolBarFile->isVisible());
	} else if (sender()==actionToolBarZoom) {
		toolBarZoom->setVisible(!(toolBarZoom->isVisible()));
		actionToolBarZoom->setChecked(toolBarZoom->isVisible());
	} else if (sender()==actionToolBarFeatures) {
		toolBarFeatures->setVisible(!(toolBarFeatures->isVisible()));
		actionToolBarFeatures->setChecked(toolBarFeatures->isVisible());
	}
	saveSettings();
}




void WindowMain::zoom() {
	if (sender()==actionZoomIn) myActiveWindowImage->zoomIn();
	else if (sender()==actionZoomOut) myActiveWindowImage->zoomOut();
	else if (sender()==actionZoomOriginal) myActiveWindowImage->zoomOriginal();
	else if (sender()==actionZoomBestFit) myActiveWindowImage->zoomBestFit();
	actionZoomIn->setEnabled(myActiveWindowImage->currentFactor < 3.0);
	actionZoomOut->setEnabled(myActiveWindowImage->currentFactor > 0.25);
	myStatusBarLabelZoom->setText(myActiveWindowImage->myImageZoom);
}




void WindowMain::harris() {
    if (toolBarParameters->findChild<QWidget*>(QString::fromUtf8("Harris Features")))
	 if (toolBarParameters->isVisible()) {
	     actionHarris->setChecked(false);
	     toolBarParameters->setVisible(false);
	 }
	 else {
	     actionHarris->setChecked(true);
	     toolBarParameters->setVisible(true);
	 }
    else {
	toolBarParameters->clear();
	toolBarParameters->setWindowTitle("Parameters Toolbar: Harris Features");
	toolBarParameters->addWidget(myWidgetHarris);
	mySettings->setValue("startupParameters", 1);
	toolBarParameters->setVisible(true);
    }
}




void WindowMain::harrisReset() {
	myUIHarris.comboBoxSobelApertureSize->setCurrentIndex(1);
	myUIHarris.spinBoxHarrisApertureSize->setValue(2);
	myUIHarris.doubleSpinBoxKValue->setValue(0.01);
	harrisSaveParams();
}




void WindowMain::harrisApplyParams() {
	int sobelApertureSize;
	switch (mySettings->value("harris/sobelApertureSize", 1).toInt()) {
	  case 0: sobelApertureSize=1; break;
	  case 1: sobelApertureSize=3; break;
	  case 2: sobelApertureSize=5; break;
	  case 3: sobelApertureSize=7;
	}
	myActiveWindowImage->harris(sobelApertureSize,
		mySettings->value("harris/harrisApertureSize", 2).toInt(),
		mySettings->value("harris/kValue", 0.01).toDouble());
	actionResetImage->setEnabled(true);
	myActiveWindowImage->feature=WindowImage::harrisType;
	myActiveWindow->setWindowIcon(*icon16Harris);
	myStatusBarLabelTime->setText(myActiveWindowImage->myImageTime);
	myStatusBarLabelKeypoints->setText(myActiveWindowImage->myImageKeypoints);
	myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/Harris16.png")));
	myStatusBarLabelIcon->setVisible(true);
	myStatusBarLine2->setVisible(true);
	myStatusBarLine3->setVisible(true);
}




void WindowMain::harrisClose() {
	actionHarris->setChecked(false);
	toolBarParameters->setVisible(false);
}




void WindowMain::harrisSaveParams() {
	mySettings->setValue("harris/sobelApertureSize", myUIHarris.comboBoxSobelApertureSize->currentIndex());
	mySettings->setValue("harris/harrisApertureSize", myUIHarris.spinBoxHarrisApertureSize->value());
	mySettings->setValue("harris/kValue", myUIHarris.doubleSpinBoxKValue->value());
}




void WindowMain::fast() {
    if (toolBarParameters->findChild<QWidget*>(QString::fromUtf8("FAST Features")))
	 if (toolBarParameters->isVisible()) {
	     actionHarris->setChecked(false);
	     toolBarParameters->setVisible(false);
	 }
	 else {
	     actionHarris->setChecked(true);
	     toolBarParameters->setVisible(true);
	 }
    else {
	toolBarParameters->clear();
	toolBarParameters->setWindowTitle("Parameters Toolbar: FAST Features");
	toolBarParameters->addWidget(myWidgetFAST);
	mySettings->setValue("startupParameters", 2);
	toolBarParameters->setVisible(true);
    }
}




void WindowMain::fastReset() {
	myUIFAST.spinBoxThreshold->setValue(50);
	myUIFAST.pushButtonNonMax->setChecked(true);
	fastSaveParams();
}




void WindowMain::fastApplyParams() {
	myActiveWindowImage->fast(mySettings->value("fast/threshold", 50).toInt(),
		mySettings->value("fast/nonMaxSuppression", true).toBool());
	actionResetImage->setEnabled(true);
	myActiveWindowImage->feature=WindowImage::fastType;
	myActiveWindow->setWindowIcon(*icon16FAST);
	myStatusBarLabelTime->setText(myActiveWindowImage->myImageTime);
	myStatusBarLabelKeypoints->setText(myActiveWindowImage->myImageKeypoints);
	myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/FAST16.png")));
	myStatusBarLabelIcon->setVisible(true);
	myStatusBarLine2->setVisible(true);
	myStatusBarLine3->setVisible(true);
}




void WindowMain::fastClose() {
	actionFAST->setChecked(false);
	toolBarParameters->setVisible(false);
}




void WindowMain::fastSaveParams() {
	mySettings->setValue("fast/threshold", myUIFAST.spinBoxThreshold->value());
	mySettings->setValue("fast/nonMaxSuppression", myUIFAST.pushButtonNonMax->isChecked());
}




void WindowMain::sift() {
    if (toolBarParameters->findChild<QWidget*>(QString::fromUtf8("SIFT Features")))
	 if (toolBarParameters->isVisible()) {
	     actionHarris->setChecked(false);
	     toolBarParameters->setVisible(false);
	 }
	 else {
	     actionHarris->setChecked(true);
	     toolBarParameters->setVisible(true);
	 }
    else {
	toolBarParameters->clear();
	toolBarParameters->setWindowTitle("Parameters Toolbar: SIFT Features");
	toolBarParameters->addWidget(myWidgetSIFT);
	mySettings->setValue("startupParameters", 3);
	toolBarParameters->setVisible(true);
    }
}




void WindowMain::siftReset() {
	myUISIFT.doubleSpinBoxThreshold->setValue(0.014);
	myUISIFT.doubleSpinBoxEdgeThreshold->setValue(10.0);
	myUISIFT.spinBoxOctaves->setValue(3);
	myUISIFT.spinBoxLayers->setValue(1);
	myUISIFT.pushButtonShowOrientation->setChecked(true);
	siftSaveParams();
}




void WindowMain::siftApplyParams() {
	myActiveWindowImage->sift(mySettings->value("sift/threshold", 0.014).toDouble(),
		mySettings->value("sift/edgeThreshold", 10.0).toDouble(),
		mySettings->value("sift/octaves", 3).toInt(),
		mySettings->value("sift/layers", 1).toInt(),
		mySettings->value("sift/showOrientation", true).toBool());
	actionResetImage->setEnabled(true);
	myActiveWindowImage->feature=WindowImage::siftType;
	myActiveWindow->setWindowIcon(*icon16SIFT);
	myStatusBarLabelTime->setText(myActiveWindowImage->myImageTime);
	myStatusBarLabelKeypoints->setText(myActiveWindowImage->myImageKeypoints);
	myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/SIFT16.png")));
	myStatusBarLabelIcon->setVisible(true);
	myStatusBarLine2->setVisible(true);
	myStatusBarLine3->setVisible(true);
}




void WindowMain::siftClose() {
	actionSIFT->setChecked(false);
	toolBarParameters->setVisible(false);
}




void WindowMain::siftSaveParams() {
	mySettings->setValue("sift/threshold", myUISIFT.doubleSpinBoxThreshold->value());
	mySettings->setValue("sift/edgeThreshold", myUISIFT.doubleSpinBoxEdgeThreshold->value());
	mySettings->setValue("sift/octaves", myUISIFT.spinBoxOctaves->value());
	mySettings->setValue("sift/layers", myUISIFT.spinBoxLayers->value());
	mySettings->setValue("sift/showOrientation", myUISIFT.pushButtonShowOrientation->isChecked());
}




void WindowMain::surf() {
    if (toolBarParameters->findChild<QWidget*>(QString::fromUtf8("SURF Features")))
	 if (toolBarParameters->isVisible()) {
	     actionHarris->setChecked(false);
	     toolBarParameters->setVisible(false);
	 }
	 else {
	     actionHarris->setChecked(true);
	     toolBarParameters->setVisible(true);
	 }
    else {
	toolBarParameters->clear();
	toolBarParameters->setWindowTitle("Parameters Toolbar: SURF Features");
	toolBarParameters->addWidget(myWidgetSURF);
	mySettings->setValue("startupParameters", 4);
	toolBarParameters->setVisible(true);
    }
}




void WindowMain::surfReset() {
	myUISURF.spinBoxThreshold->setValue(4000);
	myUISURF.spinBoxOctaves->setValue(3);
	myUISURF.spinBoxLayers->setValue(1);
	myUISURF.pushButtonShowOrientation->setChecked(true);
	surfSaveParams();
}




void WindowMain::surfApplyParams() {
	myActiveWindowImage->surf(mySettings->value("surf/threshold", 4000).toInt(),
		mySettings->value("surf/octaves", 3).toInt(),
		mySettings->value("surf/layers", 1).toInt(),
		0,
		mySettings->value("surf/showOrientation", true).toBool());
	actionResetImage->setEnabled(true);
	myActiveWindowImage->feature=WindowImage::surfType;
	myActiveWindow->setWindowIcon(*icon16SURF);
	myStatusBarLabelTime->setText(myActiveWindowImage->myImageTime);
	myStatusBarLabelKeypoints->setText(myActiveWindowImage->myImageKeypoints);
	myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/SURF16.png")));
	myStatusBarLabelIcon->setVisible(true);
	myStatusBarLine2->setVisible(true);
	myStatusBarLine3->setVisible(true);
}




void WindowMain::surfClose() {
	actionSURF->setChecked(false);
	toolBarParameters->setVisible(false);
}




void WindowMain::surfSaveParams() {
	mySettings->setValue("surf/threshold", myUISURF.spinBoxThreshold->value());
	mySettings->setValue("surf/octaves", myUISURF.spinBoxOctaves->value());
	mySettings->setValue("surf/layers", myUISURF.spinBoxLayers->value());
	mySettings->setValue("surf/showOrientation", myUISURF.pushButtonShowOrientation->isChecked());
}




void WindowMain::resetImage() {
	myActiveWindowImage->resetImage();
	actionResetImage->setEnabled(false);
	myActiveWindowImage->feature=WindowImage::none;
	myActiveWindow->setWindowIcon(QApplication::windowIcon());
	myStatusBarLabelIcon->clear();
	myStatusBarLabelIcon->setVisible(false);
	myStatusBarLabelTime->clear();
	myStatusBarLabelKeypoints->clear();
	myStatusBarLine2->setVisible(false);
	myStatusBarLine3->setVisible(false);
}




void WindowMain::do4() {
	statusbar->showMessage("Calculating features...");
	surfApplyParams();

	WindowImage* myWindowImage2 = new WindowImage(myActiveWindowImage->myImage, myActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, myActiveWindowImage->imageN);
	myMdiArea->addSubWindow(myWindowImage2);
	QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
	for (int n=0; n<myListWindows.size(); ++n) {
		WindowImage* myWindowImageCopy = qobject_cast<WindowImage*>(myListWindows.at(n)->widget());
		if (myWindowImageCopy==myWindowImage2) {
			++myWindowImageCopy->imageN;
			myActiveWindow=myListWindows.at(n);
			myActiveWindowImage=myWindowImageCopy;
			siftApplyParams();
		}
	}

	WindowImage* myWindowImage3 = new WindowImage(myActiveWindowImage->myImage, myActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, myActiveWindowImage->imageN);
	myMdiArea->addSubWindow(myWindowImage3);
	myListWindows = myMdiArea->subWindowList();
	for (int n=0; n<myListWindows.size(); ++n) {
		WindowImage* myWindowImageCopy = qobject_cast<WindowImage*>(myListWindows.at(n)->widget());
		if (myWindowImageCopy==myWindowImage3) {
			++myWindowImageCopy->imageN;
			myActiveWindow=myListWindows.at(n);
			myActiveWindowImage=myWindowImageCopy;
			fastApplyParams();
		}
	}

	WindowImage* myWindowImage4 = new WindowImage(myActiveWindowImage->myImage, myActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, myActiveWindowImage->imageN);
	myMdiArea->addSubWindow(myWindowImage4);
	myListWindows = myMdiArea->subWindowList();
	for (int n=0; n<myListWindows.size(); ++n) {
		WindowImage* myWindowImageCopy = qobject_cast<WindowImage*>(myListWindows.at(n)->widget());
		if (myWindowImageCopy==myWindowImage4) {
			++myWindowImageCopy->imageN;
			myActiveWindow=myListWindows.at(n);
			myActiveWindowImage=myWindowImageCopy;
			harrisApplyParams();
		}
	}

	myWindowImage2->show();
	myWindowImage3->show();
	myWindowImage4->show();
	myMdiArea->tileSubWindows();
	for (int n=0; n<myListWindows.size(); ++n) qobject_cast<WindowImage*>(myListWindows.at(n)->widget())->zoomBestFit();
	statusbar->clearMessage();
	myStatusBarLabelZoom->setText(myActiveWindowImage->myImageZoom);
}




void WindowMain::fastRT() {
	WindowFastRealTime* myWindowFastRealTime = new WindowFastRealTime(this);
}




void WindowMain::tile() {
	myMdiArea->tileSubWindows();
	if (mySettings->value("bestFit").toBool()) {
		QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
		for (int n=0; n<myListWindows.size(); ++n) qobject_cast<WindowImage*>(myListWindows.at(n)->widget())->zoomBestFit();
	}
}




void WindowMain::cascade() {
	myMdiArea->cascadeSubWindows();
	if (mySettings->value("bestFit").toBool()) {
		QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
		for (int n=0; n<myListWindows.size(); ++n) qobject_cast<WindowImage*>(myListWindows.at(n)->widget())->zoomBestFit();
	}
}




void WindowMain::duplicate() {
	WindowImage* myWindowImage = new WindowImage(myActiveWindowImage->myImage, myActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, myActiveWindowImage->imageN);
	myMdiArea->addSubWindow(myWindowImage);

	QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
	for (int n=0; n<myListWindows.size(); ++n) {
		WindowImage* myWindowImageC = qobject_cast<WindowImage*>(myListWindows.at(n)->widget());
		if (myWindowImageC->windowImageTitle==myWindowImage->windowImageTitle) ++myWindowImageC->imageN;
	}

	myWindowImage->show();
}




void WindowMain::closeActiveSubWindow() {
	myMdiArea->closeActiveSubWindow();
}




void WindowMain::closeAllSubWindows() {
	myMdiArea->closeAllSubWindows();
}




void WindowMain::officialWebsite() {
	QDesktopServices::openUrl(QUrl::fromEncoded("http://code.google.com/p/image-feature-detector/"));
}




void WindowMain::about() {
	new WindowAbout(this);
}




void WindowMain::mouseDoubleClickEvent(QMouseEvent* event) {
	qDebug() << "mouseDoubleClickEvent";
// 	if(qobject_cast<windowImage*>(myMdiArea->activeSubWindow())->childrenRect().contains(event->globalPos())) {
// 	qDebug() << qobject_cast<windowImage*>(myMdiArea->activeSubWindow())->childrenRect();
// 		if (event->button()==Qt::LeftButton) zoom();
// 		else zoom();
// 	}
}




void WindowMain::updateMenus(QMdiSubWindow* _mdiSubWindow) {
	myMenuWindow->clear();
	myMenuWindow->addAction(actionTile);
	myMenuWindow->addAction(actionCascade);
	myMenuWindow->addSeparator();
	myMenuWindow->addAction(actionNext);
	myMenuWindow->addAction(actionPrevious);
	myMenuWindow->addSeparator();
	myMenuWindow->addAction(actionDuplicate);
	myMenuWindow->addAction(actionClose);
	myMenuWindow->addAction(actionCloseAll);
	if (_mdiSubWindow!=0) {
		myActiveWindow=_mdiSubWindow;
		myActiveWindowImage = qobject_cast<WindowImage*>(_mdiSubWindow->widget());
		actionSaveCopyAs->setEnabled(true);
		actionCopy->setEnabled(true);
		myActionGroupZoom->setEnabled(true);
		myActionGroupFeatures->setEnabled(true);
		myActionGroupWindow->setEnabled(true);

		actionZoomIn->setEnabled(myActiveWindowImage->currentFactor < 3.0);
		actionZoomOut->setEnabled(myActiveWindowImage->currentFactor > 0.25);
		myStatusBarLabelZoom->setText(myActiveWindowImage->myImageZoom);
		if (!myActiveWindowImage->myImageTime.isEmpty()) {
			actionResetImage->setEnabled(true);
			switch (myActiveWindowImage->feature) {
			  case WindowImage::harrisType: myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/Harris16.png"))); break;
			  case WindowImage::fastType: myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/FAST16.png"))); break;
			  case WindowImage::siftType: myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/SIFT16.png"))); break;
			  case WindowImage::surfType: myStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons16/SURF16.png"))); break;
			}
			myStatusBarLabelIcon->setVisible(true);
			myStatusBarLabelTime->setText(myActiveWindowImage->myImageTime);
			myStatusBarLabelKeypoints->setText(myActiveWindowImage->myImageKeypoints);
			myStatusBarLine2->setVisible(true);
			myStatusBarLine3->setVisible(true);
		} else {
			actionResetImage->setEnabled(false);
			myStatusBarLabelIcon->setVisible(false);
			myStatusBarLabelTime->clear();
			myStatusBarLabelKeypoints->clear();
			myStatusBarLine2->setVisible(false);
			myStatusBarLine3->setVisible(false);
		}
		myStatusBarLabelDimensions->setText(myActiveWindowImage->myImageDimensions);
		myStatusBarLabelSize->setText(myActiveWindowImage->myImageSize);
		myStatusBarLine->setVisible(true);

		myMenuWindow->addSeparator();
		QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
		for (int n=0; n<myListWindows.size(); ++n) {
			WindowImage* myWindowImage = qobject_cast<WindowImage*>(myListWindows.at(n)->widget());
			QString myText;
			if (n<9) myText=tr("&%1 %2").arg(n + 1).arg(myWindowImage->windowTitle());
			else myText = tr("%1 %2").arg(n + 1).arg(myWindowImage->windowTitle());
			QAction* myActionSubwindow = myMenuWindow->addAction(myText);
			myActionSubwindow->setCheckable(true);
// 			if (QMdiSubWindow* activeSubWindow = myMdiArea->activeSubWindow())
			if (myMdiArea->activeSubWindow())
				myActionSubwindow->setChecked(myWindowImage == myActiveWindowImage);
			else myActionSubwindow->setChecked(false);
			myActionGroupWindow->addAction(myActionSubwindow);
			connect(myActionSubwindow, SIGNAL(triggered()), mySignalMapper, SLOT(map()));
			mySignalMapper->setMapping(myActionSubwindow, myListWindows.at(n));
		}
	} else if (myMdiArea->subWindowList().size()==0) {
		actionSaveCopyAs->setEnabled(false);
		actionCopy->setEnabled(false);
		myActionGroupZoom->setEnabled(false);
		myActionGroupFeatures->setEnabled(false);
		myActionGroupWindow->setEnabled(false);
		actionResetImage->setEnabled(false);

		toolBarParameters->setVisible(false);
		actionSURF->setChecked(false);
		actionSIFT->setChecked(false);
		actionHarris->setChecked(false);
		actionFAST->setChecked(false);

		myStatusBarLabelTime->clear();
		myStatusBarLabelIcon->setVisible(false);
		myStatusBarLabelKeypoints->clear();
		myStatusBarLabelZoom->clear();
		myStatusBarLabelDimensions->clear();
		myStatusBarLabelSize->clear();
		myStatusBarLine->setVisible(false);
		myStatusBarLine2->setVisible(false);
		myStatusBarLine3->setVisible(false);
	}
}




void WindowMain::updateRecentFilesMenu() {
	QStringList files = mySettings->value("recentFiles").toStringList();
	int numRecentFiles;
	if (files.size() < maxRecentFiles) numRecentFiles = files.size();
	else numRecentFiles = maxRecentFiles;
	separatorRecentFiles->setVisible(numRecentFiles>0);
	menuRecentFiles->clear();
	for (int n=0; n<numRecentFiles; ++n) {
		actionRecentFiles[n]->setText(tr("&%1 %2").arg(n+1).arg(QFileInfo(files[n]).fileName()));
		actionRecentFiles[n]->setData(files[n]);
		actionRecentFiles[n]->setVisible(true);
		menuRecentFiles->addAction(actionRecentFiles[n]);
	}
	for (int n=numRecentFiles; n<maxRecentFiles; ++n) actionRecentFiles[n]->setVisible(false);
}




QSettings* WindowMain::getSettings() {
	return mySettings;
}




void WindowMain::openRecentFile() {
	loadFile(qobject_cast<QAction*>(sender())->data().toString());
}




void WindowMain::setActiveSubWindow(QWidget* subWindow) {
	myMdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(subWindow));
}




void WindowMain::closeEvent(QCloseEvent* eventConstr) {
	saveSettings();
	QWidget::closeEvent(eventConstr);
}




void WindowMain::saveSettings() {
	mySettings->setValue("maximized", isMaximized());
	if (!isMaximized()) {
		mySettings->setValue("pos", pos());
		mySettings->setValue("size", size());
	}
	mySettings->setValue("toolBarFile", toolBarFile->isVisible());
	mySettings->setValue("toolBarZoom", toolBarZoom->isVisible());
	mySettings->setValue("toolBarFeatures", toolBarFeatures->isVisible());
}




void WindowMain::loadFile(QString filePath) {
	if (!filePath.isEmpty()) {
		QImage* myImage = new QImage(filePath);
		if (myImage->isNull()) {
			QMessageBox::warning(this, tr("Image Feature Detector"), tr("Cannot open %1.").arg(filePath));
		} else {
			setRecentFile(filePath);
			WindowImage* myWindowImage = new WindowImage(myImage, filePath);
			myMdiArea->addSubWindow(myWindowImage);
			myWindowImage->show();
		}
	}
}




void WindowMain::setRecentFile(QString filePath) {
	if (mySettings->value("rememberRecentFiles", true).toBool()) {
		QStringList files = mySettings->value("recentFiles").toStringList();
		files.removeAll(filePath);
		files.prepend(filePath);
		while (files.size()>maxRecentFiles) files.removeLast();
		mySettings->setValue("recentFiles", files);
		updateRecentFilesMenu();
	}
}