/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#include "windowMain.h"

WindowMain::WindowMain() : mTotalImages(0) {
	setupUi(this);
	
	mSubwindowActions = new QList<QAction*>();
	mSeparatorOpenWindowsAdded = false;
	
	mIconHarris = new QIcon("icons/Harris.png");
	mIconFAST = new QIcon("icons/Fast.png");
	mIconSIFT = new QIcon("icons/Sift.png");
	mIconSURF = new QIcon("icons/Surf.png");
	
	setContextMenuPolicy(Qt::PreventContextMenu);
	
	mSettings = new QSettings("imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	resize(mSettings->value("size", QSize(700, 480)).toSize());
	move(mSettings->value("pos", QPoint(150, 40)).toPoint());
	uiToolBarFile->setVisible(mSettings->value("uiToolBarFile", true).toBool());
	uiToolBarZoom->setVisible(mSettings->value("uiToolBarZoom", true).toBool());
	uiToolBarFeatures->setVisible(mSettings->value("uiToolBarFeatures", true).toBool());

	mMenuRecentFiles = new QMenu(this);
	for (int n=0; n<maxRecentFiles; ++n) {
		mActionRecentFiles[n] = new QAction(this);
		mActionRecentFiles[n]->setVisible(false);
		connect(mActionRecentFiles[n], &QAction::triggered, this, &WindowMain::openRecentFile);
		uiMenuFile->addAction(mActionRecentFiles[n]);
	}
	mActionSeparatorRecentFiles = uiMenuFile->addSeparator();
	mActionSeparatorRecentFiles->setVisible(false);
	updateRecentFilesMenu();
	mActionExit = new QAction(this);
	mActionExit->setObjectName(QString::fromUtf8("actionExit"));
	mActionExit->setText(QApplication::translate("mainWindow", "Exit", 0));
	mActionExit->setShortcut(QApplication::translate("mainWindow", "Ctrl+Q", 0));
	mActionExit->setIcon(QIcon("iconsBreeze/window-close.svg"));
	uiMenuFile->addAction(mActionExit);

	mToolButtonOpenRecent = new QToolButton(this);
	mToolButtonOpenRecent->setFocusPolicy(Qt::NoFocus);
	mToolButtonOpenRecent->setPopupMode(QToolButton::MenuButtonPopup);
	mToolButtonOpenRecent->setMenu(mMenuRecentFiles);
	mToolButtonOpenRecent->setToolButtonStyle(Qt::ToolButtonIconOnly);
	mToolButtonOpenRecent->setAutoRaise(true);
	mToolButtonOpenRecent->setDefaultAction(uiActionOpen);
	uiToolBarFile->insertWidget(uiActionCaptureWebcam, mToolButtonOpenRecent);

	mActionGroupZoom = new QActionGroup(this);
	mActionGroupZoom->setEnabled(false);
	mActionGroupZoom->addAction(uiActionZoomIn);
	mActionGroupZoom->addAction(uiActionZoomOut);
	mActionGroupZoom->addAction(uiActionZoomOriginal);
	mActionGroupZoom->addAction(uiActionZoomBestFit);
	
	mActionGroupFeatures = new QActionGroup(this);
	mActionGroupFeatures->setEnabled(false);
	mActionGroupFeatures->addAction(uiActionSIFT);
	mActionGroupFeatures->addAction(uiActionSURF);
	mActionGroupFeatures->addAction(uiActionHarris);
	mActionGroupFeatures->addAction(uiActionFAST);
	mActionGroupFeatures->addAction(uiActionDo4);

	mActionGroupWindow = new QActionGroup(this);
	mActionGroupWindow->setEnabled(false);
	mActionGroupWindow->addAction(uiActionTile);
	mActionGroupWindow->addAction(uiActionCascade);
	mActionGroupWindow->addAction(uiActionNext);
	mActionGroupWindow->addAction(uiActionPrevious);
	mActionGroupWindow->addAction(uiActionDuplicate);
	mActionGroupWindow->addAction(uiActionClose);
	mActionGroupWindow->addAction(uiActionCloseAll);

	mStatusBarLabelZoom = new QLabel(uiStatusBar);
	mStatusBarLabelZoom->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelZoom->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelDimensions = new QLabel(uiStatusBar);
	mStatusBarLabelDimensions->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelDimensions->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelSize = new QLabel(uiStatusBar);
	mStatusBarLabelSize->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelSize->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelTime = new QLabel(uiStatusBar);
	mStatusBarLabelTime->setFrameShape(QFrame::NoFrame);
	mStatusBarLabelTime->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelIcon = new QLabel(uiStatusBar);
	mStatusBarLabelIcon->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	mStatusBarLabelIcon->setMinimumSize(QSize(16, 16));
	mStatusBarLabelIcon->setMaximumSize(QSize(16, 16));
	mStatusBarLabelIcon->setScaledContents(true);
	mStatusBarLabelIcon->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelIcon->setVisible(false);
	mStatusBarLabelKeypoints = new QLabel(uiStatusBar);
	mStatusBarLabelKeypoints->setAlignment(Qt::AlignHCenter);
	mStatusBarLabelSpaceRight = new QLabel(uiStatusBar);
	mStatusBarLabelSpaceRight->setMinimumSize(0, 1);
	mStatusBarLabelSpaceLeft = new QLabel(uiStatusBar);
	mStatusBarLabelSpaceLeft->setMinimumSize(0, 1);
	mStatusBarLine = new QFrame(uiStatusBar);
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
	uiStatusBar->addWidget(mStatusBarLabelSpaceLeft);
	uiStatusBar->addWidget(mStatusBarLabelZoom);
	uiStatusBar->addPermanentWidget(mStatusBarLabelTime);
	uiStatusBar->addPermanentWidget(mStatusBarLine2);
	uiStatusBar->addPermanentWidget(mStatusBarLabelIcon);
	uiStatusBar->addPermanentWidget(mStatusBarLabelKeypoints);
	uiStatusBar->addPermanentWidget(mStatusBarLine3);
	uiStatusBar->addPermanentWidget(mStatusBarLabelDimensions);
	uiStatusBar->addPermanentWidget(mStatusBarLine);
	uiStatusBar->addPermanentWidget(mStatusBarLabelSize);
	uiStatusBar->addPermanentWidget(mStatusBarLabelSpaceRight);
	
	mHarrisToolBar = new QWidget();
	mHarrisToolBar->setVisible(false);
	mUIHarris.setupUi(mHarrisToolBar);
	mUIHarris.uiComboBoxSobelApertureSize->setCurrentIndex(mSettings->value("harris/sobelApertureSize", 2).toInt());
	mUIHarris.uiSpinBoxHarrisApertureSize->setValue(mSettings->value("harris/harrisApertureSize", 1).toInt());
	mUIHarris.uiDoubleSpinBoxKValue->setValue(mSettings->value("harris/kValue", 0.01).toDouble());
	// http://stackoverflow.com/questions/16794695/qt5-overloaded-signals-and-slots
	connect(mUIHarris.uiComboBoxSobelApertureSize, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WindowMain::saveHarrisParams);
	connect(mUIHarris.uiSpinBoxHarrisApertureSize, &QSpinBox::editingFinished, this, &WindowMain::saveHarrisParams);
	connect(mUIHarris.uiDoubleSpinBoxKValue, &QSpinBox::editingFinished, this, &WindowMain::saveHarrisParams);
	connect(mUIHarris.uiButtonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applyHarris);
	connect(mUIHarris.uiButtonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::resetHarrisParams);
	mHarrisAction = uiToolBarParameters->addWidget(mHarrisToolBar);
	
	mFastToolBar = new QWidget();
	mFastToolBar->setVisible(false);
	mUIFast.setupUi(mFastToolBar);
	mUIFast.uiSpinBoxThreshold->setValue(mSettings->value("fast/threshold", 50).toInt());
	mUIFast.uiPushButtonNonMax->setChecked(mSettings->value("fast/nonMaxSuppression", true).toBool());
	connect(mUIFast.uiSpinBoxThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveFastParams);
	connect(mUIFast.uiPushButtonNonMax, &QPushButton::toggled, this, &WindowMain::saveFastParams);
	connect(mUIFast.uiButtonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applyFast);
	connect(mUIFast.uiButtonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::restFastParams);
	mFastAction = uiToolBarParameters->addWidget(mFastToolBar);

	mSiftToolBar = new QWidget();
	mSiftToolBar->setVisible(false);
	mUISift.setupUi(mSiftToolBar);
	mUISift.uiDoubleSpinBoxThreshold->setValue(mSettings->value("sift/threshold", 0.014).toDouble());
	mUISift.uiDoubleSpinBoxEdgeThreshold->setValue(mSettings->value("sift/edgeThreshold", 10.0).toDouble());
	mUISift.uiSpinBoxOctaves->setValue(mSettings->value("sift/octaves", 3).toInt());
	mUISift.uiSpinBoxLayers->setValue(mSettings->value("sift/layers", 1).toInt());
	mUISift.uiPushButtonShowOrientation->setChecked(mSettings->value("sift/showOrientation", true).toBool());
	connect(mUISift.uiDoubleSpinBoxThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.uiDoubleSpinBoxEdgeThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.uiSpinBoxOctaves, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.uiSpinBoxLayers, &QSpinBox::editingFinished, this, &WindowMain::saveSiftParams);
	connect(mUISift.uiPushButtonShowOrientation, &QPushButton::toggled, this, &WindowMain::saveSiftParams);
	connect(mUISift.uiButtonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applySift);
	connect(mUISift.uiButtonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::resetSiftParams);
	mSiftAction = uiToolBarParameters->addWidget(mSiftToolBar);
	
	mSurfToolBar = new QWidget();
	mSurfToolBar->setVisible(false);
	mUISurf.setupUi(mSurfToolBar);
	mUISurf.uiSpinBoxThreshold->setValue(mSettings->value("surf/threshold", 4000).toInt());
	mUISurf.uiSpinBoxOctaves->setValue(mSettings->value("surf/octaves", 3).toInt());
	mUISurf.uiSpinBoxLayers->setValue(mSettings->value("surf/layers", 1).toInt());
	mUISurf.uiPushButtonShowOrientation->setChecked(mSettings->value("surf/showOrientation", true).toBool());
	connect(mUISurf.uiSpinBoxThreshold, &QSpinBox::editingFinished, this, &WindowMain::saveSurfParams);
	connect(mUISurf.uiSpinBoxOctaves, &QSpinBox::editingFinished, this, &WindowMain::saveSurfParams);
	connect(mUISurf.uiSpinBoxLayers, &QSpinBox::editingFinished, this, &WindowMain::saveSurfParams);
	connect(mUISurf.uiPushButtonShowOrientation, &QPushButton::toggled, this, &WindowMain::saveSurfParams);
	connect(mUISurf.uiButtonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &WindowMain::applySurf);
	connect(mUISurf.uiButtonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &WindowMain::resetSurfParams);
	mSurfAction = uiToolBarParameters->addWidget(mSurfToolBar);
	
	switch (mSettings->value("startupParameters", 0).toInt()) {
		case 0: showHarrisToolBar(); break;
		case 1: showFastToolBar(); break;
		case 2: showSiftToolBar(); break;
		case 3: showSurfToolBar(); break;
	}
	
	connect(uiActionOpen, &QAction::triggered, this, &WindowMain::open);
	connect(uiActionCaptureWebcam, &QAction::triggered, this, &WindowMain::captureWebcam);
	connect(uiActionSaveCopyAs, &QAction::triggered, this, &WindowMain::saveCopyAs);
	connect(uiActionPreferences, &QAction::triggered, this, &WindowMain::preferences);
	connect(mActionExit, &QAction::triggered, this, &WindowMain::exit);
	connect(uiActionCopy, &QAction::triggered, this, &WindowMain::copy);
	connect(uiActionResetImage, &QAction::triggered, this, &WindowMain::resetImage);
	connect(uiActionStartupDialog, &QAction::triggered, this, &WindowMain::startupDialog);
	connect(uiActionZoomIn, &QAction::triggered, this, &WindowMain::zoom);
	connect(uiActionZoomOut, &QAction::triggered, this, &WindowMain::zoom);
	connect(uiActionZoomOriginal, &QAction::triggered, this, &WindowMain::zoom);
	connect(uiActionZoomBestFit, &QAction::triggered, this, &WindowMain::zoom);
	connect(uiActionHarris, &QAction::triggered, this, &WindowMain::showHarrisToolBar);
	connect(uiActionFAST, &QAction::triggered, this, &WindowMain::showFastToolBar);
	connect(uiActionSIFT, &QAction::triggered, this, &WindowMain::showSiftToolBar);
	connect(uiActionSURF, &QAction::triggered, this, &WindowMain::showSurfToolBar);
	connect(uiActionDo4, &QAction::triggered, this, &WindowMain::do4);
	connect(uiActionFastRT, &QAction::triggered, this, &WindowMain::openFastRT);
	connect(uiActionTile, &QAction::triggered, this, &WindowMain::tile);
	connect(uiActionCascade, &QAction::triggered, this, &WindowMain::cascade);
	connect(uiActionNext, &QAction::triggered, uiMdiArea, &QMdiArea::activateNextSubWindow);
	connect(uiActionPrevious, &QAction::triggered, uiMdiArea, &QMdiArea::activatePreviousSubWindow);
	connect(uiActionDuplicate, &QAction::triggered, this, &WindowMain::duplicate);
	connect(uiActionClose, &QAction::triggered, this, &WindowMain::closeActiveSubWindow);
	connect(uiActionCloseAll, &QAction::triggered, this, &WindowMain::closeAllSubWindows);
	connect(uiActionWebsite, &QAction::triggered, this, &WindowMain::website);
	connect(uiActionAbout, &QAction::triggered, this, &WindowMain::about);
	connect(uiMdiArea, &QMdiArea::subWindowActivated, this, &WindowMain::updateWindowMenu);
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
	QString myFileName = QFileDialog::getSaveFileName(0, tr("Save Copy As"), QFileInfo(mActiveWindowImage->mWindowTitle).baseName().append(".png"), tr("Images (*.bmp *.png)"));
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
	if (sender()==uiActionZoomIn)
		mActiveWindowImage->zoomIn();
	else if (sender()==uiActionZoomOut)
		mActiveWindowImage->zoomOut();
	else if (sender()==uiActionZoomOriginal)
		mActiveWindowImage->zoomOriginal();
	else if (sender()==uiActionZoomBestFit)
		mActiveWindowImage->zoomBestFit();
	
	uiActionZoomIn->setEnabled(mActiveWindowImage->mCurrentFactor < 3.0);
	uiActionZoomOut->setEnabled(mActiveWindowImage->mCurrentFactor > 0.25);
	mStatusBarLabelZoom->setText(mActiveWindowImage->mImageZoom);
}




void WindowMain::showHarrisToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mHarrisAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 0);
	uiActionHarris->setChecked(true);
}




