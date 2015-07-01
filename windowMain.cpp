#include "windowMain.h"

WindowMain::WindowMain() {
	setupUi(this);
	
	mSubwindowActions = new QList<QAction*>();
	mSeparatorOpenWindowsAdded = false;

	QApplication::setWindowIcon(QIcon("IFD128.png"));
	mIconHarris = new QIcon("icons/Harris48.png");
	mIconFAST = new QIcon("icons/FAST48.png");
	mIconSIFT = new QIcon("icons/SIFT48.png");
	mIconSURF = new QIcon("icons/SURF48.png");
	
	setContextMenuPolicy(Qt::PreventContextMenu);
	
	mSettings = new QSettings("imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	resize(mSettings->value("size", QSize(700, 480)).toSize());
	move(mSettings->value("pos", QPoint(150, 40)).toPoint());
	toolBarFile->setVisible(mSettings->value("toolBarFile", true).toBool());
	toolBarZoom->setVisible(mSettings->value("toolBarZoom", true).toBool());
	toolBarFeatures->setVisible(mSettings->value("toolBarFeatures", true).toBool());

	menuRecentFiles = new QMenu(this);
	for (int n=0; n<maxRecentFiles; ++n) {
		mActionRecentFiles[n] = new QAction(this);
		mActionRecentFiles[n]->setVisible(false);
		connect(mActionRecentFiles[n], &QAction::triggered, this, &WindowMain::openRecentFile);
		menuFile->addAction(mActionRecentFiles[n]);
	}
	mActionSeparatorRecentFiles = menuFile->addSeparator();
	mActionSeparatorRecentFiles->setVisible(false);
	updateRecentFilesMenu();
	mActionExit = new QAction(this);
	mActionExit->setObjectName(QString::fromUtf8("actionExit"));
	mActionExit->setText(QApplication::translate("mainWindow", "Exit", 0));
	mActionExit->setShortcut(QApplication::translate("mainWindow", "Ctrl+Q", 0));
	menuFile->addAction(mActionExit);

	toolButtonOpenRecent = new QToolButton(this);
	toolButtonOpenRecent->setFocusPolicy(Qt::NoFocus);
	toolButtonOpenRecent->setPopupMode(QToolButton::MenuButtonPopup);
	toolButtonOpenRecent->setMenu(menuRecentFiles);
	toolButtonOpenRecent->setToolButtonStyle(Qt::ToolButtonIconOnly);
	toolButtonOpenRecent->setAutoRaise(true);
	toolButtonOpenRecent->setDefaultAction(actionOpen);
	toolBarFile->insertWidget(actionCaptureWebcam, toolButtonOpenRecent);

	mActionGroupZoom = new QActionGroup(this);
	mActionGroupZoom->setEnabled(false);
	mActionGroupZoom->addAction(actionZoomIn);
	mActionGroupZoom->addAction(actionZoomOut);
	mActionGroupZoom->addAction(actionZoomOriginal);
	mActionGroupZoom->addAction(actionZoomBestFit);
	
	mActionGroupFeatures = new QActionGroup(this);
	mActionGroupFeatures->setEnabled(false);
	mActionGroupFeatures->addAction(actionSIFT);
	mActionGroupFeatures->addAction(actionSURF);
	mActionGroupFeatures->addAction(actionHarris);
	mActionGroupFeatures->addAction(actionFAST);
	mActionGroupFeatures->addAction(actionDo4);

	mActionGroupWindow = new QActionGroup(this);
	mActionGroupWindow->setEnabled(false);
	mActionGroupWindow->addAction(actionTile);
	mActionGroupWindow->addAction(actionCascade);
	mActionGroupWindow->addAction(actionNext);
	mActionGroupWindow->addAction(actionPrevious);
	mActionGroupWindow->addAction(actionDuplicate);
	mActionGroupWindow->addAction(actionClose);
	mActionGroupWindow->addAction(actionCloseAll);

	mStatusBarLabelZoom = new QLabel(myStatusBar);
	mStatusBarLabelZoom->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelZoom->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelDimensions = new QLabel(myStatusBar);
	mStatusBarLabelDimensions->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelDimensions->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelSize = new QLabel(myStatusBar);
	mStatusBarLabelSize->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelSize->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelTime = new QLabel(myStatusBar);
	mStatusBarLabelTime->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelTime->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelIcon = new QLabel(myStatusBar);
	mStatusBarLabelIcon->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	mStatusBarLabelIcon->setMinimumSize(QSize(16, 16));
	mStatusBarLabelIcon->setMaximumSize(QSize(16, 16));
	mStatusBarLabelIcon->setScaledContents(true);
	mStatusBarLabelIcon->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelIcon->setVisible(false);
	mStatusBarLabelKeypoints = new QLabel(myStatusBar);
	mStatusBarLabelKeypoints->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelSpaceRight = new QLabel(myStatusBar);
	mStatusBarLabelSpaceRight->setMinimumSize(0, 1);
	mStatusBarLabelSpaceLeft = new QLabel(myStatusBar);
	mStatusBarLabelSpaceLeft->setMinimumSize(0, 1);
	mStatusBarLine = new QFrame(myStatusBar);
	mStatusBarLine->setVisible(false);
	mStatusBarLine->setFrameShape(QFrame::VLine);
	mStatusBarLine->setFrameShadow(QFrame::Sunken);
	mStatusBarLine2 = new QFrame(mStatusBarLabelTime);
	mStatusBarLine2->setFrameShape(QFrame::VLine);
	mStatusBarLine2->setFrameShadow(QFrame::Sunken);
	mStatusBarLine2->setVisible(false);
	mStatusBarLine3 = new QFrame(mStatusBarLabelTime);
	mStatusBarLine3->setFrameShape(QFrame::VLine);
	mStatusBarLine3->setFrameShadow(QFrame::Sunken);
	mStatusBarLine3->setVisible(false);
	myStatusBar->addWidget(mStatusBarLabelSpaceLeft);
	myStatusBar->addWidget(mStatusBarLabelZoom);
	myStatusBar->addPermanentWidget(mStatusBarLabelTime);
	myStatusBar->addPermanentWidget(mStatusBarLine2);
	myStatusBar->addPermanentWidget(mStatusBarLabelIcon);
	myStatusBar->addPermanentWidget(mStatusBarLabelKeypoints);
	myStatusBar->addPermanentWidget(mStatusBarLine3);
	myStatusBar->addPermanentWidget(mStatusBarLabelDimensions);
	myStatusBar->addPermanentWidget(mStatusBarLine);
	myStatusBar->addPermanentWidget(mStatusBarLabelSize);
	myStatusBar->addPermanentWidget(mStatusBarLabelSpaceRight);
	
	mHarrisToolBar = new QWidget();
	mHarrisToolBar->setVisible(false);
// 	mHarrisToolBar->setObjectName(QString::fromUtf8("Harris Features"));
	mUIHarris.setupUi(mHarrisToolBar);
	mUIHarris.comboBoxSobelApertureSize->setCurrentIndex(mSettings->value("harris/sobelApertureSize", 2).toInt());
	mUIHarris.spinBoxHarrisApertureSize->setValue(mSettings->value("harris/harrisApertureSize", 1).toInt());
	mUIHarris.doubleSpinBoxKValue->setValue(mSettings->value("harris/kValue", 0.01).toDouble());
	// http://stackoverflow.com/questions/16794695/qt5-overloaded-signals-and-slots
	connect(mUIHarris.comboBoxSobelApertureSize, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WindowMain::saveHarrisParams);
	connect(mUIHarris.spinBoxHarrisApertureSize, &QSpinBox::editingFinished, this, &WindowMain::saveHarrisParams);
	connect(mUIHarris.doubleSpinBoxKValue, &QSpinBox::editingFinished, this, &WindowMain::saveHarrisParams);
	connect(mUIHarris.buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applyHarris);
	connect(mUIHarris.buttonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::resetHarrisParams);
	mHarrisAction = parametersToolBar->addWidget(mHarrisToolBar);
	
	mFastToolBar = new QWidget();
	mFastToolBar->setVisible(false);
	mUIFast.setupUi(mFastToolBar);
	mUIFast.spinBoxThreshold->setValue(mSettings->value("fast/threshold", 50).toInt());
	mUIFast.pushButtonNonMax->setChecked(mSettings->value("fast/nonMaxSuppression", true).toBool());
	connect(mUIFast.spinBoxThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveFastParams);
	connect(mUIFast.pushButtonNonMax, &QPushButton::toggled, this, &WindowMain::saveFastParams);
	connect(mUIFast.buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applyFast);
	connect(mUIFast.buttonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::restFastParams);
	mFastAction = parametersToolBar->addWidget(mFastToolBar);

	mSiftToolBar = new QWidget();
	mSiftToolBar->setVisible(false);
	mUISift.setupUi(mSiftToolBar);
	mUISift.doubleSpinBoxThreshold->setValue(mSettings->value("sift/threshold", 0.014).toDouble());
	mUISift.doubleSpinBoxEdgeThreshold->setValue(mSettings->value("sift/edgeThreshold", 10.0).toDouble());
	mUISift.spinBoxOctaves->setValue(mSettings->value("sift/octaves", 3).toInt());
	mUISift.spinBoxLayers->setValue(mSettings->value("sift/layers", 1).toInt());
	mUISift.pushButtonShowOrientation->setChecked(mSettings->value("sift/showOrientation", true).toBool());
	connect(mUISift.doubleSpinBoxThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.doubleSpinBoxEdgeThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.spinBoxOctaves, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.spinBoxLayers, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.pushButtonShowOrientation, &QPushButton::toggled, this, &WindowMain::saveSiftParams);
	connect(mUISift.buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applySift);
	connect(mUISift.buttonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::resetSiftParams);
	mSiftAction = parametersToolBar->addWidget(mSiftToolBar);
	
	mSurfToolBar = new QWidget();
	mSurfToolBar->setVisible(false);
	mUISurf.setupUi(mSurfToolBar);
	mUISurf.spinBoxThreshold->setValue(mSettings->value("surf/threshold", 4000).toInt());
	mUISurf.spinBoxOctaves->setValue(mSettings->value("surf/octaves", 3).toInt());
	mUISurf.spinBoxLayers->setValue(mSettings->value("surf/layers", 1).toInt());
	mUISurf.pushButtonShowOrientation->setChecked(mSettings->value("surf/showOrientation", true).toBool());
	connect(mUISurf.spinBoxThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveSurfParams);
	connect(mUISurf.spinBoxOctaves, &QSpinBox::editingFinished, this, &WindowMain::saveSurfParams);
	connect(mUISurf.spinBoxLayers, &QSpinBox::editingFinished, this, &WindowMain::saveSurfParams);
	connect(mUISurf.pushButtonShowOrientation, &QPushButton::toggled, this, &WindowMain::saveSurfParams);
	connect(mUISurf.buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applySurf);
	connect(mUISurf.buttonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::resetSurfParams);
	mSurfAction = parametersToolBar->addWidget(mSurfToolBar);
	
	switch (mSettings->value("startupParameters", 0).toInt()) {
		case 0: showHarrisToolBar(); break;
		case 1: showFastToolBar(); break;
		case 2: showSiftToolBar(); break;
		case 3: showSurfToolBar(); break;
	}
	
	connect(actionOpen, &QAction::triggered, this, &WindowMain::open);
	connect(actionCaptureWebcam, &QAction::triggered, this, &WindowMain::captureWebcam);
	connect(actionSaveCopyAs, &QAction::triggered, this, &WindowMain::saveCopyAs);
	connect(mActionExit, &QAction::triggered, this, &WindowMain::exit);
	connect(actionCopy, &QAction::triggered, this, &WindowMain::copy);
	connect(actionResetImage, &QAction::triggered, this, &WindowMain::resetImage);
	connect(actionPreferences, &QAction::triggered, this, &WindowMain::preferences);
	connect(actionStartupDialog, &QAction::triggered, this, &WindowMain::startupDialog);
	connect(actionZoomIn, &QAction::triggered, this, &WindowMain::zoom);
	connect(actionZoomOut, &QAction::triggered, this, &WindowMain::zoom);
	connect(actionZoomOriginal, &QAction::triggered, this, &WindowMain::zoom);
	connect(actionZoomBestFit, &QAction::triggered, this, &WindowMain::zoom);
	connect(actionHarris, &QAction::triggered, this, &WindowMain::showHarrisToolBar);
	connect(actionFAST, &QAction::triggered, this, &WindowMain::showFastToolBar);
	connect(actionSIFT, &QAction::triggered, this, &WindowMain::showSiftToolBar);
	connect(actionSURF, &QAction::triggered, this, &WindowMain::showSurfToolBar);
	connect(actionDo4, &QAction::triggered, this, &WindowMain::do4);
	connect(actionFastRT, &QAction::triggered, this, &WindowMain::openFastRT);
	connect(actionTile, &QAction::triggered, this, &WindowMain::tile);
	connect(actionCascade, &QAction::triggered, this, &WindowMain::cascade);
	connect(actionNext, &QAction::triggered, myMdiArea, &QMdiArea::activateNextSubWindow);
	connect(actionPrevious, &QAction::triggered, myMdiArea, &QMdiArea::activatePreviousSubWindow);
	connect(actionDuplicate, &QAction::triggered, this, &WindowMain::duplicate);
	connect(actionClose, &QAction::triggered, this, &WindowMain::closeActiveSubWindow);
	connect(actionCloseAll, &QAction::triggered, this, &WindowMain::closeAllSubWindows);
	connect(actionWebsite, &QAction::triggered, this, &WindowMain::website);
	connect(actionAbout, &QAction::triggered, this, &WindowMain::about);
	connect(myMdiArea, &QMdiArea::subWindowActivated, this, &WindowMain::updateWindowMenu);
	mSignalMapper = new QSignalMapper(this); // for the Open Windows menu entries
	connect(mSignalMapper, static_cast<void (QSignalMapper::*)(QWidget*)>(&QSignalMapper::mapped), this, &WindowMain::setActiveSubWindow);

	
	if (mSettings->value("maximized", true).toBool())
		showMaximized();
	else show();
	
	if (mSettings->value("startupDialog", true).toBool())
		startupDialog();
}




void WindowMain::open() {
	loadFile(QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.bmp *.jpg)")));
}




void WindowMain::captureWebcam() {
	new WindowCaptureWebcam(this);
}




void WindowMain::saveCopyAs() {
	QString myFileName = QFileDialog::getSaveFileName(0, tr("Save Copy As"), QFileInfo(mActiveWindowImage->windowImageTitle).baseName().append(".png"), tr("Images (*.bmp *.png)"));
	if (!myFileName.isEmpty()) mActiveWindowImage->mPixmap.save(myFileName);
}




void WindowMain::exit() {
	saveSettings();
	close();
}




void WindowMain::copy() {
	QApplication::clipboard()->setPixmap(mActiveWindowImage->mPixmap);
}




void WindowMain::preferences() {
	new WindowPreferences(this);
}




void WindowMain::startupDialog() {
	new WindowStartup(this);
}




void WindowMain::zoom() {
	if (sender()==actionZoomIn)
		mActiveWindowImage->zoomIn();
	else if (sender()==actionZoomOut)
		mActiveWindowImage->zoomOut();
	else if (sender()==actionZoomOriginal)
		mActiveWindowImage->zoomOriginal();
	else if (sender()==actionZoomBestFit)
		mActiveWindowImage->zoomBestFit();
	
	actionZoomIn->setEnabled(mActiveWindowImage->currentFactor < 3.0);
	actionZoomOut->setEnabled(mActiveWindowImage->currentFactor > 0.25);
	mStatusBarLabelZoom->setText(mActiveWindowImage->mImageZoom);
}




void WindowMain::showHarrisToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mHarrisAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 0);
	actionHarris->setChecked(true);
}




