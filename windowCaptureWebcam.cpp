#include "windowCaptureWebcam.h"

WindowCaptureWebcam::WindowCaptureWebcam(QWidget* _widget) : myWidget(_widget), QDialog(_widget, Qt::Dialog) {
	setupUi(this);
	myTimer = new QTimer();

	connect(myPushButtonCapture, SIGNAL(clicked(bool)), this, SLOT(capture()));
	connect(myPushButtonOK, SIGNAL(clicked(bool)), this, SLOT(ok()));
	connect(myPushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(myTimer, SIGNAL(timeout()), this, SLOT(compute()));

	myCamera = cvCaptureFromCAM(-1);
// 	myCapture;
// 	myCapture.open(0);
	if (myCamera!=NULL) {
		qDebug() << "Camera Resolution: " << cvGetCaptureProperty(myCamera, CV_CAP_PROP_FRAME_WIDTH)
			<< cvGetCaptureProperty(myCamera, CV_CAP_PROP_FRAME_HEIGHT);
		myIplImage320 = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
		myTimer->start(40); //25fps
	} else {
		myLabelRealTime->setText("Capture from webcam: there is some problem with the cam.\nCannot get images.");
		myLabelCaptured->setText("Damn!");
		myPushButtonOK->setEnabled(false);
		myPushButtonCapture->setEnabled(false);
	}
	show();
}




void WindowCaptureWebcam::capture() {
	myLabelCaptured->setPixmap(QPixmap::fromImage(QImage((uchar*)myIplImageRealTime->imageData, myIplImageRealTime->width, myIplImageRealTime->height, QImage::Format_RGB888)));
	myImage = new QImage((uchar*)myIplImageRealTime->imageData, myIplImageRealTime->width, myIplImageRealTime->height, QImage::Format_RGB888);
	myPushButtonOK->setEnabled(true);
}




void WindowCaptureWebcam::ok() {
	WindowMain* myWorker = qobject_cast<WindowMain*>(myWidget);
	WindowImage* myWindowImage;
	myWindowImage = new WindowImage(myImage, tr("WebCam Captured Image %1").arg(++myWorker->capturedWebcamImages), WindowImage::capturedWebcam);
// 	else myWindowImage = new windowImage(new QImage(myLabelRealTime->pixmap()->toImage()), tr("WebCam Captured Image %1").arg(++myWorker->capturedWebcamImages), windowImage::capturedWebcam);
	myWorker->myMdiArea->addSubWindow(myWindowImage);
	close();
	myWindowImage->show();
}




void WindowCaptureWebcam::close() {
	myTimer->stop();
	cvReleaseCapture(&myCamera);
	cvReleaseImage(&myIplImageRealTime);
	cvReleaseImage(&myIplImage320);
// 	myCapture.release();
	qDebug() << "WindowCaptureWebcam 1";
	QWidget::close();
}




void WindowCaptureWebcam::closeEvent(QCloseEvent* _event) {
	close();
	qDebug() << "closeEvent 1";
	QWidget::closeEvent(_event);
}




void WindowCaptureWebcam::compute() {
// 	myCamera->retrieve(myImage);
// 	Warning! cvQueryFrame() sometimes causes memory overflow. I don't know why it occurs sometimes and another ones doesn't.
	myIplImageRealTime = cvQueryFrame(myCamera);
	cvCvtColor(myIplImageRealTime, myIplImageRealTime, CV_BGR2RGB);
	cvResize(myIplImageRealTime, myIplImage320);
	myLabelRealTime->setPixmap(QPixmap::fromImage(QImage((uchar*)myIplImage320->imageData, 320, 240, QImage::Format_RGB888)));
}