void WindowMain::resetHarrisParams() {
	mUIHarris.uiComboBoxSobelApertureSize->setCurrentIndex(1);
	mUIHarris.uiSpinBoxHarrisApertureSize->setValue(2);
	mUIHarris.uiDoubleSpinBoxKValue->setValue(0.01);
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
	uiActionResetImage->setEnabled(true);
	mActiveWindowImage->mFeatureType=WindowImage::harris;
	mActiveWindow->setWindowIcon(*mIconHarris);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime + " ms");
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints + " keypoints");
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/Harris48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveHarrisParams() {
	mSettings->setValue("harris/sobelApertureSize", mUIHarris.uiComboBoxSobelApertureSize->currentIndex());
	mSettings->setValue("harris/harrisApertureSize", mUIHarris.uiSpinBoxHarrisApertureSize->value());
	mSettings->setValue("harris/kValue", mUIHarris.uiDoubleSpinBoxKValue->value());
}




void WindowMain::showFastToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mFastAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 1);
	uiActionFAST->setChecked(true);
}




void WindowMain::restFastParams() {
	mUIFast.uiSpinBoxThreshold->setValue(50);
	mUIFast.uiPushButtonNonMax->setChecked(true);
	saveFastParams();
}




void WindowMain::applyFast() {
	mActiveWindowImage->applyFast(mSettings->value("fast/threshold", 50).toInt(), mSettings->value("fast/nonMaxSuppression", true).toBool());
	uiActionResetImage->setEnabled(true);
	mActiveWindowImage->mFeatureType=WindowImage::fast;
	mActiveWindow->setWindowIcon(*mIconFAST);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime + " ms");
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints + " keypoints");
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/FAST48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveFastParams() {
	mSettings->setValue("fast/threshold", mUIFast.uiSpinBoxThreshold->value());
	mSettings->setValue("fast/nonMaxSuppression", mUIFast.uiPushButtonNonMax->isChecked());
}