void WindowMain::resetHarrisParams() {
	mUIHarris.comboBoxSobelApertureSize->setCurrentIndex(1);
	mUIHarris.spinBoxHarrisApertureSize->setValue(2);
	mUIHarris.doubleSpinBoxKValue->setValue(0.01);
	saveHarrisParams();
}




void WindowMain::applyHarris() {
	int sobelApertureSize;
	switch (mSettings->value("harris/sobelApertureSize", 1).toInt()) {
		case 0: sobelApertureSize=1; break;
		case 1: sobelApertureSize=3; break;
		case 2: sobelApertureSize=5; break;
		case 3: sobelApertureSize=7;
	}
	mActiveWindowImage->applyHarris(sobelApertureSize,
			mSettings->value("harris/harrisApertureSize", 2).toInt(),
			mSettings->value("harris/kValue", 0.01).toDouble());
	actionResetImage->setEnabled(true);
	mActiveWindowImage->feature=WindowImage::harrisType;
	mActiveWindow->setWindowIcon(*mIconHarris);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime);
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints);
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/Harris48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveHarrisParams() {
	mSettings->setValue("harris/sobelApertureSize", mUIHarris.comboBoxSobelApertureSize->currentIndex());
	mSettings->setValue("harris/harrisApertureSize", mUIHarris.spinBoxHarrisApertureSize->value());
	mSettings->setValue("harris/kValue", mUIHarris.doubleSpinBoxKValue->value());
}




