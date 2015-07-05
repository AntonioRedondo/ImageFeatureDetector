#include "windowCaptureWebcam.h"

WindowCaptureWebcam::WindowCaptureWebcam(WindowMain* windowMain)
		: mWindowMain(windowMain), QDialog(windowMain, Qt::Dialog), mCamera(NULL), mTimer(NULL) {
	setupUi(this);
	
	connect(uiPushButtonCapture, &QAbstractButton::clicked, this, &WindowCaptureWebcam::capture);
	connect(uiPushButtonCapture, &QAbstractButton::clicked, this, &WindowCaptureWebcam::ok);
	connect(uiPushButtonCapture, &QAbstractButton::clicked, this, &WindowCaptureWebcam::close);
	
	
	
// 	VideoCapture cap(0); // open the default camera
// 	if(!cap.isOpened())  // check if we succeeded
// 		return -1;

// 	Mat edges;
// 	for(;;) {
// 		Mat frame;
// 		cap >> frame; // get a new frame from camera
// 		cvtColor(frame, edges, COLOR_BGR2GRAY);
// 		GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
// 		Canny(edges, edges, 0, 30, 3);
// 		if(waitKey(30) >= 0) break;
// 	}
	
	
	
	

// 	mCamera = cvCaptureFromCAM(-1);
// 	mCapture;
// 	mCapture.open(0);
	if (mCamera) {
		if (!mTimer) {
			mTimer = new QTimer();
			connect(mTimer, SIGNAL(timeout()), this, SLOT(compute()));
		}
		qDebug() << "Camera Resolution: " << cvGetCaptureProperty(mCamera, CV_CAP_PROP_FRAME_WIDTH)
			<< cvGetCaptureProperty(mCamera, CV_CAP_PROP_FRAME_HEIGHT);
		mIplImage320 = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
		mTimer->start(40); //25fps
	} else {
		uiLabelRealTime->setText("Capture from webcam: there is some problem with the cam.\nCannot get images.");
		uiLabelCaptured->setText("Damn!");
		uiPushButtonOK->setEnabled(false);
		uiPushButtonCapture->setEnabled(false);
	}
	
	show();
}




void WindowCaptureWebcam::capture() {
	uiLabelCaptured->setPixmap(QPixmap::fromImage(QImage((uchar*)mIplImageRealTime->imageData, mIplImageRealTime->width, mIplImageRealTime->height, QImage::Format_RGB888)));
	mImage = new QImage((uchar*)mIplImageRealTime->imageData, mIplImageRealTime->width, mIplImageRealTime->height, QImage::Format_RGB888);
	uiPushButtonOK->setEnabled(true);
}




void WindowCaptureWebcam::ok() {
	WindowImage* windowImage = new WindowImage(mImage, tr("WebCam Captured Image %1").arg(++mWindowMain->mCapturedWebcamImages), WindowImage::fromWebcam);
// 	else myWindowImage = new windowImage(new QImage(myLabelRealTime->pixmap()->toImage()), tr("WebCam Captured Image %1").arg(++myWorker->capturedWebcamImages), windowImage::capturedWebcam);
	mWindowMain->uiMdiArea->addSubWindow(windowImage);
	windowImage->show();
	close();
}




void WindowCaptureWebcam::close() {
	if (mCamera)
		mTimer->stop();
// 	cvReleaseCapture(&myCamera);
// 	cvReleaseImage(&myIplImageRealTime);
// 	cvReleaseImage(&myIplImage320);
// 	myCapture.release();
	QWidget::close();
}




void WindowCaptureWebcam::closeEvent(QCloseEvent* closeEvent) {
	close();
	QWidget::closeEvent(closeEvent);
}




void WindowCaptureWebcam::compute() {
// 	myCamera->retrieve(myImage);
// 	Warning! cvQueryFrame() sometimes causes memory overflow. I don't know why it occurs sometimes and another ones doesn't.
	mIplImageRealTime = cvQueryFrame(mCamera);
	cvCvtColor(mIplImageRealTime, mIplImageRealTime, CV_BGR2RGB);
	cvResize(mIplImageRealTime, mIplImage320);
	uiLabelRealTime->setPixmap(QPixmap::fromImage(QImage((uchar*)mIplImage320->imageData, 320, 240, QImage::Format_RGB888)));
}