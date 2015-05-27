/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWABOUT_H
#define WINDOWABOUT_H

#include <QtGui/QApplication>
#include "ui_aboutWindow.h"

/**
       \brief
       @author authorname
*/
class WindowAbout : public QDialog, private Ui::aboutWindow {
  Q_OBJECT
  public:
	WindowAbout();
	WindowAbout(QWidget* widgetConstr) : QDialog(widgetConstr, Qt::Dialog) {
		setupUi(this);
// 		setWindowFlags(Qt::Dialog);
		show();
	}
};

#endif