void WindowMain::showFastToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mFastAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 1);
	actionFAST->setChecked(true);
}




void WindowMain::restFastParams() {
	mUIFast.spinBoxThreshold->setValue(50);
	mUIFast.pushButtonNonMax->setChecked(true);
	saveFastParams();
}




void WindowMain::applyFast() {
	mActiveWindowImage->applyFast(mSettings->value("fast/threshold", 50).toInt(), mSettings->value("fast/nonMaxSuppression", true).toBool());
	actionResetImage->setEnabled(true);
	mActiveWindowImage->feature=WindowImage::fastType;
	mActiveWindow->setWindowIcon(*mIconFAST);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime);
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints);
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/FAST48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveFastParams() {
	mSettings->setValue("fast/threshold", mUIFast.spinBoxThreshold->value());
	mSettings->setValue("fast/nonMaxSuppression", mUIFast.pushButtonNonMax->isChecked());
}




void WindowMain::showSiftToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mSiftAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 2);
	actionSIFT->setChecked(true);
}




void WindowMain::resetSiftParams() {
	mUISift.doubleSpinBoxThreshold->setValue(0.014);
	mUISift.doubleSpinBoxEdgeThreshold->setValue(10.0);
	mUISift.spinBoxOctaves->setValue(3);
	mUISift.spinBoxLayers->setValue(1);
	mUISift.pushButtonShowOrientation->setChecked(true);
	saveSiftParams();
}




