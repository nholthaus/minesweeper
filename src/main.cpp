#include <QApplication>
#include <QStyleFactory>

// Use the extended MainWindow with all new features
// Uncomment the line below to use the original MainWindow
// #define USE_ORIGINAL_MAINWINDOW

#ifdef USE_ORIGINAL_MAINWINDOW
#include "mainwindow.h"
#else
#include "mainwindow_ext.h"
#endif

#include "highScore.h"
#include "highScoreModel.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("Menari Softworks");
	QCoreApplication::setOrganizationDomain("github.com/nholthaus");
	QCoreApplication::setApplicationName("minesweeper");

#ifdef USE_ORIGINAL_MAINWINDOW
	MainWindow w;
#else
	MainWindowExt w;
#endif
	w.show();

	return app.exec();
}