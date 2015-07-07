/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWFASTREALTIME_H
#define WINDOWFASTREALTIME_H

#include <qt5/QtWidgets/QtWidgets>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "ui_fastRealTimeWindow.h"
#include "windowMain.h"

using namespace cv;
using namespace std;

class WindowFastRealTime : public QDialog, Ui::fastRealTimeWindow {
  Q_OBJECT
  public:
	WindowFastRealTime(WindowMain* windowMain);
	void closeEvent(QCloseEvent* closeEvent);

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