void WindowMain::applySift() {
	mActiveWindowImage->applySift(mSettings->value("sift/threshold", 0.014).toDouble(),
			mSettings->value("sift/edgeThreshold", 10.0).toDouble(),
			mSettings->value("sift/octaves", 3).toInt(),
			mSettings->value("sift/layers", 1).toInt(),
			mSettings->value("sift/showOrientation", true).toBool());
	actionResetImage->setEnabled(true);
	mActiveWindowImage->feature=WindowImage::siftType;
	mActiveWindow->setWindowIcon(*mIconSIFT);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime);
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints);
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/SIFT48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveSiftParams() {
	mSettings->setValue("sift/threshold", mUISift.doubleSpinBoxThreshold->value());
	mSettings->setValue("sift/edgeThreshold", mUISift.doubleSpinBoxEdgeThreshold->value());
	mSettings->setValue("sift/octaves", mUISift.spinBoxOctaves->value());
	mSettings->setValue("sift/layers", mUISift.spinBoxLayers->value());
	mSettings->setValue("sift/showOrientation", mUISift.pushButtonShowOrientation->isChecked());
}




void WindowMain::showSurfToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mSurfAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 3);
	actionSURF->setChecked(true);
}




void WindowMain::resetSurfParams() {
	mUISurf.spinBoxThreshold->setValue(4000);
	mUISurf.spinBoxOctaves->setValue(3);
	mUISurf.spinBoxLayers->setValue(1);
	mUISurf.pushButtonShowOrientation->setChecked(true);
	saveSurfParams();
}




