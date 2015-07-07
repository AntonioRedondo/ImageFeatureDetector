/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWDO4_H
#define WINDOWDO4_H

#include "ui_do4Window.h"
#include "windowImage.h"

using namespace std;

class WindowDo4 : public QWidget, Ui::do4Window {
  Q_OBJECT
  public:
	WindowDo4(QString windowTitle, WindowImage* harrisImage, WindowImage* fastImage, WindowImage* siftImage, WindowImage* surfImage);

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