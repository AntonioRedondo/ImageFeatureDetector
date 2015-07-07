/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWSTARTUP_H
#define WINDOWSTARTUP_H

#include "ui_startupWindow.h"
#include "windowMain.h"

class WindowMain; // http://stackoverflow.com/questions/2133250/does-not-name-a-type-error-in-c

class WindowStartup : public QDialog, private Ui::startupWindow {
  Q_OBJECT
  public:
	WindowStartup(WindowMain* windowMain);

  private:
	WindowMain* mWindowMain;
	QSettings* mSettings;

  private slots:
	void open();
	void captureWebcam();
	void saveSettings();
};

#endif