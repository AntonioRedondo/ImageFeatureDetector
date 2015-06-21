/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
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

/**
       \brief
       @author authorname
*/
class WindowImage : public QWidget, Ui::imageWindow {
  Q_OBJECT
  public:
	WindowImage(QImage* imageConstr, QString windowTitleConstr, int titleTypeConstr=normal, int imageNConstr=1);
	void zoomIn();
	void zoomOut();
	void zoomOriginal();
	void zoomBestFit();
	void resetImage();
	void applyHarris(int sobelApertureSize, int harrisApertureSize, double kValue);
	void applyFast(int threshold, bool nonmax_suppression);
	void applySift(double _threshold, double _edgeThreshold, int _nOctaves, int _nOctaveLayers, bool _showOrientation);
	void applySurf(double threshold, int nOctaves, int nOctaveLayers, int extended, bool showOrientation);

	enum windowTitleType {normal=0, duplicated=1, capturedWebcam=2, capturedRoboLab=3};
	enum featureType {none=0, harrisType=1, fastType=2, siftType=3, surfType=4};
	QImage* mImage;
	QImage mImageOriginal;
	QPixmap mPixmap;
	QString mImageZoom;
	QString mImageTime;
	QString mImageKeypoints;
	QString mImageDimensions;
	QString mImageSize;
	QString windowImageTitle;
	int titleType, imageN, feature;
	float currentFactor;

  protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
// 	void mouseDoubleClickEvent(QMouseEvent* event);

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
// 	QLabel* myLabelImage;
// 	QScrollArea* mySrollArea;
	bool mModified;
	int mOriginalWidth, mOiginalHeight;
	float mScaleFactorAbove100, mScaleFactorUnder100, mFactorIncrement;
};

#endif