void WindowMain::showSiftToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mSiftAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 2);
	uiActionSIFT->setChecked(true);
}




void WindowMain::resetSiftParams() {
	mUISift.uiDoubleSpinBoxThreshold->setValue(0.014);
	mUISift.uiDoubleSpinBoxEdgeThreshold->setValue(10.0);
	mUISift.uiSpinBoxOctaves->setValue(3);
	mUISift.uiSpinBoxLayers->setValue(1);
	mUISift.uiPushButtonShowOrientation->setChecked(true);
	saveSiftParams();
}




void WindowMain::applySift() {
	mActiveWindowImage->applySift(mSettings->value("sift/threshold", 0.014).toDouble(),
			mSettings->value("sift/edgeThreshold", 10.0).toDouble(),
			mSettings->value("sift/octaves", 3).toInt(),
			mSettings->value("sift/layers", 1).toInt(),
			mSettings->value("sift/showOrientation", true).toBool());
	uiActionResetImage->setEnabled(true);
	mActiveWindowImage->mFeatureType=WindowImage::sift;
	mActiveWindow->setWindowIcon(*mIconSIFT);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime + " ms");
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints + " keypoints");
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/SIFT48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveSiftParams() {
	mSettings->setValue("sift/threshold", mUISift.uiDoubleSpinBoxThreshold->value());
	mSettings->setValue("sift/edgeThreshold", mUISift.uiDoubleSpinBoxEdgeThreshold->value());
	mSettings->setValue("sift/octaves", mUISift.uiSpinBoxOctaves->value());
	mSettings->setValue("sift/layers", mUISift.uiSpinBoxLayers->value());
	mSettings->setValue("sift/showOrientation", mUISift.uiPushButtonShowOrientation->isChecked());
}




