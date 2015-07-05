#include "windowStartup.h"


WindowStartup::WindowStartup(WindowMain* windowMain)
		: mWindowMain(windowMain), QDialog::QDialog(windowMain, Qt::Dialog) {
	setupUi(this);

	mSettings = new QSettings("imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	
	uiCheckBoxStartup->setChecked(mSettings->value("startupDialog", true).toBool());
	if (mSettings->value("recentFiles").toStringList().isEmpty()) {
		uiToolButtonOpenRecent->setEnabled(false);
		uiToolButtonOpenRecent->setText("There Is No Recent Files");
	}

	QMenu* recentFiles = mWindowMain->mMenuRecentFiles;
	uiToolButtonOpenRecent->setMenu(recentFiles);	
	
	connect(uiCommandLinkButtonOpen, &QAbstractButton::clicked, this, &WindowStartup::open);
	connect(uiCommandLinkButtonCaptureWebcam, &QAbstractButton::clicked, this, &WindowStartup::captureWebcam);
	connect(uiCheckBoxStartup, &QAbstractButton::clicked, this, &WindowStartup::saveSettings);
	connect(recentFiles, &QMenu::triggered, this, &WindowStartup::close);
	connect(recentFiles, &QMenu::aboutToHide, this, &WindowStartup::close);

	show();
}




void WindowStartup::open() {
	close();
	mWindowMain->open();
}




void WindowStartup::captureWebcam() {
	mWindowMain->captureWebcam();
	close();
}




void WindowStartup::saveSettings() {
	mSettings->setValue("startupDialog", uiCheckBoxStartup->isChecked());
}