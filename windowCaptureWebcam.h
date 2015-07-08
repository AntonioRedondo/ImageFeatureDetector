/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWCAPTUREWEBCAM_H
#define WINDOWCAPTUREWEBCAM_H

#include <opencv2/opencv.hpp>
#include "ui_captureWebcamWindow.h"
#include "windowMain.h"
#include "windowImage.h"

using namespace cv;

class WindowMain; // http://stackoverflow.com/questions/2133250/does-not-name-a-type-error-in-c

class WindowCaptureWebcam : public QDialog, private Ui::captureWebcamWindow {
  Q_OBJECT
  public:
	WindowCaptureWebcam(WindowMain*);
	void closeEvent(QCloseEvent*);
	
	WindowMain* mWindowMain;
	
  private:
	QTimer* mTimer;
	VideoCapture mCamera;
	Mat mImageRT;
	QImage* mImage;

  private slots:
	void capture();
	void ok();
	void compute();
	void close();
};

#endif