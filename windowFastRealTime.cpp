/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#include "windowFastRealTime.h"

WindowFastRealTime::WindowFastRealTime(WindowMain* windowMain) : QDialog(windowMain, Qt::Dialog), mDetecting(false), mCamera(VideoCapture(0)), mTimer(0) {
	setupUi(this);
	
	setAttribute(Qt::WA_DeleteOnClose);
	mSettings = new QSettings("imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	mLocale = new QLocale(QLocale::English);

	uiSpinBoxThresholdFAST->setValue(mSettings->value("fastRT/threshold", 25).toInt());
	uiPushButtonNonMaxFAST->setChecked(mSettings->value("fastRT/nonMaxSuppression", true).toBool());
	
	connect(uiPushButtonNonMaxFAST, &QPushButton::toggled, this, &WindowFastRealTime::saveFastParams);
	connect(uiSpinBoxThresholdFAST, &QSpinBox::editingFinished, this, &WindowFastRealTime::saveFastParams);
	connect(uiPushButtonResetFAST, &QAbstractButton::clicked, this, &WindowFastRealTime::resetFastParams);
	connect(uiPushButtonDetect, &QAbstractButton::clicked, this, &WindowFastRealTime::detect);
	connect(uiPushButtonCancel, &QAbstractButton::clicked, this, &WindowFastRealTime::close);
	
	if (mCamera.isOpened()) {
		mPainter = new QPainter();
		mTimer = new QTimer();
		mTimer->start(40); //25fps
		connect(mTimer, &QTimer::timeout, this, &WindowFastRealTime::compute);
		uiPushButtonDetect->setEnabled(true);
	} else {
		uiLabelRealTime->setText("There is some problem with the cam.\nCannot get images.");
	}

	show();
}




void WindowFastRealTime::detect() {
	if (!mDetecting) {
		uiPushButtonDetect->setIcon(QIcon("icons/media-playback-stop.svg"));
		uiPushButtonDetect->setText("Stop Detecting");
	} else {
		uiPushButtonDetect->setIcon(QIcon("icons/media-playback-start.svg"));
		uiPushButtonDetect->setText("Detect");
	}
	mDetecting = !mDetecting;
}




void WindowFastRealTime::close() {
	if (mTimer)
		mTimer->stop();
	mCamera.release();
	QWidget::close();
}




void WindowFastRealTime::closeEvent(QCloseEvent* closeEvent) {
	close();
	QWidget::closeEvent(closeEvent);
}




void WindowFastRealTime::saveFastParams() {
	mSettings->setValue("fastRT/threshold", uiSpinBoxThresholdFAST->value());
	mSettings->setValue("fastRT/nonMaxSuppression", uiPushButtonNonMaxFAST->isChecked());
}




void WindowFastRealTime::resetFastParams() {
	uiSpinBoxThresholdFAST->setValue(25);
	uiPushButtonNonMaxFAST->setChecked(true);
	saveFastParams();
}




void WindowFastRealTime::compute() {
	mCamera >> mImageRT;
	if (mDetecting) {
		Mat mImageGrey(mImageRT.rows, mImageRT.cols, CV_8UC1);
		cvtColor(mImageRT, mImageGrey, CV_RGB2GRAY);
		mTime = (float) getTickCount();
		FAST(mImageGrey, mKeypoints, mSettings->value("fastRT/threshold", true).toInt(), mSettings->value("fastRT/nonMaxSuppression", true).toBool());
		uiLabelTime->setText(QString("Detecting Time: ").append(mLocale->toString((float)((getTickCount()-mTime)/(getTickFrequency()*1000)),'f', 2).append(" ms")));
		uiLabelKeypoints->setText(QString("Keypoints: ").append(mLocale->toString((float)mKeypoints.size(),'f', 0)));
		
		mPixmap = QPixmap::fromImage(QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB888).rgbSwapped());
		mPainter->begin(&mPixmap);
		QPen pen(QColor::fromRgb(255, 0, 0));
		pen.setWidth(2);
		mPainter->setPen(pen);
		mPainter->setRenderHint(QPainter::Antialiasing);
		for(int n=0; n<mKeypoints.size(); ++n)
			mPainter->drawEllipse((int)mKeypoints.at(n).pt.x, (int)mKeypoints.at(n).pt.y, 4, 4);
		mPainter->end();
		uiLabelRealTime->setPixmap(mPixmap);
	} else {
		uiLabelRealTime->setPixmap(QPixmap::fromImage(QImage(mImageRT.data, mImageRT.cols, mImageRT.rows, mImageRT.step, QImage::Format_RGB888).rgbSwapped()));
		uiLabelTime->setText("Detecting Time: -");
		uiLabelKeypoints->setText("Keypoints: -");
	}
}