void WindowMain::applySurf() {
	mActiveWindowImage->applySurf(mSettings->value("surf/threshold", 4000).toInt(),
			mSettings->value("surf/octaves", 3).toInt(),
			mSettings->value("surf/layers", 1).toInt(),
			0,
			mSettings->value("surf/showOrientation", true).toBool());
	actionResetImage->setEnabled(true);
	mActiveWindowImage->feature=WindowImage::surfType;
	mActiveWindow->setWindowIcon(*mIconSURF);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime);
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints);
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/SURF48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveSurfParams() {
	mSettings->setValue("surf/threshold", mUISurf.spinBoxThreshold->value());
	mSettings->setValue("surf/octaves", mUISurf.spinBoxOctaves->value());
	mSettings->setValue("surf/layers", mUISurf.spinBoxLayers->value());
	mSettings->setValue("surf/showOrientation", mUISurf.pushButtonShowOrientation->isChecked());
}




void WindowMain::resetImage() {
	mActiveWindowImage->resetImage();
	actionResetImage->setEnabled(false);
	mActiveWindowImage->feature=WindowImage::none;
	mActiveWindow->setWindowIcon(QApplication::windowIcon());
	mStatusBarLabelIcon->clear();
	mStatusBarLabelIcon->setVisible(false);
	mStatusBarLabelTime->clear();
	mStatusBarLabelKeypoints->clear();
	mStatusBarLine2->setVisible(false);
	mStatusBarLine3->setVisible(false);
}




