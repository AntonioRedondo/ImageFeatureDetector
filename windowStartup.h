/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWSTARTUP_H
#define WINDOWSTARTUP_H

#include "ui_startupWindow.h"
#include "windowMain.h"

/**
       \brief
       @author authorname
*/
class WindowStartup : public QDialog, private Ui::startupWindow {
  Q_OBJECT
  public:
	WindowStartup(QWidget* parentWidget);

  private:
	QWidget* mParentWidget;
	QSettings* mSettings;

  private slots:
	void open();
	void captureWebcam();
	void saveSettings();
};

#endif