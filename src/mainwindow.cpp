#include "mainwindow.h"
#include "gameboard.h"
#include "mineCounter.h"
#include "minetimer.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QFrame>
#include <QTimer>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainFrame(nullptr)
{
	this->setWindowIcon(QIcon(":/mine"));
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
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
	mineTimer = new MineTimer(mainFrame);
	newGame = new QPushButton(mainFrame);
	gameClock = new QTimer(this);

	mineCounter->setNumMines(99);

	connect(gameBoard, &GameBoard::initialized, this, &MainWindow::startGame);
	connect(gameBoard, &GameBoard::flagCountChanged, mineCounter, &MineCounter::setFlagCount);
	connect(gameBoard, &GameBoard::victory, this, &MainWindow::victory);
	connect(gameBoard, &GameBoard::defeat, this, &MainWindow::defeat);

	newGame->setMinimumSize(35, 35);
	newGame->setIconSize(QSize(30, 30));
	newGame->setIcon(QIcon(":/emoji/smile"));
	connect(newGame, &QPushButton::clicked, this, &MainWindow::startNewGame);
	connect(this, &MainWindow::defeat, [this]()
	{
		newGame->setIcon(QIcon(":/emoji/injured"));
	});
	connect(this, &MainWindow::victory, [this]()
	{
		newGame->setIcon(QIcon(":/emoji/sunglasses"));
	});

	gameClock->setInterval(1000);
	connect(gameClock, &QTimer::timeout, mineTimer, &MineTimer::incrementTime);

	infoLayout->addWidget(mineCounter);;
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	infoLayout->addWidget(newGame);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	infoLayout->addWidget(mineTimer);

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
		gameClock->start();
	});

	connect(victoryState, &QState::entered, [this]()
	{
		gameClock->stop();
	});

	connect(defeatState, &QState::entered, [this]()
	{
		gameClock->stop();
	});

	m_machine->addState(unstartedState);
	m_machine->addState(inProgressState);
	m_machine->addState(victoryState);
	m_machine->addState(defeatState);

	m_machine->setInitialState(unstartedState);
	m_machine->start();
}

