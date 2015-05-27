/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWIMAGE_H
#define WINDOWIMAGE_H

#include <QtGui>
#include <QtCore/qmath.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include "ui_imageWindow.h"

using namespace cv;
using namespace std;

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
	void harris(int sobelApertureSize, int harrisApertureSize, double kValue);
	void fast(int threshold, bool nonmax_suppression);
	void sift(double _threshold, double _edgeThreshold, int _nOctaves, int _nOctaveLayers, bool _showOrientation);
	void surf(double threshold, int nOctaves, int nOctaveLayers, int extended, bool showOrientation);

	enum windowTitleType {normal=0, duplicated=1, capturedWebcam=2, capturedRoboLab=3};
	enum featureType {none=0, harrisType=1, fastType=2, siftType=3, surfType=4};
	QImage* myImage;
	QImage myImageOriginal;
	QPixmap myPixmap;
	QString myImageZoom;
	QString myImageTime;
	QString myImageKeypoints;
	QString myImageDimensions;
	QString myImageSize;
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

	QPixmap myPixmapOriginal;
	QSize originalSize;
	QPoint lastPoint;
	QLocale* myLocale;
	QPainter* myPainter;
// 	QLabel* myLabelImage;
// 	QScrollArea* mySrollArea;
	bool modified;
	int originalWidth, originalHeight;
	float scaleFactorAbove100, scaleFactorUnder100, factorIncrement;
};

#endif