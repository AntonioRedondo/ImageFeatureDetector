/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWABOUT_H
#define WINDOWABOUT_H

#include "ui_windowAbout.h"

class WindowAbout : public QDialog, private Ui::windowAbout {
  Q_OBJECT
  public:
	WindowAbout();
	WindowAbout(QWidget* windowMain) : QDialog(windowMain, Qt::Dialog) {
		setupUi(this);
// 		setWindowFlags(Qt::Dialog);
		show();
	}
};

#endif