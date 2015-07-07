/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#include "windowImage.h"

WindowImage::WindowImage(QImage* image, QString windowTitle, int windowType, int imageN, QString originalTitle)
		: mImage(image), mWindowTitle(windowTitle), mWindowType(windowType), nImageN(imageN), mOriginalTitle(originalTitle) {
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	mPainter = new QPainter();
	mLocale = new QLocale(QLocale::English);
	
	setObjectName(windowTitle);
	setWindowTitle(mWindowTitle);
	if (mOriginalTitle == 0)
		mOriginalTitle = mWindowTitle;

	uiScrollAreaWidgetContents->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	mPixmap = QPixmap::fromImage(*mImage);
	if (mWindowType==fromWebcam) {
		// Whit this we redirect the pointer mImage to a new perdurable variable.
		mImageOriginal = new QImage(mPixmap.toImage());
		mImage = mImageOriginal;
	}
	mPixmapOriginal = mPixmap;
	uiLabelImage->setPixmap(mPixmap);
	mModified = false;

	mScaleFactorAbove100 = 0.5;
	mScaleFactorUnder100 = 0.25;
	mFactorIncrement = 0;
	mCurrentFactor = 1.0;

	mOriginalSize = mImage->size();
	mOriginalWidth = mImage->width();
	mOiginalHeight = mImage->height();

	mImageZoom = tr("%1%").arg((int)(mCurrentFactor*100));
	mImageDimensions = tr("%1x%2 px").arg(mOriginalWidth).arg(mOiginalHeight);
	float sizeInKiB = mImage->byteCount()/(float)1024;
	if (sizeInKiB > 1024)
		mImageSize = mLocale->toString(sizeInKiB/(float)1024,'f', 2).append(" MiB");
	else mImageSize = mLocale->toString(sizeInKiB,'f', 2).append(" KiB");
}




void WindowImage::zoomIn() {
	if (mCurrentFactor>=1.0) {
		mFactorIncrement = (mCurrentFactor+mScaleFactorAbove100)/mCurrentFactor;
		mCurrentFactor += mScaleFactorAbove100;
	} else {
		mFactorIncrement = (mCurrentFactor+mScaleFactorUnder100)/mCurrentFactor;
		mCurrentFactor += mScaleFactorUnder100;
	}
	scaleImage();
}




void WindowImage::zoomOut() {
	if (mCurrentFactor>1.0) {
		mFactorIncrement = (mCurrentFactor-mScaleFactorAbove100)/mCurrentFactor;
		mCurrentFactor -= mScaleFactorAbove100;
	} else {
		mFactorIncrement = (mCurrentFactor-mScaleFactorUnder100)/mCurrentFactor;
		mCurrentFactor -= mScaleFactorUnder100;
	}
	scaleImage();
}




void WindowImage::zoomBestFit() {
	float correctF = 0.99; // This correct factor allows the image fits the main window area without scrollbars.
	int scrollWidth = width();
	int scrollHeight = height();

	float relationScroll = scrollWidth/(float)scrollHeight;
	float relationImage = mOriginalWidth/(float)mOiginalHeight;

	float scaleWidth = scrollWidth/(float)mOriginalWidth;
	float scaleHeight = scrollHeight/(float)mOiginalHeight;

	if (relationScroll > relationImage) {
		if (correctF*scaleHeight>1.0)
			mFactorIncrement = correctF*scaleHeight/mCurrentFactor;
		else mFactorIncrement = correctF*scaleHeight/mCurrentFactor;
		mCurrentFactor = correctF*scaleHeight;
	} else {
		if (correctF*scaleWidth>1.0)
			mFactorIncrement = correctF*scaleWidth/mCurrentFactor;
		else mFactorIncrement = correctF*scaleWidth/mCurrentFactor;
		mCurrentFactor = correctF*scaleWidth;
	}
	
	scaleImage();
}




void WindowImage::zoomOriginal() {
	mFactorIncrement = 1/mCurrentFactor;
	mCurrentFactor = 1.0;
	scaleImage();
}




