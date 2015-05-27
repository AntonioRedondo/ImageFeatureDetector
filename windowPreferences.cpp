#include "windowPreferences.h"


WindowPreferences::WindowPreferences(QWidget* widgetConstr) : myWidget(widgetConstr), QDialog::QDialog(widgetConstr, Qt::Dialog) {
	setupUi(this);

	mySettings = new QSettings("./imageFeatureDetectorSettings.ini", QSettings::IniFormat);
	if (mySettings->value("recentFiles").toStringList().isEmpty()) {
		myPushButtonClearRecentFiles->setEnabled(false);
		myPushButtonClearRecentFiles->setText("Recent Files List Is Empty");
	}
	myCheckBoxStartupDialog->setChecked(mySettings->value("startupDialog", true).toBool());
	myCheckBoxBestFit->setChecked(mySettings->value("bestFit", true).toBool());
	myCheckBoxRecentFiles->setChecked(mySettings->value("rememberRecentFiles", true).toBool());
// 	myComboBoxLanguage->setCurrentIndex(mySettings->value("language", Worker::english).toInt());

	QPushButton* myPushButtonRestore = myButtonBox->button(QDialogButtonBox::RestoreDefaults);
	connect(myPushButtonClearRecentFiles, SIGNAL(clicked()), this, SLOT(clearRecentFilesQuestion()));
	connect(myPushButtonRestore, SIGNAL(clicked()), this, SLOT(restore()));
	connect(myButtonBox, SIGNAL(accepted()), this, SLOT(save()));
	connect(myButtonBox, SIGNAL(rejected()), this, SLOT(close()));

	show();
}




void WindowPreferences::clearRecentFilesQuestion() {
	if (QMessageBox::Yes == QMessageBox::warning(this, tr("Image Feature Detector"),
		tr("Do you want to clear the Recent Files List?"),
		QMessageBox::Yes | QMessageBox::Cancel)) clearRecentFiles();
}




void WindowPreferences::clearRecentFiles() {
	QStringList files = mySettings->value("recentFiles").toStringList();
	files.clear();
	mySettings->setValue("recentFiles", files);
	myPushButtonClearRecentFiles->setEnabled(false);
	myPushButtonClearRecentFiles->setText("Recent File List Cleared");
	qobject_cast<WindowMain*>(myWidget)->updateRecentFilesMenu();
}




void WindowPreferences::restore() {
	myCheckBoxStartupDialog->setChecked(true);
	myCheckBoxBestFit->setChecked(true);
	myCheckBoxRecentFiles->setChecked(true);
// 	myComboBoxLanguage->setCurrentIndex(Worker::english);
}




void WindowPreferences::save() {
	if (!myCheckBoxRecentFiles->isChecked()) clearRecentFiles();
	mySettings->setValue("rememberRecentFiles", myCheckBoxRecentFiles->isChecked());
	mySettings->setValue("bestFit", myCheckBoxBestFit->isChecked());
	mySettings->setValue("startupDialog", myCheckBoxStartupDialog->isChecked());
// 	mySettings->setValue("language", myComboBoxLanguage->currentIndex());
	close();
}