void WindowMain::do4() {
	myStatusBar->showMessage("Calculating features...");
	applySurf();

	WindowImage* siftImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, mActiveWindowImage->imageN);
	myMdiArea->addSubWindow(siftImage);
	QList<QMdiSubWindow*> windowsList = myMdiArea->subWindowList();
	for (int n=0; n<windowsList.size(); ++n) {
		WindowImage* myWindowImageCopy = qobject_cast<WindowImage*>(windowsList.at(n)->widget());
		if (myWindowImageCopy==siftImage) {
			++myWindowImageCopy->imageN;
			mActiveWindow=windowsList.at(n);
			mActiveWindowImage=myWindowImageCopy;
			applySift();
		}
	}

	WindowImage* fastImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, mActiveWindowImage->imageN);
	myMdiArea->addSubWindow(fastImage);
	windowsList = myMdiArea->subWindowList();
	for (int n=0; n<windowsList.size(); ++n) {
		WindowImage* myWindowImageCopy = qobject_cast<WindowImage*>(windowsList.at(n)->widget());
		if (myWindowImageCopy==fastImage) {
			++myWindowImageCopy->imageN;
			mActiveWindow=windowsList.at(n);
			mActiveWindowImage=myWindowImageCopy;
			applyFast();
		}
	}

	WindowImage* harrisImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, mActiveWindowImage->imageN);
	myMdiArea->addSubWindow(harrisImage);
	windowsList = myMdiArea->subWindowList();
	for (int n=0; n<windowsList.size(); ++n) {
		WindowImage* myWindowImageCopy = qobject_cast<WindowImage*>(windowsList.at(n)->widget());
		if (myWindowImageCopy==harrisImage) {
			++myWindowImageCopy->imageN;
			mActiveWindow=windowsList.at(n);
			mActiveWindowImage=myWindowImageCopy;
			applyHarris();
		}
	}

	siftImage->show();
	fastImage->show();
	harrisImage->show();
	myMdiArea->tileSubWindows();
	for (int n=0; n<windowsList.size(); ++n)
		qobject_cast<WindowImage*>(windowsList.at(n)->widget())->zoomBestFit();
	myStatusBar->clearMessage();
	mStatusBarLabelZoom->setText(mActiveWindowImage->mImageZoom);
}




void WindowMain::openFastRT() {
	new WindowFastRealTime(this);
}




void WindowMain::tile() {
	myMdiArea->tileSubWindows();
	if (mSettings->value("bestFit").toBool()) {
		QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
		for (int n=0; n<myListWindows.size(); ++n)
			qobject_cast<WindowImage*>(myListWindows.at(n)->widget())->zoomBestFit();
	}
}




