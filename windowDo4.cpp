#include "windowDo4.h"


WindowDo4::WindowDo4(QString windowTitle, QSettings* settings, WindowImage* harrisImage, WindowImage* fastImage, WindowImage* siftImage, WindowImage* surfImage)
		: mHarrisImage(harrisImage), mFastImage(fastImage), mSiftImage(siftImage), mSurfImage(surfImage), mTimer(new QTimer()) {
	setupUi(this);
	
	setWindowTitle(windowTitle + " - Do4!");
	setWindowIcon(QIcon("icons/Do448.png"));
	setAttribute(Qt::WA_DeleteOnClose);
	
	
	int sobelApertureSize = 0;
	switch (settings->value("harris/sobelApertureSize", 1).toInt()) {
		case 0: sobelApertureSize=1; break;
		case 1: sobelApertureSize=3; break;
		case 2: sobelApertureSize=5; break;
		case 3: sobelApertureSize=7;
	}
	mHarrisImage->applyHarris(sobelApertureSize,
			settings->value("harris/harrisApertureSize", 2).toInt(),
			settings->value("harris/kValue", 0.01).toDouble());

	mFastImage->applyFast(settings->value("fast/threshold", 50).toInt(), settings->value("fast/nonMaxSuppression", true).toBool());

	mSiftImage->applySift(settings->value("sift/threshold", 0.014).toDouble(),
			settings->value("sift/edgeThreshold", 10.0).toDouble(),
			settings->value("sift/octaves", 3).toInt(),
			settings->value("sift/layers", 1).toInt(),
			settings->value("sift/showOrientation", true).toBool());

	mSurfImage->applySurf(settings->value("surf/threshold", 4000).toInt(),
			settings->value("surf/octaves", 3).toInt(),
			settings->value("surf/layers", 1).toInt(),
			0,
			settings->value("surf/showOrientation", true).toBool());
	
	uiHLayout1->insertWidget(1, mHarrisImage);
	uiHLayout1->insertWidget(3, mFastImage);
	uiHLayout2->insertWidget(1, mSiftImage);
	uiHLayout2->insertWidget(3, mSurfImage);
	
	connect(uiPushButtonZoomBestFit, &QPushButton::released, this, &WindowDo4::zoomBestFit);
	connect(mTimer, &QTimer::timeout, this, &WindowDo4::zoomBestFit);
	
	uiHarrisTimeLabel->setText(mHarrisImage->mImageTime);
	uiHarrisKPLabel->setText(mHarrisImage->mImageKeypoints);
	uiFastTimeLabel->setText(mFastImage->mImageTime);
	uiFastKPLabel->setText(mFastImage->mImageKeypoints);
	uiSiftTimeLabel->setText(mSiftImage->mImageTime);
	uiSiftKPLabel->setText(mSiftImage->mImageKeypoints);
	uiSurfTimeLabel->setText(mSurfImage->mImageTime);
	uiSurfKPLabel->setText(mSurfImage->mImageKeypoints);
	
	// http://wiki.qt.io/Center_a_Window_on_the_Screen
	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
	show();
	
	// This timer is to resize the images once the WindowStateChange event is fired because the window is actually resized
	// after the event is fired, i.e., when the window's size is not yet ready and images would get a not fit size.
	mTimer->setSingleShot(true);
	mTimer->setInterval(200);
	mTimer->start();
	
}




void WindowDo4::zoomBestFit() {
	mHarrisImage->zoomBestFit();
	mFastImage->zoomBestFit();
	mSiftImage->zoomBestFit();
	mSurfImage->zoomBestFit();
}




void WindowDo4::changeEvent(QEvent* event) {
	if (event->type() == QEvent::WindowStateChange) {
		QWindowStateChangeEvent* eventb = static_cast<QWindowStateChangeEvent*>(event);
		if (eventb->oldState() == Qt::WindowMaximized && this->windowState() == Qt::WindowNoState)
			mTimer->start();
		else if (eventb->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized)
			mTimer->start();
	}
}
