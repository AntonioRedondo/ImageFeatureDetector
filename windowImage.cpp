#include "windowImage.h"

WindowImage::WindowImage(QImage* imageConstr, QString windowTitleConstr, int titleTypeConstr,
	int imageNConstr) : myImage(imageConstr), windowImageTitle(windowTitleConstr), titleType(titleTypeConstr), imageN(imageNConstr) {
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	myPainter = new QPainter();
	myLocale = new QLocale(QLocale::English);
	if (titleTypeConstr==duplicated) setWindowTitle(windowTitleConstr.append(" (Duplicated %1)").arg(imageNConstr));
	else setWindowTitle(windowTitleConstr);

	scrollAreaWidgetContents->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	myPixmap = QPixmap::fromImage(*myImage);
	if (titleType==capturedWebcam || titleType==capturedRoboLab) {
		//Whit this we redirect the pointer myImage to a new perdurable time variable.
		//myIplImageRealTime which was the former target is deleted when we call to QWidget::close() in the capture window
		myImageOriginal = QImage(myPixmap.toImage());
		myImage = &myImageOriginal;
	}
	myPixmapOriginal = myPixmap;
	myLabelImage->setPixmap(myPixmap);
	modified = false;

	scaleFactorAbove100 = 0.5;
	scaleFactorUnder100 = 0.25;
	factorIncrement = 0;
	currentFactor = 1.0;

	originalSize = myImage->size();
	originalWidth = myImage->size().width();
	originalHeight = myImage->size().height();

	myImageZoom = tr("%1%").arg((int)(currentFactor*100));
	myImageDimensions = tr("%1x%2 px").arg(originalWidth).arg(originalHeight);
	float myNumber = myImage->numBytes()/(float)1024;
	if (myNumber > 1024) myImageSize = myLocale->toString(myNumber/(float)1024,'f', 2).append(" MiB");
	else myImageSize = myLocale->toString(myNumber,'f', 2).append(" KiB");

	qDebug() << "myImageZoom: " << myImageZoom;
	qDebug() << "myImageSize: " << myImageSize;
	qDebug() << "myImageDimensions: " << myImageDimensions;
	qDebug() << "myImageChannels: " << myImage->format();
	qDebug() << "Factor increment: " << factorIncrement;
	qDebug() << "Current factor: " << currentFactor;
}




void WindowImage::zoomIn() {
	if (currentFactor>=1.0) {
		factorIncrement = (currentFactor+scaleFactorAbove100)/currentFactor;
		currentFactor += scaleFactorAbove100;
	} else {
		factorIncrement = (currentFactor+scaleFactorUnder100)/currentFactor;
		currentFactor += scaleFactorUnder100;
	}
	scaleImage();
}




void WindowImage::zoomOut() {
	if (currentFactor>1.0) {
		factorIncrement = (currentFactor-scaleFactorAbove100)/currentFactor;
		currentFactor -= scaleFactorAbove100;
	} else {
		factorIncrement = (currentFactor-scaleFactorUnder100)/currentFactor;
		currentFactor -= scaleFactorUnder100;
	}
	scaleImage();
}




void WindowImage::zoomBestFit() {
	float correctF = 0.98; //This correct factor allows the image fits to the space of the main window without fear to scrollbars appear.
	int scrollWidth = scrollArea->size().width();
	int scrollHeight = scrollArea->size().height();

	float relationScroll = scrollWidth/(float)scrollHeight;
	float relationImage = originalWidth/(float)originalHeight;

	float scaleWidth = scrollWidth/(float)originalWidth;
	float scaleHeight = scrollHeight/(float)originalHeight;

	if (relationScroll > relationImage) {
		if (correctF*scaleHeight>1.0) factorIncrement = correctF*scaleHeight/currentFactor;
		else factorIncrement = correctF*scaleHeight/currentFactor;
		currentFactor = correctF*scaleHeight;
	} else {
		if (correctF*scaleWidth>1.0) factorIncrement = correctF*scaleWidth/currentFactor;
		else factorIncrement = correctF*scaleWidth/currentFactor;
		currentFactor = correctF*scaleWidth;
	}
	
	scaleImage();
}




