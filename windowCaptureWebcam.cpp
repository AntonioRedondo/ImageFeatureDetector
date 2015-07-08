/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#include "windowCaptureWebcam.h"

WindowCaptureWebcam::WindowCaptureWebcam(WindowMain* windowMain) : mWindowMain(windowMain), QDialog(windowMain, Qt::Dialog), mCamera(VideoCapture(0)), mTimer(0) {
	setupUi(this);
	
	connect(uiPushButtonCapture, &QAbstractButton::clicked, this, &WindowCaptureWebcam::capture);
	connect(uiPushButtonOK, &QAbstractButton::clicked, this, &WindowCaptureWebcam::ok);
	connect(uiPushButtonCancel, &QAbstractButton::clicked, this, &WindowCaptureWebcam::close);
	
	if (mCamera.isOpened()) {
		mTimer = new QTimer();
		mTimer->start(40); //25fps
		connect(mTimer, &QTimer::timeout, this, &WindowCaptureWebcam::compute);
		uiPushButtonCapture->setEnabled(true);
	} else {
		uiLabelRealTime->setText("There is some problem with the cam.\nCannot get images.");
		uiLabelCaptured->setText("Damn!");
	}
	
	show();
}




void WindowCaptureWebcam::capture() {
	mImage = new QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB32);
	uiLabelCaptured->setPixmap(QPixmap::fromImage(QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB32)));
	uiPushButtonOK->setEnabled(true);
}




void WindowCaptureWebcam::ok() {
	WindowImage* windowImage = new WindowImage(mImage, tr("WebCam Captured Image %1").arg(++mWindowMain->mCapturedWebcamImages), WindowImage::fromWebcam);
	mWindowMain->showWindowImage(windowImage);
	windowImage->show();
	close();
}




void WindowCaptureWebcam::close() {
	if (mTimer)
		mTimer->stop();
	QWidget::close();
}




void WindowCaptureWebcam::closeEvent(QCloseEvent* closeEvent) {
	close();
	QWidget::closeEvent(closeEvent);
}




void WindowCaptureWebcam::compute() {
	mCamera >> mImageRT;
// 	cvtColor(mImageRT, mImageRT, CV_BGR2RGB);
	uiLabelRealTime->setPixmap(QPixmap::fromImage(QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB32).rgbSwapped()));
}
