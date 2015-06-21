#include "windowStartup.h"


WindowStartup::WindowStartup(QWidget* widgetConstr) : mWidget(widgetConstr), QDialog::QDialog(widgetConstr, Qt::Dialog) {
	setupUi(this);

	mSettings = new QSettings("imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	
	checkBoxStartup->setChecked(mSettings->value("startupDialog", true).toBool());
	if (mSettings->value("recentFiles").toStringList().isEmpty()) {
		toolButtonOpenRecent->setEnabled(false);
		toolButtonOpenRecent->setText("There Is No Recent Files");
	}

	QMenu* recentFiles = qobject_cast<WindowMain*>(mWidget)->menuRecentFiles;
	toolButtonOpenRecent->setMenu(recentFiles);
	connect(commandLinkButtonOpen, SIGNAL(clicked()), this, SLOT(open()));
	connect(commandLinkButtonCaptureWebcam, SIGNAL(clicked()), this, SLOT(captureWebcam()));
	connect(checkBoxStartup, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(recentFiles, SIGNAL(triggered(QAction*)), this, SLOT(close()));
	connect(recentFiles, SIGNAL(aboutToHide()), this, SLOT(close()));

	show();
}




void WindowStartup::open() {
	close();
	qobject_cast<WindowMain*>(mWidget)->open();
}




void WindowStartup::captureWebcam() {
	qobject_cast<WindowMain*>(mWidget)->captureWebcam();
	close();
}




void WindowStartup::saveSettings() {
	mSettings->setValue("startupDialog", checkBoxStartup->isChecked());
}