void WindowImage::zoomOriginal() {
	factorIncrement = 1/currentFactor;
	currentFactor = 1.0;
	scaleImage();
}




void WindowImage::harris(int sobelApertureSize, int harrisApertureSize, double kValue) {
	// The dst in cvCornerHarris must be CV_32FC1 Mat or "IPL_DEPTH_32F, 1" IplImage.
	IplImage* myIplImage = cvCreateImage(cvSize(myImage->size().width(), myImage->size().height()), IPL_DEPTH_8U, 4);
	memcpy((uchar*)myIplImage->imageDataOrigin, myImage->bits(), sizeof(uchar)*myImage->numBytes());
	IplImage* myIplImageGray = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 1);
	cvCvtColor(myIplImage, myIplImageGray, CV_RGBA2GRAY);

	IplImage* myIplImageHarris = cvCreateImage(cvSize(myImage->size().width(), myImage->size().height()), IPL_DEPTH_32F, 1);
	float time = (float)cvGetTickCount();
	cvCornerHarris(myIplImageGray, myIplImageHarris, harrisApertureSize, sobelApertureSize, kValue);
	myImageTime = myLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
	myImageKeypoints = "-";

	IplImage* myIplImageHarris8U = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 1);
	IplImage* myIplImageFinal = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 4);

	//We have to do this scale conversion because the resulted myIplImageHarris has very little values and
	//we'll only see a black image then. In addition, like this we take advantage to pass from 32F to 8U.
	double min=0, max=255, minVal=0.0, maxVal=0.0, scale, shift;
	cvMinMaxLoc(myIplImageHarris, &minVal, &maxVal);
	scale = (max-min)/(maxVal-minVal);
	shift = -minVal*scale+min;
	cvConvertScale(myIplImageHarris, myIplImageHarris8U, scale, shift);
	cvCvtColor(myIplImageHarris8U, myIplImageFinal, CV_GRAY2RGBA);

	// Although it is not necessary to show the Harris image on screen, we have to save the image on myPixmap
	// because if not we can not copy it to clipboard.
	myPixmap = QPixmap::fromImage(QImage((uchar*)myIplImageFinal->imageDataOrigin, myIplImageFinal->width, myIplImageFinal->height, QImage::Format_RGB32));
	modified = true;
	myLabelImage->setPixmap(myPixmap.scaled(currentFactor*originalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	cvReleaseImage(&myIplImage);
	cvReleaseImage(&myIplImageGray);
	cvReleaseImage(&myIplImageHarris);
	cvReleaseImage(&myIplImageHarris8U);
	cvReleaseImage(&myIplImageFinal);


//This is the same that above but with Mat and C++ fuctions instead of IplImage and C fuctions.
//cornerHarris(), getTickCount() and convert from Mat to QImage does work fairly like with C fuctions.
	
// 	Mat myMat(myImage->width(), myImage->height(), CV_8UC4, myImage->bits());
// 	Mat myMatGray(myImage->width(), myImage->height(), CV_8UC1);
// 	cvtColor(myMat, myMatGray, CV_RGBA2GRAY);
// 
// 	Mat myMatHarris(myImage->width(), myImage->height(), CV_32FC1);
// 	float time = (float)getTickCount();
// 	cornerHarris(myMatGray, myMatHarris, blockSize, apertureSize, kValue);
// 	myImageTime = myLocale->toString((float)((getTickCount()-time)/(getTickFrequency()*1000)),'f', 2).append(" ms");
// 	myImageKeypoints = "--";
// 
// 	Mat myMatHarris8U(myImage->width(), myImage->height(), CV_8UC1);
// 	double min=0, max=255, minVal, maxVal, scale, shift;
// 	minMaxLoc(myMatHarris, &minVal, &maxVal);
// 	scale = (max-min)/(maxVal-minVal);
// 	shift = -minVal*scale+min;
// 	myMatHarris.convertTo(myMatHarris8U, CV_8UC1, scale, shift);
// 
// 	//With this we can convert rightly from Mat to CvMat. We need to do this conversion because the conversion
// 	//from Mat to QImage does not work fairly like the CvMat to QImage. oldCvMatColor MUST be 8UC4. If it is
// 	//8UC3 it does NOT work. The conversion code can be CV_GRAY2RGB or CV_GRAY2RGBA. QImage can be Format_RGB32
// 	//or Format_ARGB32. With Format_RGB888 there is a strange horizontal deformation or directly, a
// 	//distorsionated image.
// 	CvMat* oldCvMat = cvCreateMat(myImage->size().width(), myImage->size().height(), CV_8UC1);
// 	oldCvMat->data.ptr = myMatHarris8U.data;
// 	CvMat* oldCvMatColor = cvCreateMat(myMatHarris8U->size().width(), myMatHarris8U->size().height(), CV_8UC4);
// 	cvCvtColor(oldCvMat, oldCvMatColor, CV_GRAY2RGB);
// 	myPixmap = QPixmap::fromImage(QImage(oldCvMatColor->data.ptr, myMatHarris8U->size()->width(), myMatHarris8U->size()->height(), QImage::Format_RGB32));
// 	modified = true;
// 	myLabelImage->setPixmap(myPixmap);
}




void WindowImage::fast(int threshold, bool nonMaxSuppression) {
	if (modified) myPixmap = myPixmapOriginal;

	//see "test.cc" from fast_opencv-1.0
	IplImage* myIplImage = cvCreateImage(cvSize(myImage->size().width(), myImage->size().height()), IPL_DEPTH_8U, 4);
	memcpy((uchar*)myIplImage->imageDataOrigin, myImage->bits(), sizeof(uchar)*myImage->numBytes());
	IplImage* myIplImageGray = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 1);
	cvCvtColor(myIplImage, myIplImageGray, CV_RGBA2GRAY);

	vector<KeyPoint> myKeypoints;
	float time = (float)cvGetTickCount();
	FAST(myIplImageGray, myKeypoints, threshold, nonMaxSuppression);
	myImageTime = myLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
	myImageKeypoints = myLocale->toString((float)myKeypoints.size(),'f', 0).append(" keypoints");
	cvReleaseImage(&myIplImage);
	cvReleaseImage(&myIplImageGray);
	
	myPainter->begin(&myPixmap);
	myPainter->setPen(QColor::fromRgb(255, 0, 0));
	myPainter->setRenderHint(QPainter::Antialiasing);
	for(int n=0; n<myKeypoints.size(); ++n)
		myPainter->drawEllipse((int)myKeypoints.at(n).pt.x, (int)myKeypoints.at(n).pt.y, 3, 3);
	myPainter->end();
	modified = true;
	myLabelImage->setPixmap(myPixmap.scaled(currentFactor*originalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




void WindowImage::sift(double _threshold, double _edgeThreshold, int _nOctaves, int _nOctaveLayers, bool _showOrientation) {
	if (modified) myPixmap = myPixmapOriginal;

	Mat myMatImage(myImage->size().width(), myImage->size().height(), CV_8UC4, myImage->bits());//, (size_t)myImage->bytesPerLine()
	Mat myMatImageGray(myImage->size().width(), myImage->size().height(), CV_8UC1);
	cvtColor(myMatImage, myMatImageGray, CV_RGBA2GRAY);

	vector<KeyPoint> myKeypoints;
	SIFT::CommonParams mySIFTCommon(_nOctaves, _nOctaveLayers, SIFT::CommonParams::DEFAULT_FIRST_OCTAVE, SIFT::CommonParams::AVERAGE_ANGLE);
	SIFT::DetectorParams mySIFTDetector(_threshold, _edgeThreshold);
	SIFT::DescriptorParams mySIFTDescriptor(SIFT::DescriptorParams::GET_DEFAULT_MAGNIFICATION(), SIFT::DescriptorParams::DEFAULT_IS_NORMALIZE, SIFT::DescriptorParams::DESCRIPTOR_SIZE);
	SIFT mySIFT(mySIFTCommon, mySIFTDetector, mySIFTDescriptor);
// 	Mat myMatImageDescriptors(myImage->size().width(), myImage->size().height(), CV_8UC4);
	float time = (float)cvGetTickCount();
	mySIFT(myMatImageGray, Mat(), myKeypoints);
// 	mySIFT(myMatImageGray, Mat(), myKeypoints, myMatImageDescriptors);
// 	time = (float)cvGetTickCount()-time;
	myImageTime = myLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
	myImageKeypoints = myLocale->toString((float)myKeypoints.size(),'f', 0).append(" keypoints");

	myPainter->begin(&myPixmap);
	myPainter->setRenderHint(QPainter::Antialiasing);
	for(int n=0; n<myKeypoints.size(); ++n) {
		if(_showOrientation) {
			myPainter->setPen(QColor::fromRgb(255, 0, 0));
			myPainter->drawLine(QLineF(myKeypoints.at(n).pt.x,
								myKeypoints.at(n).pt.y,
								myKeypoints.at(n).pt.x + myKeypoints.at(n).size*qCos(myKeypoints.at(n).angle*3.14159265/180),
								myKeypoints.at(n).pt.y + myKeypoints.at(n).size*qSin(myKeypoints.at(n).angle*3.14159265/180)));
		}
		myPainter->setPen(QColor::fromRgb(0, 0, 255));
		myPainter->drawEllipse(myKeypoints.at(n).pt.x, myKeypoints.at(n).pt.y, myKeypoints.at(n).size, myKeypoints.at(n).size);
	}
	myPainter->end();

	modified = true;
	myLabelImage->setPixmap(myPixmap.scaled(currentFactor*originalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




void WindowImage::surf(double threshold, int nOctaves, int nOctaveLayers, int extended, bool showOrientation) {
	if (modified) myPixmap = myPixmapOriginal;

	IplImage* myIplImage = cvCreateImage(cvSize(myImage->size().width(), myImage->size().height()), IPL_DEPTH_8U, 4);
	memcpy((uchar*)myIplImage->imageDataOrigin, myImage->bits(), sizeof(uchar)*myImage->numBytes());
	IplImage* myIplImageGray = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 1);
	cvCvtColor(myIplImage, myIplImageGray, CV_RGBA2GRAY);

	CvSeq* myKeypoints;
	CvSURFParams myCvSURFParams = {0, threshold, nOctaves, nOctaveLayers};
	float time = (float)cvGetTickCount();
	cvExtractSURF(myIplImageGray, 0, &myKeypoints, 0, cvCreateMemStorage(), myCvSURFParams);
	myImageTime = myLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
	myImageKeypoints = myLocale->toString((float)myKeypoints->total,'f', 0).append(" keypoints");
	cvReleaseImage(&myIplImage);
	cvReleaseImage(&myIplImageGray);

	QPoint center;
	int radius;
	myPainter->begin(&myPixmap);
	myPainter->setRenderHint(QPainter::Antialiasing);
	for(int n=0; n<myKeypoints->total; ++n) {
		CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem(myKeypoints, n);
		center.setX((int)(r->pt.x));
		center.setY((int)(r->pt.y));
		radius = (int)(r->size);// radius = (int)(r->size*1.2/9.*2); = 0.266666
		if(showOrientation) {
			myPainter->setPen(QColor::fromRgb(255, 0, 0));
			myPainter->drawLine(QLine(int(r->pt.x),
								int(r->pt.y),
								int(r->pt.x + r->size*qCos(r->dir*3.14159265/180)),
								int(r->pt.y + r->size*qSin(r->dir*3.14159265/180))));
		}
		myPainter->setPen(QColor::fromRgb(0, 0, 255));
		myPainter->drawEllipse(center, radius, radius);
	}
	myPainter->end();
	modified = true;
	myLabelImage->setPixmap(myPixmap.scaled(currentFactor*originalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

// 	if (modified) myPixmap = myPixmapOriginal;
// 
// 	Mat myMatImage(myImage->size().width(), myImage->size().height(), CV_8UC4, myImage->bits());
// 	Mat myMatImageGray(myImage->size().width(), myImage->size().height(), CV_8UC1);
// 	cvtColor(myMatImage, myMatImageGray, CV_RGBA2GRAY);
// 
// 	vector<KeyPoint> myKeypoints;
// 	SURF mySURF(threshold, nOctaves, nOctaveLayers, extended);
// 	float time = (float)cvGetTickCount();
// 	mySURF(myMatImageGray, Mat(), myKeypoints);
// 	myImageTime = myLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
// 	myImageKeypoints = myLocale->toString((float)myKeypoints.size(),'f', 0).append(" keypoints");
// 
// 	myPainter->begin(&myPixmap);
// 	myPainter->setPen(QColor::fromRgb(255, 0, 0));
// 	myPainter->setRenderHint(QPainter::Antialiasing);
// 	for(int n=0; n<myKeypoints.size(); ++n) {
// 		qDebug() << myKeypoints.at(n).angle;
// 		myPainter->drawEllipse((int)myKeypoints.at(n).pt.x, (int)myKeypoints.at(n).pt.y, (int)myKeypoints.at(n).size, (int)myKeypoints.at(n).size);
// 	}
// 	myPainter->end();
// 
// 	modified = true;
// 	myLabelImage->setPixmap(myPixmap);
}




void WindowImage::resetImage() {
	myPixmap = myPixmapOriginal;
	myImageTime.clear();
	myImageKeypoints.clear();
	modified = false;
	myLabelImage->setPixmap(myPixmap.scaled(currentFactor*originalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




void WindowImage::mousePressEvent(QMouseEvent* event) {
	lastPoint = event->pos();
	setCursor(Qt::ClosedHandCursor);
}




void WindowImage::mouseMoveEvent(QMouseEvent* event) {
	QPoint myPos = event->pos();
	int hValue = scrollArea->horizontalScrollBar()->value();
	int vValue = scrollArea->verticalScrollBar()->value();
	scrollArea->horizontalScrollBar()->setValue(hValue+(lastPoint.x()-myPos.x()));
	scrollArea->verticalScrollBar()->setValue(vValue+(lastPoint.y()-myPos.y()));
	lastPoint = myPos;
}




void WindowImage::mouseReleaseEvent(QMouseEvent* event) {
	unsetCursor();
}




// void windowImage::mouseDoubleClickEvent(QMouseEvent* event) {
// 	if (event->button()==Qt::LeftButton) zoomIn();
// 	else zoomOut();
// }




void WindowImage::scaleImage() {
	qDebug() << "Factor increment: " << factorIncrement;
	qDebug() << "Current factor: " << currentFactor;
	qDebug() << "";
// 	myLabelImage->resize(currentFactor*originalSize);
	myLabelImage->setPixmap(myPixmap.scaled(currentFactor*originalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	adjustScrollBar(scrollArea->horizontalScrollBar());
	adjustScrollBar(scrollArea->verticalScrollBar());
	myImageZoom = tr("%1%").arg((int)(currentFactor*100));
}




void WindowImage::adjustScrollBar(QScrollBar* scrollBar) {
	scrollBar->setValue(int(factorIncrement*scrollBar->value()+(factorIncrement-1)*scrollBar->pageStep()/2));
}