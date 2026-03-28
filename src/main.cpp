#include <QApplication>
#include <QGuiApplication>
#include <QIcon>
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
	app.setWindowIcon(QIcon(":/mine"));
#if defined(Q_OS_LINUX)
	QGuiApplication::setDesktopFileName("minesweeper");
#endif

	MainWindow w;
	w.show();

	return app.exec();
}
