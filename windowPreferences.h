/*
* 2010-2015 (C) Antonio Redondo
* http://antonioredondo.com
* https://github.com/AntonioRedondo/ImageFeatureDetector
*
* Code under the terms of the GNU General Public License v3.
*
*/

#ifndef WINDOWPREFERENCES_H
#define WINDOWPREFERENCES_H

#include "ui_preferencesWindow.h"
#include "windowMain.h"

class WindowMain; // http://stackoverflow.com/questions/2133250/does-not-name-a-type-error-in-c

class WindowPreferences : public QDialog, private Ui::preferencesWindow {
  Q_OBJECT
  public:
	WindowPreferences(WindowMain* windowMain);

  private:
	WindowMain* mWindowMain;
	QSettings* mSettings;

  private slots:
	void clearRecentFilesPrompt();
	void clearRecentFiles();
	void restore();
	void save();
};

#endif