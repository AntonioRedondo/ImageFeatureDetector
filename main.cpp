/*
 * File:   main.cpp
 * Author: chucknorris
 *
 * Created on 05 December 2011, 03:31
 */

#include <QApplication>
#include "windowMain.h"

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);
	new WindowMain();
	return app.exec();
}