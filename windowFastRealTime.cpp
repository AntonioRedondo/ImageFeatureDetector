#include "windowFastRealTime.h"

WindowFastRealTime::WindowFastRealTime(QWidget* widget)
		: QDialog(widget, Qt::Dialog) {
	setupUi(this);
	mSettings = new QSettings("./imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	mTimer = new QTimer();
	mLocale = new QLocale(QLocale::English);
	mDetecting = false;

	uiSpinBoxThresholdFAST->setValue(mSettings->value("fastRT/threshold", 25).toInt());
	uiPushButtonNonMaxFAST->setChecked(mSettings->value("fastRT/nonMaxSuppression", true).toBool());
	
	connect(uiPushButtonNonMaxFAST, &QPushButton::toggled, this, &WindowFastRealTime::saveFastParams);
	connect(uiSpinBoxThresholdFAST, &QSpinBox::editingFinished, this, &WindowFastRealTime::saveFastParams);
	connect(uiPushButtonResetFAST, &QAbstractButton::clicked, this, &WindowFastRealTime::resetFastParams);
	connect(uiPushButtonDetect, &QAbstractButton::clicked, this, &WindowFastRealTime::detect);
	connect(uiPushButtonCancel, &QAbstractButton::clicked, this, &WindowFastRealTime::close);
	connect(mTimer, &QTimer::timeout, this, &WindowFastRealTime::compute);

	mCamera = cvCaptureFromCAM(-1);
	if (mCamera!=NULL) {
		qDebug() << "Camera Resolution: " << cvGetCaptureProperty(mCamera, CV_CAP_PROP_FRAME_WIDTH)
			<< cvGetCaptureProperty(mCamera, CV_CAP_PROP_FRAME_HEIGHT);
			
		mIplImage320 = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
		mIplImage320Gray = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 1);
		mPainter = new QPainter();

		mTimer->start(40); //25fps
	} else {
		uiLabelRealTime->setText("Capture from webcam: there is some problem with the cam.\nCannot get images.");
		uiPushButtonDetect->setEnabled(false);
	}

	show();
}




void WindowFastRealTime::detect() {
	if (mDetecting==false) {
		mDetecting = true;
		uiPushButtonDetect->setIcon(QIcon("../icons24/media-playback-stop.png"));
		uiPushButtonDetect->setText("Stop Detecting");
	} else {
		mDetecting = false;
		uiPushButtonDetect->setIcon(QIcon("../icons24/media-playback-start.png"));
		uiPushButtonDetect->setText("Detect");
	}
}




void WindowFastRealTime::close() {
	mTimer->stop();
	cvReleaseCapture(&mCamera);
	cvReleaseImage(&mIplImageRealTime);
	cvReleaseImage(&mIplImage320);
	cvReleaseImage(&mIplImage320Gray);
	QWidget::close();
}




void WindowFastRealTime::closeEvent(QCloseEvent* _event) {
	close();
	QWidget::closeEvent(_event);
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
// 	Warning! cvQueryFrame() sometimes causes memory overflow. I don't know why it occurs sometimes and another ones it doesn't.
	mIplImageRealTime = cvQueryFrame(mCamera);
	cvResize(mIplImageRealTime, mIplImage320);
	cvCvtColor(mIplImage320, mIplImage320Gray, CV_BGR2GRAY);
	
// 	Mat image(myImage->size().width(), myImage->size().height(), CV_8UC4, myImage->bits());//, (size_t)myImage->bytesPerLine()
// 	Mat imageGray(myImage->size().width(), myImage->size().height(), CV_8UC1);
// 	cvtColor(image, imageGray, CV_RGBA2GRAY);

	if (mDetecting) {
		mTime = (float)cvGetTickCount();
		Mat aa = cvarrToMat(mIplImage320Gray);
		FAST(aa, mKeypoints, mSettings->value("fastRT/threshold", true).toInt(), mSettings->value("fastRT/nonMaxSuppression", true).toBool());
		uiLabelTime->setText(QString::fromUtf8("Detecting Time: ").append(mLocale->toString((float)((cvGetTickCount()-mTime)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms")));
		uiLabelKeypoints->setText(QString::fromUtf8("Keypoints: ").append(mLocale->toString((float)mKeypoints.size(),'f', 0).append(" keypoints")));
		
		mPixmap = QPixmap::fromImage(QImage((uchar*)mIplImage320->imageData, 320, 240, QImage::Format_RGB888).rgbSwapped());
		mPainter->begin(&mPixmap);
		mPainter->setPen(QColor::fromRgb(255, 0, 0));
	// 	myPainter->setRenderHint(QPainter::Antialiasing);
		for(int n=0; n<mKeypoints.size(); ++n)
			mPainter->drawEllipse((int)mKeypoints.at(n).pt.x, (int)mKeypoints.at(n).pt.y, 3, 3);
		mPainter->end();
		uiLabelRealTime->setPixmap(mPixmap);
	} else {
		uiLabelRealTime->setPixmap(QPixmap::fromImage(QImage((uchar*)mIplImage320->imageData, 320, 240, QImage::Format_RGB888).rgbSwapped()));
		uiLabelTime->setText("Detecting Time: -");
		uiLabelKeypoints->setText("Keypoints: -");
	}
}