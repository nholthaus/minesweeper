#include "mainwindow.h"
#include "gameboard.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	this->setLayout(new QVBoxLayout);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setCentralWidget(new GameBoard(16, 30, 99));
}

