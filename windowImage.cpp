#include "windowImage.h"

WindowImage::WindowImage(QImage* imageConstr, QString windowTitleConstr, int titleTypeConstr, int imageNConstr)
		: mImage(imageConstr), windowImageTitle(windowTitleConstr), titleType(titleTypeConstr), imageN(imageNConstr) {
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	mPainter = new QPainter();
	mLocale = new QLocale(QLocale::English);
	if (titleTypeConstr==duplicated)
		setWindowTitle(windowTitleConstr.append(" (Duplicated %1)").arg(imageNConstr));
	else setWindowTitle(windowTitleConstr);

	scrollAreaWidgetContents->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	mPixmap = QPixmap::fromImage(*mImage);
	if (titleType==capturedWebcam) {
		// Whit this we redirect the pointer mImage to a new perdurable variable.
		// myIplImageRealTime which was the former target is deleted when we call to QWidget::close() in the capture window
		mImageOriginal = QImage(mPixmap.toImage());
		mImage = &mImageOriginal;
	}
	mPixmapOriginal = mPixmap;
	myLabelImage->setPixmap(mPixmap);
	mModified = false;

	mScaleFactorAbove100 = 0.5;
	mScaleFactorUnder100 = 0.25;
	mFactorIncrement = 0;
	currentFactor = 1.0;

	mOriginalSize = mImage->size();
	mOriginalWidth = mImage->size().width();
	mOiginalHeight = mImage->size().height();

	mImageZoom = tr("%1%").arg((int)(currentFactor*100));
	mImageDimensions = tr("%1x%2 px").arg(mOriginalWidth).arg(mOiginalHeight);
	float myNumber = mImage->byteCount()/(float)1024;
	if (myNumber > 1024)
		mImageSize = mLocale->toString(myNumber/(float)1024,'f', 2).append(" MiB");
	else mImageSize = mLocale->toString(myNumber,'f', 2).append(" KiB");

// 	qDebug() << "myImageZoom: " << mImageZoom;
// 	qDebug() << "myImageSize: " << mImageSize;
// 	qDebug() << "myImageDimensions: " << mImageDimensions;
// 	qDebug() << "myImageChannels: " << mImage->format();
// 	qDebug() << "Factor increment: " << mFactorIncrement;
// 	qDebug() << "Current factor: " << currentFactor;
}




void WindowImage::zoomIn() {
	if (currentFactor>=1.0) {
		mFactorIncrement = (currentFactor+mScaleFactorAbove100)/currentFactor;
		currentFactor += mScaleFactorAbove100;
	} else {
		mFactorIncrement = (currentFactor+mScaleFactorUnder100)/currentFactor;
		currentFactor += mScaleFactorUnder100;
	}
	scaleImage();
}




void WindowImage::zoomOut() {
	if (currentFactor>1.0) {
		mFactorIncrement = (currentFactor-mScaleFactorAbove100)/currentFactor;
		currentFactor -= mScaleFactorAbove100;
	} else {
		mFactorIncrement = (currentFactor-mScaleFactorUnder100)/currentFactor;
		currentFactor -= mScaleFactorUnder100;
	}
	scaleImage();
}




void WindowImage::zoomBestFit() {
	float correctF = 0.98; // This correct factor allows the image fits to the space of the main window without scrollbars.
	int scrollWidth = scrollArea->size().width();
	int scrollHeight = scrollArea->size().height();

	float relationScroll = scrollWidth/(float)scrollHeight;
	float relationImage = mOriginalWidth/(float)mOiginalHeight;

	float scaleWidth = scrollWidth/(float)mOriginalWidth;
	float scaleHeight = scrollHeight/(float)mOiginalHeight;

	if (relationScroll > relationImage) {
		if (correctF*scaleHeight>1.0)
			mFactorIncrement = correctF*scaleHeight/currentFactor;
		else mFactorIncrement = correctF*scaleHeight/currentFactor;
		currentFactor = correctF*scaleHeight;
	} else {
		if (correctF*scaleWidth>1.0)
			mFactorIncrement = correctF*scaleWidth/currentFactor;
		else mFactorIncrement = correctF*scaleWidth/currentFactor;
		currentFactor = correctF*scaleWidth;
	}
	
	scaleImage();
}