void WindowMain::showSurfToolBar() {
	if (mCurrentFeatureAction)
		mCurrentFeatureAction->setVisible(false);
	mCurrentFeatureAction = mSurfAction;
	mCurrentFeatureAction->setVisible(true);
	mSettings->setValue("startupParameters", 3);
	uiActionSURF->setChecked(true);
}




void WindowMain::resetSurfParams() {
	mUISurf.uiSpinBoxThreshold->setValue(4000);
	mUISurf.uiSpinBoxOctaves->setValue(3);
	mUISurf.uiSpinBoxLayers->setValue(1);
	mUISurf.uiPushButtonShowOrientation->setChecked(true);
	saveSurfParams();
}




void WindowMain::applySurf() {
	mActiveWindowImage->applySurf(mSettings->value("surf/threshold", 4000).toInt(),
			mSettings->value("surf/octaves", 3).toInt(),
			mSettings->value("surf/layers", 1).toInt(),
			0,
			mSettings->value("surf/showOrientation", true).toBool());
	uiActionResetImage->setEnabled(true);
	mActiveWindowImage->mFeatureType=WindowImage::surf;
	mActiveWindow->setWindowIcon(*mIconSURF);
	mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime + " ms");
	mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints + " keypoints");
	mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/SURF48.png")));
	mStatusBarLabelIcon->setVisible(true);
	mStatusBarLine2->setVisible(true);
	mStatusBarLine3->setVisible(true);
}




