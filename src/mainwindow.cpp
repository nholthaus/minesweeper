#include "mainwindow.h"
#include "gameboard.h"
#include "mineCounter.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QFrame>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainFrame(nullptr)
{
	setupStateMachine();
}

void MainWindow::initialize()
{
	delete mainFrame;

	mainFrame = new QFrame(this);
	auto mainFrameLayout = new QVBoxLayout;
	auto infoLayout = new QHBoxLayout;
	gameBoard = new GameBoard(16, 30, 99, mainFrame);
	mineCounter = new MineCounter(mainFrame);
	newGame = new QPushButton(mainFrame);

	mineCounter->setNumMines(99);

	connect(gameBoard, &GameBoard::initialized, this, &MainWindow::startGame);
	connect(gameBoard, &GameBoard::flagCountChanged, mineCounter, &MineCounter::setFlagCount);
	connect(gameBoard, &GameBoard::victory, this, &MainWindow::victory);
	connect(gameBoard, &GameBoard::defeat, this, &MainWindow::defeat);

	newGame->setMinimumSize(40, 40);
	newGame->setIcon(QIcon(":/emoji/smile"));
	connect(newGame, &QPushButton::clicked, this, &MainWindow::startNewGame);
	connect(newGame, &QPushButton::clicked, []()
	{
		qDebug() << "new game clicked";
	});
	connect(this, &MainWindow::startNewGame, []()
	{
		qDebug() << "start new game";
	});

	infoLayout->addWidget(mineCounter);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	infoLayout->addWidget(newGame);

	mainFrameLayout->addLayout(infoLayout);
	mainFrameLayout->addWidget(gameBoard);

	mainFrame->setLayout(mainFrameLayout);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setCentralWidget(mainFrame);
}

void MainWindow::setupStateMachine()
{
	m_machine = new QStateMachine;

	unstartedState = new QState;
	inProgressState = new QState;
	victoryState = new QState;
	defeatState = new QState;

	unstartedState->addTransition(this, &MainWindow::startGame, inProgressState);

	inProgressState->addTransition(this, &MainWindow::victory, victoryState);
	inProgressState->addTransition(this, &MainWindow::defeat, defeatState);
	inProgressState->addTransition(this, &MainWindow::startNewGame, unstartedState);

	victoryState->addTransition(this, &MainWindow::startNewGame, unstartedState);

	defeatState->addTransition(this, &MainWindow::startNewGame, unstartedState);

	connect(unstartedState, &QState::entered, [this]()
	{
		initialize();
	});

	connect(inProgressState, &QState::entered, [this]()
	{
		qDebug() << "started a game";
	});

	connect(victoryState, &QState::entered, [this]()
	{
		qDebug() << "got victory";
	});

	connect(defeatState, &QState::entered, [this]()
	{
		qDebug() << "got defeat";
	});

	m_machine->addState(unstartedState);
	m_machine->addState(inProgressState);
	m_machine->addState(victoryState);
	m_machine->addState(defeatState);

	m_machine->setInitialState(unstartedState);
	m_machine->start();
}

