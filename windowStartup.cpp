#include "windowStartup.h"


WindowStartup::WindowStartup(QWidget* widgetConstr) : myWidget(widgetConstr), QDialog::QDialog(widgetConstr, Qt::Dialog) {
	setupUi(this);

	mySettings = new QSettings("imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	
	checkBoxStartup->setChecked(mySettings->value("startupDialog", true).toBool());
	if (mySettings->value("recentFiles").toStringList().isEmpty()) {
		toolButtonOpenRecent->setEnabled(false);
		toolButtonOpenRecent->setText("There Is No Recent Files");
	}

	QMenu* myMenuRecentFiles = qobject_cast<WindowMain*>(myWidget)->menuRecentFiles;
	toolButtonOpenRecent->setMenu(myMenuRecentFiles);
	connect(commandLinkButtonOpen, SIGNAL(clicked()), this, SLOT(open()));
	connect(commandLinkButtonCaptureWebcam, SIGNAL(clicked()), this, SLOT(captureWebcam()));
	connect(checkBoxStartup, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(myMenuRecentFiles, SIGNAL(triggered(QAction*)), this, SLOT(close()));
	connect(myMenuRecentFiles, SIGNAL(aboutToHide()), this, SLOT(close()));

	show();
}




void WindowStartup::open() {
	close();
	qobject_cast<WindowMain*>(myWidget)->open();
}




void WindowStartup::captureWebcam() {
	qobject_cast<WindowMain*>(myWidget)->captureWebcam();
	close();
}




void WindowStartup::saveSettings() {
	mySettings->setValue("startupDialog", checkBoxStartup->isChecked());
}