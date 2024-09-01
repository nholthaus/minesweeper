#include <QApplication>
#include <QStyleFactory>

#include "mainwindow.h"
#include "highScore.h"
#include "highScoreModel.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("Menari Softworks");
	QCoreApplication::setOrganizationDomain("github.com/nholthaus");
	QCoreApplication::setApplicationName("minesweeper");

	MainWindow w;
	w.show();

	return app.exec();
}