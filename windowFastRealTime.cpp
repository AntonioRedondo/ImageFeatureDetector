#include "windowFastRealTime.h"

WindowFastRealTime::WindowFastRealTime(QWidget* _widget) : QDialog(_widget, Qt::Dialog) {
	setupUi(this);
	mySettings = new QSettings("./imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	myTimer = new QTimer();
	myLocale = new QLocale(QLocale::English);
	detecting = false;

	spinBoxThresholdFAST->setValue(mySettings->value("fastRT/threshold", 25).toInt());
	pushButtonNonMaxFAST->setChecked(mySettings->value("fastRT/nonMaxSuppression", true).toBool());

	connect(pushButtonNonMaxFAST, SIGNAL(toggled(bool)), this, SLOT(fastSaveParams()));
	connect(spinBoxThresholdFAST, SIGNAL(editingFinished()), this, SLOT(fastSaveParams()));
	connect(pushButtonResetFAST, SIGNAL(clicked(bool)), this, SLOT(fastReset()));
	connect(myPushButtonDetect, SIGNAL(clicked(bool)), this, SLOT(detect()));
	connect(myPushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(myTimer, SIGNAL(timeout()), this, SLOT(compute()));

	myCamera = cvCaptureFromCAM(-1);
	if (myCamera!=NULL) {
// 		qDebug() << cvSetCaptureProperty(myCamera, CV_CAP_PROP_FRAME_WIDTH, 320);
// 		qDebug() << cvSetCaptureProperty(myCamera, CV_CAP_PROP_FRAME_HEIGHT, 240);
		qDebug() << "Camera Resolution: " << cvGetCaptureProperty(myCamera, CV_CAP_PROP_FRAME_WIDTH)
			<< cvGetCaptureProperty(myCamera, CV_CAP_PROP_FRAME_HEIGHT);
			
		myIplImage320 = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
		myIplImage320Gray = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 1);
		myPainter = new QPainter();

		myTimer->start(40); //25fps
	} else {
		myLabelRealTime->setText("Capture from webcam: there is some problem with the cam.\nCannot get images.");
		myPushButtonDetect->setEnabled(false);
	}

	show();
}




void WindowFastRealTime::detect() {
	if (detecting==false) {
		detecting = true;
		myPushButtonDetect->setIcon(QIcon("../icons24/media-playback-stop.png"));
		myPushButtonDetect->setText("Stop Detecting");
	}
	else {
		detecting = false;
		myPushButtonDetect->setIcon(QIcon("../icons24/media-playback-start.png"));
		myPushButtonDetect->setText("Detect");
	}
}




void WindowFastRealTime::close() {
	myTimer->stop();
	cvReleaseCapture(&myCamera);
	cvReleaseImage(&myIplImageRealTime);
	cvReleaseImage(&myIplImage320);
	cvReleaseImage(&myIplImage320Gray);
	QWidget::close();
}




void WindowFastRealTime::closeEvent(QCloseEvent* _event) {
	close();
	QWidget::closeEvent(_event);
}




void WindowFastRealTime::fastSaveParams() {
	mySettings->setValue("fastRT/threshold", spinBoxThresholdFAST->value());
	mySettings->setValue("fastRT/nonMaxSuppression", pushButtonNonMaxFAST->isChecked());
}




void WindowFastRealTime::fastReset() {
	spinBoxThresholdFAST->setValue(25);
	pushButtonNonMaxFAST->setChecked(true);
	fastSaveParams();
}




void WindowFastRealTime::compute() {
// 	Warning! cvQueryFrame() sometimes causes memory overflow. I don't know why it occurs sometimes and another ones it doesn't.
	myIplImageRealTime = cvQueryFrame(myCamera);
	cvResize(myIplImageRealTime, myIplImage320);
	cvCvtColor(myIplImage320, myIplImage320Gray, CV_BGR2GRAY);

	if (detecting) {
		time = (float)cvGetTickCount();
		FAST(myIplImage320Gray, myKeypoints, mySettings->value("fastRT/threshold", true).toInt(), mySettings->value("fastRT/nonMaxSuppression", true).toBool());
		myLabelTime->setText(QString::fromUtf8("Detecting Time: ").append(myLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms")));
		myLabelKeypoints->setText(QString::fromUtf8("Keypoints: ").append(myLocale->toString((float)myKeypoints.size(),'f', 0).append(" keypoints")));
		
		myPixmap = QPixmap::fromImage(QImage((uchar*)myIplImage320->imageData, 320, 240, QImage::Format_RGB888).rgbSwapped());
		myPainter->begin(&myPixmap);
		myPainter->setPen(QColor::fromRgb(255, 0, 0));
	// 	myPainter->setRenderHint(QPainter::Antialiasing);
		for(int n=0; n<myKeypoints.size(); ++n)
			myPainter->drawEllipse((int)myKeypoints.at(n).pt.x, (int)myKeypoints.at(n).pt.y, 3, 3);
		myPainter->end();
		myLabelRealTime->setPixmap(myPixmap);
	} else {
		myLabelRealTime->setPixmap(QPixmap::fromImage(QImage((uchar*)myIplImage320->imageData, 320, 240, QImage::Format_RGB888).rgbSwapped()));
		myLabelTime->setText("Detecting Time: -");
		myLabelKeypoints->setText("Keypoints: -");
	}
}