void WindowMain::cascade() {
	myMdiArea->cascadeSubWindows();
	if (mSettings->value("bestFit").toBool()) {
		QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
		for (int n=0; n<myListWindows.size(); ++n)
			qobject_cast<WindowImage*>(myListWindows.at(n)->widget())->zoomBestFit();
	}
}




void WindowMain::duplicate() {
	WindowImage* windowImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->windowImageTitle,
		WindowImage::duplicated, mActiveWindowImage->imageN);
	myMdiArea->addSubWindow(windowImage);

	QList<QMdiSubWindow*> myListWindows = myMdiArea->subWindowList();
	for (int n=0; n<myListWindows.size(); ++n) {
		WindowImage* windowImageC = qobject_cast<WindowImage*>(myListWindows.at(n)->widget());
		if (windowImageC->windowImageTitle==windowImage->windowImageTitle)
			++windowImageC->imageN;
	}

	windowImage->show();
}




void WindowMain::closeActiveSubWindow() {
	myMdiArea->closeActiveSubWindow();
}




void WindowMain::closeAllSubWindows() {
	myMdiArea->closeAllSubWindows();
}




void WindowMain::website() {
	QDesktopServices::openUrl(QUrl::fromEncoded("https://github.com/AntonioRedondo/ImageFeatureDetector"));
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




// http://doc.qt.io/qt-5/qtwidgets-mainwindows-mdi-example.html
void WindowMain::updateWindowMenu(QMdiSubWindow* mdiSubWindow) {
	if (!mSeparatorOpenWindowsAdded) { // Adding the separator on Qt Designer doesn't work
		menuWindow->addSeparator();
		mSeparatorOpenWindowsAdded = true;
	}
	for (int n=0; n<mSubwindowActions->size(); ++n) {
// 		menuWindow->removeAction(mSubwindowActions->at(n)); // Makes not to trigger new actions added
		mSubwindowActions->at(n)->setVisible(false);
	}
	mSubwindowActions->clear();
	
	if (mdiSubWindow != 0) {
		mActiveWindow = mdiSubWindow;
		mActiveWindowImage = qobject_cast<WindowImage*> (mdiSubWindow->widget());
		
		actionSaveCopyAs->setEnabled(true);
		actionCopy->setEnabled(true);
		mActionGroupZoom->setEnabled(true);
		mActionGroupFeatures->setEnabled(true);
		mActionGroupWindow->setEnabled(true);
		actionZoomIn->setEnabled(mActiveWindowImage->currentFactor < 3.0);
		actionZoomOut->setEnabled(mActiveWindowImage->currentFactor > 0.25);
		
		if (!mActiveWindowImage->mImageTime.isEmpty()) {
			actionResetImage->setEnabled(true);
			switch (mActiveWindowImage->feature) {
				case WindowImage::harrisType: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/Harris48.png"))); break;
				case WindowImage::fastType: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/FAST48.png"))); break;
				case WindowImage::siftType: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/SIFT48.png"))); break;
				case WindowImage::surfType: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/SURF48.png"))); break;
			}
			mStatusBarLabelIcon->setVisible(true);
			mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime);
			mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints);
			mStatusBarLine2->setVisible(true);
			mStatusBarLine3->setVisible(true);
		} else {
			actionResetImage->setEnabled(false);
			mStatusBarLabelIcon->setVisible(false);
			mStatusBarLabelTime->clear();
			mStatusBarLabelKeypoints->clear();
			mStatusBarLine2->setVisible(false);
			mStatusBarLine3->setVisible(false);
		}
		
		mStatusBarLabelZoom->setText(mActiveWindowImage->mImageZoom);
		mStatusBarLabelDimensions->setText(mActiveWindowImage->mImageDimensions);
		mStatusBarLabelSize->setText(mActiveWindowImage->mImageSize);
		mStatusBarLine->setVisible(true);
		
		QList<QMdiSubWindow*> subwindows = myMdiArea->subWindowList();
		for (int n=0; n<subwindows.size(); ++n) {
			WindowImage* windowImage = qobject_cast<WindowImage*> (subwindows.at(n)->widget());
			QString actionName;
			if (n<9)
				actionName = tr("&%1 %2").arg(n+1).arg(windowImage->windowTitle());
			else actionName = tr("%1 %2").arg(n+1).arg(windowImage->windowTitle());
			QAction* actionSubwindow = menuWindow->addAction(actionName);
			mSubwindowActions->append(actionSubwindow);
			actionSubwindow->setCheckable(true);
			if (myMdiArea->activeSubWindow())
				actionSubwindow->setChecked(windowImage == mActiveWindowImage);
			else actionSubwindow->setChecked(false);
			mActionGroupWindow->addAction(actionSubwindow);
			mSignalMapper->setMapping(actionSubwindow, subwindows.at(n));
			connect(actionSubwindow, &QAction::triggered, mSignalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
		}
	} else if (myMdiArea->subWindowList().size()==0) {
		actionSaveCopyAs->setEnabled(false);
		actionCopy->setEnabled(false);
		mActionGroupZoom->setEnabled(false);
		mActionGroupFeatures->setEnabled(false);
		mActionGroupWindow->setEnabled(false);
		actionResetImage->setEnabled(false);
		parametersToolBar->setEnabled(false);

		mStatusBarLabelTime->clear();
		mStatusBarLabelIcon->setVisible(false);
		mStatusBarLabelKeypoints->clear();
		mStatusBarLabelZoom->clear();
		mStatusBarLabelDimensions->clear();
		mStatusBarLabelSize->clear();
		mStatusBarLine->setVisible(false);
		mStatusBarLine2->setVisible(false);
		mStatusBarLine3->setVisible(false);
	}
}




