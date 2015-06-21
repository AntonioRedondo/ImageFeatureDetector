/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWCAPTUREWEBCAM_H
#define WINDOWCAPTUREWEBCAM_H

#include <opencv2/opencv.hpp>
#include "ui_captureWebcamWindow.h"
#include "windowMain.h"
#include "windowImage.h"

using namespace cv;
using namespace std;

/**
       \brief
       @author authorname
*/
class WindowCaptureWebcam : public QDialog, private Ui::captureWebcamWindow {
  Q_OBJECT
  public:
	WindowCaptureWebcam(QWidget* _widget);
	void closeEvent(QCloseEvent* _event);
	
	QWidget* myWidget;
	
  private:
	CvCapture* myCamera;
	QTimer* myTimer;
	IplImage* myIplImageRealTime;
	IplImage* myIplImage320;
	QImage* myImage;
// 	VideoCapture myCamera;
// 	Mat myImage;

  private slots:
	void capture();
	void ok();
	void compute();
	void close();
};

#endif