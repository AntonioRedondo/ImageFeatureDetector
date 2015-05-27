/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWFASTREALTIME_H
#define WINDOWFASTREALTIME_H

#include <QtGui>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "ui_fastRealTimeWindow.h"

using namespace cv;
using namespace std;

/**
       \brief
       @author authorname
*/
class WindowFastRealTime : public QDialog, Ui::fastRealTimeWindow {
  Q_OBJECT
  public:
	WindowFastRealTime(QWidget* _widget);
	void closeEvent(QCloseEvent* _event);

	QWidget* myWidget;

  private:
	QSettings* mySettings;
	QLocale* myLocale;
	CvCapture* myCamera;
	QTimer* myTimer;
	IplImage* myIplImageRealTime;
	QPixmap myPixmap;
	QImage* myImage;
	QPainter* myPainter;
	bool detecting;
	IplImage* myIplImage320;
	IplImage* myIplImage320Gray;
	vector<KeyPoint> myKeypoints;
	float time;

  private slots:
	void detect();
	void compute();
	void close();
	void fastSaveParams();
	void fastReset();
};

#endif