void WindowImage::zoomOriginal() {
	mFactorIncrement = 1/currentFactor;
	currentFactor = 1.0;
	scaleImage();
}




void WindowImage::applyHarris(int sobelApertureSize, int harrisApertureSize, double kValue) {
	Mat image(mImage->height(), mImage->width(), CV_8UC4, mImage->bits(), mImage->bytesPerLine()); // With CV_8UC3 it doesn't work
	Mat imageGray(mImage->height(), mImage->width(), CV_8UC1);
	cvtColor(image, imageGray, CV_RGB2GRAY);
	
	Mat imageHarris(mImage->height(), mImage->width(), CV_32FC1);
	float time = (float) getTickCount();
	cornerHarris(imageGray, imageHarris, harrisApertureSize, sobelApertureSize, kValue);
	
	mImageTime = mLocale->toString((float)((getTickCount()-time)/(getTickFrequency()*1000)),'f', 2).append(" ms");
	mImageKeypoints = "--";
	
	// Incrises the contrast. If not only an almost black image would be seen
	Mat imageHarris8U(mImage->height(), mImage->width(), CV_8UC1);
	double min=0, max=255, minVal, maxVal, scale, shift;
	minMaxLoc(imageHarris, &minVal, &maxVal);
	scale = (max-min)/(maxVal-minVal);
	shift = -minVal*scale+min;
	imageHarris.convertTo(imageHarris8U, CV_8UC1, scale, shift);
	
// 	Mat imageColor(mImage->height(), mImage->width(), CV_8UC4); // With CV_8UC3 it doesn't work
// 	cvtColor(imageHarris8U, imageColor, CV_GRAY2RGBA); // With CV_GRAY2RGB it doesn't work
// 	mPixmap = QPixmap::fromImage(QImage(imageColor.data, mImage->width(), mImage->height(), imageColor.step, QImage::Format_RGB32)); // With Format_RGB888 it doesn't work. It can be Format_ARGB32 as well
	mPixmap = QPixmap::fromImage(convertMat2QImage(imageHarris8U)); // This should be faster than the above three lines
	mModified = true;
	myLabelImage->setPixmap(mPixmap);
	
	
	// The same that above but with IplImage and C fuctions. The dst in cvCornerHarris must be CV_32FC1 Mat or "IPL_DEPTH_32F, 1" IplImage.
	
// 	IplImage* myIplImage = cvCreateImage(cvSize(mImage->size().width(), mImage->size().height()), IPL_DEPTH_8U, 4);
// 	memcpy((uchar*)myIplImage->imageDataOrigin, mImage->bits(), sizeof(uchar)*mImage->byteCount());
// 	IplImage* myIplImageGray = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 1);
// 	cvCvtColor(myIplImage, myIplImageGray, CV_RGBA2GRAY);
// 
// 	IplImage* myIplImageHarris = cvCreateImage(cvSize(mImage->size().width(), mImage->size().height()), IPL_DEPTH_32F, 1);
// 	float time = (float)cvGetTickCount();
// 	cvCornerHarris(myIplImageGray, myIplImageHarris, harrisApertureSize, sobelApertureSize, kValue);
// 	mImageTime = mLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
// 	mImageKeypoints = "-";
// 
// 	IplImage* myIplImageHarris8U = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 1);
// 	IplImage* myIplImageFinal = cvCreateImage(cvGetSize(myIplImage), IPL_DEPTH_8U, 4);
// 
// 	double min=0, max=255, minVal=0.0, maxVal=0.0, scale, shift;
// 	cvMinMaxLoc(myIplImageHarris, &minVal, &maxVal);
// 	scale = (max-min)/(maxVal-minVal);
// 	shift = -minVal*scale+min;
// 	cvConvertScale(myIplImageHarris, myIplImageHarris8U, scale, shift);
// 	cvCvtColor(myIplImageHarris8U, myIplImageFinal, CV_GRAY2RGBA);
// 
// 	mPixmap = QPixmap::fromImage(QImage((uchar*)myIplImageFinal->imageDataOrigin, myIplImageFinal->width, myIplImageFinal->height, QImage::Format_RGB32));
// 	mModified = true;
// 	myLabelImage->setPixmap(mPixmap.scaled(currentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
// 	cvReleaseImage(&myIplImage);
// 	cvReleaseImage(&myIplImageGray);
// 	cvReleaseImage(&myIplImageHarris);
// 	cvReleaseImage(&myIplImageHarris8U);
// 	cvReleaseImage(&myIplImageFinal);
}