void WindowMain::saveSurfParams() {
	mSettings->setValue("surf/threshold", mUISurf.uiSpinBoxThreshold->value());
	mSettings->setValue("surf/octaves", mUISurf.uiSpinBoxOctaves->value());
	mSettings->setValue("surf/layers", mUISurf.uiSpinBoxLayers->value());
	mSettings->setValue("surf/showOrientation", mUISurf.uiPushButtonShowOrientation->isChecked());
}




void WindowMain::resetImage() {
	mActiveWindowImage->resetImage();
	uiActionResetImage->setEnabled(false);
	mActiveWindowImage->mFeatureType=WindowImage::none;
	mActiveWindow->setWindowIcon(QApplication::windowIcon());
	mStatusBarLabelIcon->clear();
	mStatusBarLabelIcon->setVisible(false);
	mStatusBarLabelTime->clear();
	mStatusBarLabelKeypoints->clear();
	mStatusBarLine2->setVisible(false);
	mStatusBarLine3->setVisible(false);
}




void WindowMain::do4() {
	WindowImage* harrisImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->mWindowTitle);
	WindowImage* fastImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->mWindowTitle);
	WindowImage* siftImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->mWindowTitle);
	WindowImage* surfImage = new WindowImage(mActiveWindowImage->mImage, mActiveWindowImage->mWindowTitle);
	
	int sobelApertureSize = 0;
	switch (mSettings->value("harris/sobelApertureSize", 1).toInt()) {
		case 0: sobelApertureSize=1; break;
		case 1: sobelApertureSize=3; break;
		case 2: sobelApertureSize=5; break;
		case 3: sobelApertureSize=7;
	}
	harrisImage->applyHarris(sobelApertureSize,
			mSettings->value("harris/harrisApertureSize", 2).toInt(),
			mSettings->value("harris/kValue", 0.01).toDouble());

	fastImage->applyFast(mSettings->value("fast/threshold", 50).toInt(), mSettings->value("fast/nonMaxSuppression", true).toBool());

	siftImage->applySift(mSettings->value("sift/threshold", 0.014).toDouble(),
			mSettings->value("sift/edgeThreshold", 10.0).toDouble(),
			mSettings->value("sift/octaves", 3).toInt(),
			mSettings->value("sift/layers", 1).toInt(),
			mSettings->value("sift/showOrientation", true).toBool());

	surfImage->applySurf(mSettings->value("surf/threshold", 4000).toInt(),
			mSettings->value("surf/octaves", 3).toInt(),
			mSettings->value("surf/layers", 1).toInt(),
			0,
			mSettings->value("surf/showOrientation", true).toBool());
	
	new WindowDo4(mActiveWindowImage->mWindowTitle, harrisImage, fastImage, siftImage, surfImage);
}