void WindowImage::applyHarris(int sobelApertureSize, int harrisApertureSize, double kValue) {
	Mat image(mImage->height(), mImage->width(), CV_8UC4, mImage->bits(), mImage->bytesPerLine()); // With CV_8UC3 it doesn't work
	Mat imageGray(mImage->height(), mImage->width(), CV_8UC1);
	cvtColor(image, imageGray, CV_RGB2GRAY);
	
	Mat imageHarris(mImage->height(), mImage->width(), CV_32FC1);
	float time = (float) getTickCount();
	cornerHarris(imageGray, imageHarris, harrisApertureSize, sobelApertureSize, kValue);
	
	mImageTime = mLocale->toString((float)((getTickCount()-time)/(getTickFrequency()*1000)),'f', 2);
	mImageKeypoints = "--";
	
	// Incrises the contrast. If not only an almost black image would be seen
	Mat imageHarris8U(mImage->height(), mImage->width(), CV_8UC1);
	double min=0, max=255, minVal, maxVal, scale, shift;
	minMaxLoc(imageHarris, &minVal, &maxVal);
	scale = (max-min)/(maxVal-minVal);
	shift = -minVal*scale+min;
	imageHarris.convertTo(imageHarris8U, CV_8UC1, scale, shift);
	
	mPixmap = QPixmap::fromImage(convertMat2QImage(imageHarris8U)); // This should be faster than the below three lines
// 	Mat imageColor(mImage->height(), mImage->width(), CV_8UC4); // With CV_8UC3 it doesn't work
// 	cvtColor(imageHarris8U, imageColor, CV_GRAY2RGBA); // With CV_GRAY2RGB it doesn't work
// 	mPixmap = QPixmap::fromImage(QImage(imageColor.data, mImage->width(), mImage->height(), imageColor.step, QImage::Format_RGB32)); // With Format_RGB888 it doesn't work. It can be Format_ARGB32 as well
	mModified = true;
	uiLabelImage->setPixmap(mPixmap);
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
	
	mImageTime = mLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2);
	mImageKeypoints = mLocale->toString((float)keypoints.size(),'f', 0);
	
	mPainter->begin(&mPixmap);
	mPainter->setPen(QColor::fromRgb(255, 0, 0));
	mPainter->setRenderHint(QPainter::Antialiasing);
	for (int n=0; n<keypoints.size(); ++n)
		mPainter->drawEllipse((int)keypoints.at(n).pt.x, (int)keypoints.at(n).pt.y, 3, 3);
	mPainter->end();
	
	mModified = true;
	uiLabelImage->setPixmap(mPixmap.scaled(mCurrentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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
	
	mImageTime = mLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2);
	mImageKeypoints = mLocale->toString((float)keypoints.size(),'f', 0);
	
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
	uiLabelImage->setPixmap(mPixmap.scaled(mCurrentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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
	
	mImageTime = mLocale->toString((float)((cvGetTickCount()-time)/(cvGetTickFrequency()*1000)),'f', 2);
	mImageKeypoints = mLocale->toString((float) keypoints.size(),'f', 0);

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
	uiLabelImage->setPixmap(mPixmap.scaled(mCurrentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}




void WindowImage::resetImage() {
	mPixmap = mPixmapOriginal;
	mImageTime.clear();
	mImageKeypoints.clear();
	mModified = false;
	uiLabelImage->setPixmap(mPixmap.scaled(mCurrentFactor*mOriginalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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
// 	uiLabelImage->resize(mCurrentFactor*originalSize);
	uiLabelImage->setPixmap(mPixmap.scaled(mCurrentFactor*mOriginalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	adjustScrollBar(horizontalScrollBar());
	adjustScrollBar(verticalScrollBar());
	mImageZoom = tr("%1%").arg((int)(mCurrentFactor*100));
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
	int hValue = horizontalScrollBar()->value();
	int vValue = verticalScrollBar()->value();
	horizontalScrollBar()->setValue(hValue+(mLastPoint.x()-myPos.x()));
	verticalScrollBar()->setValue(vValue+(mLastPoint.y()-myPos.y()));
	mLastPoint = myPos;
}




void WindowImage::mouseReleaseEvent(QMouseEvent* event) {
	unsetCursor();
}




void WindowImage::mouseDoubleClickEvent(QMouseEvent* event) {
	if (event->button()==Qt::LeftButton)
		zoomIn();
	else zoomOut();
}
