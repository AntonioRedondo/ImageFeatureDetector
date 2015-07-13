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
	
	setAttribute(Qt::WA_DeleteOnClose);
	
	connect(uiPushButtonCapture, &QAbstractButton::clicked, this, &WindowCaptureWebcam::capture);
	connect(uiPushButtonOK, &QAbstractButton::clicked, this, &WindowCaptureWebcam::ok);
	connect(uiPushButtonCancel, &QAbstractButton::clicked, this, &WindowCaptureWebcam::close);
	
	if (mCamera.isOpened()) {
		mTimer = new QTimer();
		mTimer->start(40); //25fps
		connect(mTimer, &QTimer::timeout, this, &WindowCaptureWebcam::compute);
		uiPushButtonCapture->setEnabled(true);
// 		qDebug() << "Frame format: " << mCamera.get(CV_CAP_PROP_FORMAT);
// 		qDebug() << "Frame count: " << mCamera.get(CV_CAP_PROP_FRAME_COUNT);
// 		qDebug() << "Frame mode: " << mCamera.get(CV_CAP_PROP_MODE);
// 		qDebug() << "Frame rate: " << mCamera.get(CV_CAP_PROP_FPS);
// 		qDebug() << "Frame width: " << mCamera.get(CV_CAP_PROP_FRAME_WIDTH);
// 		qDebug() << "Frame height: " << mCamera.get(CV_CAP_PROP_FRAME_HEIGHT);
	} else {
		uiLabelRealTime->setText("There is some problem with the cam.\nCannot get images.");
		uiLabelCaptured->setText("Damn!");
	}
	
	show();
}




void WindowCaptureWebcam::capture() {
	uiLabelCaptured->setPixmap(QPixmap::fromImage(QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB888)));
	uiPushButtonOK->setEnabled(true);
// 	qDebug() << "Frame camptured?";
// 	qDebug() << "Frame width: " << mImageRT.cols;
// 	qDebug() << "Frame height: " << mImageRT.rows;
// 	qDebug() << "Frame height: " << mImageRT.step;
// 	qDebug() << "Image type: " << mImageRT.type();
// 	qDebug() << "CV_8UC3: " << (mImageRT.type() == CV_8UC3);
// 	qDebug() << "CV_8UC4: " << (mImageRT.type() == CV_8UC4);
// 	qDebug() << "CV_32FC1: " << (mImageRT.type() == CV_32FC1);
}




void WindowCaptureWebcam::ok() {
	mWindowMain->showWindowImage(new WindowImage(new QImage(uiLabelCaptured->pixmap()->toImage()), tr("WebCam Captured Image %1").arg(++mWindowMain->mCapturedWebcamImages), WindowImage::fromWebcam));
	close();
}




void WindowCaptureWebcam::close() {
	if (mTimer)
		mTimer->stop();
	mCamera.release();
	QWidget::close();
}




void WindowCaptureWebcam::closeEvent(QCloseEvent* closeEvent) {
	close();
	QWidget::closeEvent(closeEvent);
}




void WindowCaptureWebcam::compute() {
	mCamera >> mImageRT;
	cvtColor(mImageRT, mImageRT, CV_BGR2RGB);
	uiLabelRealTime->setPixmap(QPixmap::fromImage(QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB888))); // With RGB32 doesn't work
// 	uiLabelRealTime->setPixmap(QPixmap::fromImage(QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB888).rgbSwapped()));
}