void WindowMain::openFastRT() {
	new WindowFastRealTime(this);
}




void WindowMain::tile() {
	uiMdiArea->tileSubWindows();
	if (mSettings->value("bestFit").toBool()) {
		QList<QMdiSubWindow*> subwindows = uiMdiArea->subWindowList();
		for (int n=0; n<subwindows.size(); ++n)
			qobject_cast<WindowImage*>(subwindows.at(n)->widget())->zoomBestFit();
	}
}




void WindowMain::cascade() {
	uiMdiArea->cascadeSubWindows();
	if (mSettings->value("bestFit").toBool()) {
		QList<QMdiSubWindow*> subwindows = uiMdiArea->subWindowList();
		for (int n=0; n<subwindows.size(); ++n)
			qobject_cast<WindowImage*>(subwindows.at(n)->widget())->zoomBestFit();
	}
}




void WindowMain::duplicate() {
	WindowImage* imageOriginal = uiMdiArea->findChild<WindowImage*>(QString(mActiveWindowImage->mOriginalUid));
	++imageOriginal->mImageN;
	WindowImage* imageDuplicated = new WindowImage(mActiveWindowImage->mImage,
							imageOriginal->mWindowTitle + QString(" (Duplicated %1)").arg(imageOriginal->mImageN),
							WindowImage::duplicated);
	imageDuplicated->mOriginalUid = imageOriginal->mUid;
	showWindowImage(imageDuplicated);
}




