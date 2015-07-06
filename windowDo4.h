/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWDO4_H
#define WINDOWDO4_H

#include "ui_do4Window.h"
#include "windowImage.h"

using namespace std;

/**
       \brief
       @author authorname
*/
class WindowDo4 : public QWidget, Ui::do4Window {
  Q_OBJECT
  public:
	WindowDo4(QString windowTitle, QSettings* settings, WindowImage* harrisImage, WindowImage* fastImage, WindowImage* siftImage, WindowImage* surfImage);

  private:
	void changeEvent(QEvent* event);
	
	WindowImage* mHarrisImage;
	WindowImage* mFastImage;
	WindowImage* mSiftImage;
	WindowImage* mSurfImage;
	
	QTimer* mTimer;

  private slots:
	void zoomBestFit();
};

#endif