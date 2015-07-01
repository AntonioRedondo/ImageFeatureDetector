/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWFASTREALTIME_H
#define WINDOWFASTREALTIME_H

#include <qt5/QtWidgets/QtWidgets>
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

	QWidget* mWidget;

  private:
	QSettings* mSettings;
	QLocale* mLocale;
	CvCapture* mCamera;
	QTimer* mTimer;
	QPixmap mPixmap;
	QImage* mImage;
	QPainter* mPainter;
	bool mDetecting;
	IplImage* mIplImageRealTime;
	IplImage* mIplImage320;
	IplImage* mIplImage320Gray;
// 	Mat myIplImage320;
// 	Mat myIplImage320Gray;
	vector<KeyPoint> mKeypoints;
	float mTime;

  private slots:
	void detect();
	void compute();
	void close();
	void saveFastParams();
	void resetFastParams();
};

#endif