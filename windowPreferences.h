/* Copyright (C) 2010-2011 Antonio Redondo López.
 * This file is part of Image Feature Detector.
 * Source code published under the GNU GPL v3.
 * For further information visit http://code.google.com/p/image-feature-detector/
 */

#ifndef WINDOWPREFERENCES_H
#define WINDOWPREFERENCES_H

#include "ui_preferencesWindow.h"
#include "windowMain.h"

/**
       \brief
       @author authorname
*/
class WindowPreferences : public QDialog, private Ui::preferencesWindow {
  Q_OBJECT
  public:
	//If we pass diretcly a Worker object we get a "ISO C++ forbids declaration of ‘Worker’ with no type" error.
	WindowPreferences(QWidget* widgetConstr);

  private:
	QWidget* myWidget;
	QSettings* mySettings;

  private slots:
	void clearRecentFilesQuestion();
	void clearRecentFiles();
	void restore();
	void save();
};

#endif