void WindowMain::loadFile(QString filePath) {
	if (!filePath.isEmpty()) {
		QImage* image = new QImage(filePath);
		if (!image->isNull()) {
			setRecentFile(filePath);
			WindowImage* windowImage = new WindowImage(image, filePath);
			myMdiArea->addSubWindow(windowImage);
			windowImage->parentWidget()->setGeometry(0,0,image->width()+8,image->height()+30); // 8 and 30 are hardcoded values for the decorations of the subwindow
			windowImage->show();
			parametersToolBar->setEnabled(true);
		} else {
			QMessageBox::warning(this, tr("Image Feature Detector"), tr("Cannot open %1.").arg(filePath));
		}
	}
}




void WindowMain::setRecentFile(QString filePath) {
	if (mSettings->value("rememberRecentFiles", true).toBool()) {
		QStringList files = mSettings->value("recentFiles").toStringList();
		files.removeAll(filePath);
		files.prepend(filePath);
		while (files.size()>maxRecentFiles)
			files.removeLast();
		mSettings->setValue("recentFiles", files);
		updateRecentFilesMenu();
	}
}




// http://doc.qt.io/qt-5/qtwidgets-mainwindows-recentfiles-example.html
void WindowMain::updateRecentFilesMenu() {
	QStringList files = mSettings->value("recentFiles").toStringList();
	int numRecentFiles;
	if (files.size() < maxRecentFiles)
		numRecentFiles = files.size();
	else numRecentFiles = maxRecentFiles;
	mActionSeparatorRecentFiles->setVisible(numRecentFiles>0);
	menuRecentFiles->clear();
	for (int n=0; n<numRecentFiles; ++n) {
		mActionRecentFiles[n]->setText(tr("&%1 %2").arg(n+1).arg(QFileInfo(files[n]).fileName()));
		mActionRecentFiles[n]->setData(files[n]);
		mActionRecentFiles[n]->setVisible(true);
		menuRecentFiles->addAction(mActionRecentFiles[n]);
	}
	for (int n=numRecentFiles; n<maxRecentFiles; ++n)
		mActionRecentFiles[n]->setVisible(false);
}




QSettings* WindowMain::getSettings() {
	return mSettings;
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
	mSettings->setValue("maximized", isMaximized());
	if (!isMaximized()) {
		mSettings->setValue("pos", pos());
		mSettings->setValue("size", size());
	}
}