void WindowMain::closeActiveSubWindow() {
	uiMdiArea->closeActiveSubWindow();
}




void WindowMain::closeAllSubWindows() {
	uiMdiArea->closeAllSubWindows();
}




void WindowMain::website() {
	QDesktopServices::openUrl(QUrl::fromEncoded("https://github.com/AntonioRedondo/ImageFeatureDetector"));
}




void WindowMain::about() {
	new WindowAbout(this);
}




// http://doc.qt.io/qt-5/qtwidgets-mainwindows-mdi-example.html
void WindowMain::updateWindowMenu(QMdiSubWindow* mdiSubWindow) {
	if (!mSeparatorOpenWindowsAdded) { // Adding the separator on Qt Designer doesn't work
		uiMenuWindow->addSeparator();
		mSeparatorOpenWindowsAdded = true;
	}
	for (int n=0; n<mSubwindowActions->size(); ++n) {
// 		uiMenuWindow->removeAction(mSubwindowActions->at(n)); // Makes not to trigger new actions added
		mSubwindowActions->at(n)->setVisible(false);
	}
	mSubwindowActions->clear();
	
	if (mdiSubWindow != 0) {
		mActiveWindow = mdiSubWindow;
		mActiveWindowImage = qobject_cast<WindowImage*> (mdiSubWindow->widget());
		
		uiActionSaveCopyAs->setEnabled(true);
		uiActionCopy->setEnabled(true);
		mActionGroupZoom->setEnabled(true);
		mActionGroupFeatures->setEnabled(true);
		mActionGroupWindow->setEnabled(true);
		uiActionZoomIn->setEnabled(mActiveWindowImage->mCurrentFactor < 3.0);
		uiActionZoomOut->setEnabled(mActiveWindowImage->mCurrentFactor > 0.25);
		
		if (!mActiveWindowImage->mImageTime.isEmpty()) {
			uiActionResetImage->setEnabled(true);
			switch (mActiveWindowImage->mFeatureType) {
				case WindowImage::harris: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/Harris.png"))); break;
				case WindowImage::fast: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/Fast.png"))); break;
				case WindowImage::sift: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/Sift.png"))); break;
				case WindowImage::surf: mStatusBarLabelIcon->setPixmap(QPixmap::fromImage(QImage("icons/Surf.png"))); break;
			}
			mStatusBarLabelIcon->setVisible(true);
			mStatusBarLabelTime->setText(mActiveWindowImage->mImageTime);
			mStatusBarLabelKeypoints->setText(mActiveWindowImage->mImageKeypoints);
			mStatusBarLine2->setVisible(true);
			mStatusBarLine3->setVisible(true);
		} else {
			uiActionResetImage->setEnabled(false);
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
		
		QList<QMdiSubWindow*> subwindows = uiMdiArea->subWindowList();
		for (int n=0; n<subwindows.size(); ++n) {
			WindowImage* windowImage = qobject_cast<WindowImage*> (subwindows.at(n)->widget());
			QString actionName;
			if (n<9)
				actionName = tr("&%1 %2").arg(n+1).arg(windowImage->windowTitle());
			else actionName = tr("%1 %2").arg(n+1).arg(windowImage->windowTitle());
			QAction* actionSubwindow = uiMenuWindow->addAction(actionName);
			mSubwindowActions->append(actionSubwindow);
			actionSubwindow->setCheckable(true);
			if (uiMdiArea->activeSubWindow())
				actionSubwindow->setChecked(windowImage == mActiveWindowImage);
			else actionSubwindow->setChecked(false);
			mActionGroupWindow->addAction(actionSubwindow);
			mSignalMapper->setMapping(actionSubwindow, subwindows.at(n));
			connect(actionSubwindow, &QAction::triggered, mSignalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
		}
	} else if (uiMdiArea->subWindowList().size()==0) {
		uiActionSaveCopyAs->setEnabled(false);
		uiActionCopy->setEnabled(false);
		mActionGroupZoom->setEnabled(false);
		mActionGroupFeatures->setEnabled(false);
		mActionGroupWindow->setEnabled(false);
		uiActionResetImage->setEnabled(false);
		uiToolBarParameters->setEnabled(false);

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
			showWindowImage(new WindowImage(image, filePath));
			uiToolBarParameters->setEnabled(true);
		} else {
			removeRecentFile(filePath);
			QMessageBox::warning(this, tr("Image Feature Detector"), tr("Cannot open %1.").arg(filePath));
		}
	}
}




void WindowMain::showWindowImage(WindowImage* windowImage) {
	windowImage->mUid = ++mTotalImages;
	if (windowImage->mWindowType != WindowImage::duplicated)
		windowImage->mOriginalUid = mTotalImages;
	windowImage->setObjectName(QString(mTotalImages));
	uiMdiArea->addSubWindow(windowImage);
	windowImage->parentWidget()->setGeometry(0, 0, windowImage->mImage->width()+8, windowImage->mImage->height()+31); // 8 and 31 are hardcoded values for the decorations of the subwindow
	if (windowImage->mImage->width() > uiMdiArea->width())
		windowImage->parentWidget()->setGeometry(0, 0, uiMdiArea->width(), windowImage->parentWidget()->height());
	if (windowImage->mImage->height() > uiMdiArea->height())
		windowImage->parentWidget()->setGeometry(0, 0, windowImage->parentWidget()->width(), uiMdiArea->height());
	windowImage->show();
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




void WindowMain::removeRecentFile(QString filePath) {
	QStringList files = mSettings->value("recentFiles").toStringList();
	files.removeAll(filePath);
	mSettings->setValue("recentFiles", files);
	updateRecentFilesMenu();
}




// http://doc.qt.io/qt-5/qtwidgets-mainwindows-recentfiles-example.html
void WindowMain::updateRecentFilesMenu() {
	QStringList files = mSettings->value("recentFiles").toStringList();
	int numRecentFiles;
	if (files.size() < maxRecentFiles)
		numRecentFiles = files.size();
	else numRecentFiles = maxRecentFiles;
	mActionSeparatorRecentFiles->setVisible(numRecentFiles>0);
	mMenuRecentFiles->clear();
	for (int n=0; n<numRecentFiles; ++n) {
		mActionRecentFiles[n]->setText(tr("&%1 %2").arg(n+1).arg(QFileInfo(files[n]).fileName()));
		mActionRecentFiles[n]->setData(files[n]);
		mActionRecentFiles[n]->setVisible(true);
		mMenuRecentFiles->addAction(mActionRecentFiles[n]);
	}
	for (int n=numRecentFiles; n<maxRecentFiles; ++n)
		mActionRecentFiles[n]->setVisible(false);
}




void WindowMain::openRecentFile() {
	loadFile(qobject_cast<QAction*>(sender())->data().toString());
}




void WindowMain::setActiveSubWindow(QWidget* subWindow) {
	uiMdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(subWindow));
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