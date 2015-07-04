#include "windowPreferences.h"


WindowPreferences::WindowPreferences(QWidget* parentWidget)
		: mWidget(parentWidget), QDialog::QDialog(parentWidget, Qt::Dialog) {
	setupUi(this);

	mSettings = new QSettings("./imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	if (mSettings->value("recentFiles").toStringList().isEmpty()) {
		uiPushButtonClearRecentFiles->setEnabled(false);
		uiPushButtonClearRecentFiles->setText("Recent Files List Is Empty");
	}
	uiCheckBoxStartupDialog->setChecked(mSettings->value("startupDialog", true).toBool());
	uiCheckBoxBestFit->setChecked(mSettings->value("bestFit", true).toBool());
	uiCheckBoxRecentFiles->setChecked(mSettings->value("rememberRecentFiles", true).toBool());
	
	connect(uiPushButtonClearRecentFiles, &QAbstractButton::clicked, this, &WindowPreferences::clearRecentFilesPrompt);
	connect(uiDialogButtonBox->button(QDialogButtonBox::RestoreDefaults), &QAbstractButton::clicked, this, &WindowPreferences::restore);
	connect(uiDialogButtonBox, &QDialogButtonBox::accepted, this, &WindowPreferences::save);
	connect(uiDialogButtonBox, &QDialogButtonBox::rejected, this, &WindowPreferences::close);

	show();
}




void WindowPreferences::clearRecentFilesPrompt() {
	if (QMessageBox::Yes == QMessageBox::warning(this, tr("Image Feature Detector"),
			tr("Do you want to clear the Recent Files List?"),
			QMessageBox::Yes | QMessageBox::Cancel))
		clearRecentFiles();
}




void WindowPreferences::clearRecentFiles() {
	QStringList files = mSettings->value("recentFiles").toStringList();
	files.clear();
	mSettings->setValue("recentFiles", files);
	uiPushButtonClearRecentFiles->setEnabled(false);
	uiPushButtonClearRecentFiles->setText("Recent File List Cleared");
	qobject_cast<WindowMain*>(mWidget)->updateRecentFilesMenu();
}




void WindowPreferences::restore() {
	uiCheckBoxStartupDialog->setChecked(true);
	uiCheckBoxBestFit->setChecked(true);
	uiCheckBoxRecentFiles->setChecked(true);
}




void WindowPreferences::save() {
	if (!uiCheckBoxRecentFiles->isChecked())
		clearRecentFiles();
	mSettings->setValue("rememberRecentFiles", uiCheckBoxRecentFiles->isChecked());
	mSettings->setValue("bestFit", uiCheckBoxBestFit->isChecked());
	mSettings->setValue("startupDialog", uiCheckBoxStartupDialog->isChecked());
	close();
}