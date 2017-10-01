#include "mainwindow.h"
#include "gameboard.h"
#include "mineCounter.h"
#include "minetimer.h"

#include <QDebug>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFrame>
#include <QTimer>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainFrame(nullptr)
{
	this->setWindowIcon(QIcon(":/mine"));
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
	setupStateMachine();
	setupMenus();
}

void MainWindow::initialize()
{
	delete mainFrame;

	mainFrame = new QFrame(this);
	auto mainFrameLayout = new QVBoxLayout;
	auto infoLayout = new QHBoxLayout;
	gameBoard = new GameBoard(numRows, numCols, numMines, mainFrame);
	mineCounter = new MineCounter(mainFrame);
	mineTimer = new MineTimer(mainFrame);
	newGame = new QPushButton(mainFrame);
	gameClock = new QTimer(this);

	mineCounter->setNumMines(numMines);

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
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
	infoLayout->addWidget(newGame);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
	infoLayout->addWidget(mineTimer);

	mainFrameLayout->addLayout(infoLayout);
	mainFrameLayout->addWidget(gameBoard);

	mainFrame->setLayout(mainFrameLayout);

	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
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

void MainWindow::setupMenus()
{
	gameMenu = new QMenu("Game");

	newGameAction = new QAction("New Game");
	newGameAction->setShortcut(QKeySequence(Qt::Key_F2));
	connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);

	difficultyMenu = new QMenu("Difficulty");
	difficultyActionGroup = new QActionGroup(difficultyMenu);

	beginnerAction = new QAction("Beginner", difficultyActionGroup);
	beginnerAction->setCheckable(true);
	connect(beginnerAction, &QAction::triggered, [this]()
	{
		numRows = 9;
		numCols = 9;
		numMines = 10;
		initialize();
		adjustSize();
	});

	intermediateAction = new QAction("Intermediate", difficultyActionGroup);
	intermediateAction->setCheckable(true);
	connect(intermediateAction, &QAction::triggered, [this]()
	{
		numRows = 16;
		numCols = 16;
		numMines = 40;
		initialize();
		adjustSize();
	});

	expertAction = new QAction("Expert", difficultyActionGroup);
	expertAction->setCheckable(true);
	connect(expertAction, &QAction::triggered, [this]()
	{
		numRows = 16;
		numCols = 30;
		numMines = 99;
		initialize();
		adjustSize();
	});

	difficultyMenu->addAction(beginnerAction);
	difficultyMenu->addAction(intermediateAction);
	difficultyMenu->addAction(expertAction);

	gameMenu->addAction(newGameAction);
	gameMenu->addSeparator();
	gameMenu->addMenu(difficultyMenu);

	this->menuBar()->addMenu(gameMenu);

	expertAction->trigger();
}
