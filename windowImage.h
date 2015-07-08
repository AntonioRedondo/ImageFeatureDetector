/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWIMAGE_H
#define WINDOWIMAGE_H

#include <QtWidgets>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include "ui_imageWindow.h"

using namespace std;
using namespace cv;
using namespace xfeatures2d;

class WindowImage : public QScrollArea, Ui::imageWindow {
  Q_OBJECT
  public:
	WindowImage(QImage* image, QString windowTitle, int windowType=normal);
	void zoomIn();
	void zoomOut();
	void zoomOriginal();
	void zoomBestFit();
	void resetImage();
	void applyHarris(int sobelApertureSize, int harrisApertureSize, double kValue);
	void applyFast(int threshold, bool nonMaxSuppression);
	void applySift(double threshold, double edgeThreshold, int nOctaves, int nOctaveLayers, bool showOrientation);
	void applySurf(double threshold, int nOctaves, int nOctaveLayers, int extended, bool showOrientation);

	enum windowType {normal=0, duplicated=1, fromWebcam=2, do4=3};
	enum featureType {none=0, harris=1, fast=2, sift=3, surf=4};
	QImage* mImage;
	QImage* mImageOriginal;
	QPixmap mPixmap;
	QString mImageZoom;
	QString mImageTime;
	QString mImageKeypoints;
	QString mImageDimensions;
	QString mImageSize;
	QString mWindowTitle;
	QString mUid;
	QString mOriginalUid;
	int mWindowType, mFeatureType, mImageN;
	float mCurrentFactor;

  protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);

  private:
	void scaleImage();
	void scrollImage(QPoint* pos);
	void adjustScrollBar(QScrollBar* scrollBar);
	QImage Mat2QImageRGB(const cv::Mat3b &src);
	QImage convertMat2QImage(const cv::Mat_<double> &src);

	QPixmap mPixmapOriginal;
	QSize mOriginalSize;
	QPoint mLastPoint;
	QLocale* mLocale;
	QPainter* mPainter;
	bool mModified;
	int mOriginalWidth, mOiginalHeight;
	float mScaleFactorAbove100, mScaleFactorUnder100, mFactorIncrement;
};

#endif