void WindowImage::applyFast(int threshold, bool nonMaxSuppression) {
	if (mModified)
		mPixmap = mPixmapOriginal;
	
	Mat image(mImage->height(), mImage->width(), CV_8UC4, mImage->bits(), mImage->bytesPerLine()); // With CV_8UC3 it doesn't work
	Mat imageGray(mImage->height(), mImage->width(), CV_8UC1);
	cvtColor(image, imageGray, CV_RGB2GRAY);
	
	vector<KeyPoint> keypoints;
	float time = (float) getTickCount();
	FAST(imageGray, keypoints, threshold, nonMaxSuppression);
	
	mImageTime = mLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
	mImageKeypoints = mLocale->toString((float)keypoints.size(),'f', 0).append(" keypoints");
	
	mPainter->begin(&mPixmap);
	mPainter->setPen(QColor::fromRgb(255, 0, 0));
	mPainter->setRenderHint(QPainter::Antialiasing);
	for (int n=0; n<keypoints.size(); ++n)
		mPainter->drawEllipse((int)keypoints.at(n).pt.x, (int)keypoints.at(n).pt.y, 3, 3);
	mPainter->end();
	
	mModified = true;
	myLabelImage->setPixmap(mPixmap.scaled(currentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




void WindowImage::applySift(double threshold, double edgeThreshold, int nOctaves, int nOctaveLayers, bool showOrientation) {
	if (mModified)
		mPixmap = mPixmapOriginal;

	Mat image(mImage->height(), mImage->width(), CV_8UC4, mImage->bits(), mImage->bytesPerLine()); // With CV_8UC3 it doesn't work
	Mat imageGray(mImage->height(), mImage->width(), CV_8UC1);
	cvtColor(image, imageGray, CV_RGB2GRAY);
	
	vector<KeyPoint> keypoints;
	float time = (float) getTickCount();
	Ptr<Feature2D> feature = SIFT::create(nOctaveLayers, nOctaves, threshold, edgeThreshold);
	feature->detect(imageGray, keypoints);
	
	mImageTime = mLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
	mImageKeypoints = mLocale->toString((float)keypoints.size(),'f', 0).append(" keypoints");
	
	QPoint center;
	int radius;
	mPainter->begin(&mPixmap);
	mPainter->setRenderHint(QPainter::Antialiasing);
	for (int n=0; n<keypoints.size(); ++n) {
		center.setX((int) (keypoints.at(n).pt.x));
		center.setY((int) (keypoints.at(n).pt.y));
		radius = (int) (keypoints.at(n).size); // radius = (int)(keypoints->at(n).size*1.2/9.*2); = 0.266666
		if (showOrientation) {
			mPainter->setPen(QColor::fromRgb(255, 0, 0));
			mPainter->drawLine(QLineF(keypoints.at(n).pt.x,
					keypoints.at(n).pt.y,
					keypoints.at(n).pt.x + keypoints.at(n).size*qCos(keypoints.at(n).angle*3.14159265/180),
					keypoints.at(n).pt.y + keypoints.at(n).size*qSin(keypoints.at(n).angle*3.14159265/180)));
		}
		mPainter->setPen(QColor::fromRgb(0, 0, 255));
		mPainter->drawEllipse(center, radius, radius);
	}
	mPainter->end();
	
	mModified = true;
	myLabelImage->setPixmap(mPixmap.scaled(currentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




void WindowImage::applySurf(double threshold, int nOctaves, int nOctaveLayers, int extended, bool showOrientation) {
	if (mModified)
		mPixmap = mPixmapOriginal;
	
	Mat image(mImage->height(), mImage->width(), CV_8UC4, mImage->bits(), mImage->bytesPerLine()); // With CV_8UC3 it doesn't work
	Mat imageGray(mImage->height(), mImage->width(), CV_8UC1);
	cvtColor(image, imageGray, CV_RGB2GRAY);
	
	vector<KeyPoint> keypoints;
	float time = getTickCount();
	Ptr<Feature2D> feature = SURF::create(threshold, nOctaves, nOctaveLayers, false, false);
	feature->detect(imageGray, keypoints);
	
	mImageTime = mLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2).append(" ms");
	mImageKeypoints = mLocale->toString((float) keypoints.size(),'f', 0).append(" keypoints");

	QPoint center;
	int radius;
	mPainter->begin(&mPixmap);
	mPainter->setRenderHint(QPainter::Antialiasing);
	for (int n=0; n<keypoints.size(); ++n) {
		center.setX((int) (keypoints.at(n).pt.x));
		center.setY((int) (keypoints.at(n).pt.y));
		radius = (int) (keypoints.at(n).size); // radius = (int)(keypoints->at(n).size*1.2/9.*2); = 0.266666
		if (showOrientation) {
			mPainter->setPen(QColor::fromRgb(255, 0, 0));
			mPainter->drawLine(QLineF(keypoints.at(n).pt.x,
					keypoints.at(n).pt.y,
					keypoints.at(n).pt.x + keypoints.at(n).size*qCos(keypoints.at(n).angle*3.14159265/180),
					keypoints.at(n).pt.y + keypoints.at(n).size*qSin(keypoints.at(n).angle*3.14159265/180)));
		}
		mPainter->setPen(QColor::fromRgb(0, 0, 255));
		mPainter->drawEllipse(center, radius, radius);
	}
	mPainter->end();
	
	mModified = true;
	myLabelImage->setPixmap(mPixmap.scaled(currentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




void WindowImage::resetImage() {
	mPixmap = mPixmapOriginal;
	mImageTime.clear();
	mImageKeypoints.clear();
	mModified = false;
	myLabelImage->setPixmap(mPixmap.scaled(currentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




// http://stackoverflow.com/questions/5026965/how-to-convert-an-opencv-cvmat-to-qimage
QImage WindowImage::convertMat2QImage(const cv::Mat_<double> &src) {
        double scale = -255.0;
        QImage dest(src.cols, src.rows, QImage::Format_RGB32);
        for (int y = 0; y < src.rows; ++y) {
                const double *srcrow = src[y];
                QRgb *destrow = (QRgb*) dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x) {
                        unsigned int color = srcrow[x] * scale;
                        destrow[x] = qRgb(color, color, color);
                }
        }
        return dest;
}




void WindowImage::scaleImage() {
	qDebug() << "Factor increment: " << mFactorIncrement;
	qDebug() << "Current factor: " << currentFactor;
	qDebug() << "";
// 	myLabelImage->resize(currentFactor*originalSize);
	myLabelImage->setPixmap(mPixmap.scaled(currentFactor*mOriginalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	adjustScrollBar(scrollArea->horizontalScrollBar());
	adjustScrollBar(scrollArea->verticalScrollBar());
	mImageZoom = tr("%1%").arg((int)(currentFactor*100));
}




void WindowImage::adjustScrollBar(QScrollBar* scrollBar) {
	scrollBar->setValue(int(mFactorIncrement*scrollBar->value()+(mFactorIncrement-1)*scrollBar->pageStep()/2));
}




void WindowImage::mousePressEvent(QMouseEvent* event) {
	mLastPoint = event->pos();
	setCursor(Qt::ClosedHandCursor);
}




void WindowImage::mouseMoveEvent(QMouseEvent* event) {
	QPoint myPos = event->pos();
	int hValue = scrollArea->horizontalScrollBar()->value();
	int vValue = scrollArea->verticalScrollBar()->value();
	scrollArea->horizontalScrollBar()->setValue(hValue+(mLastPoint.x()-myPos.x()));
	scrollArea->verticalScrollBar()->setValue(vValue+(mLastPoint.y()-myPos.y()));
	mLastPoint = myPos;
}




void WindowImage::mouseReleaseEvent(QMouseEvent* event) {
	unsetCursor();
}




// void windowImage::mouseDoubleClickEvent(QMouseEvent* event) {
// 	if (event->button()==Qt::LeftButton) zoomIn();
// 	else zoomOut();
// }
