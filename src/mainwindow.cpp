#include "mainwindow.h"
#include "gameboard.h"
#include "mineCounter.h"

#include <QVBoxLayout>
#include <QFrame>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainFrame(nullptr)
{
	setupStateMachine();
	initialize();
}

void MainWindow::initialize()
{
	delete mainFrame;

	mainFrame = new QFrame;
	auto mainFrameLayout = new QVBoxLayout;
	auto infoLayout = new QHBoxLayout;
	gameBoard = new GameBoard(16, 30, 99);
	mineCounter = new MineCounter;

	mineCounter->setNumMines(99);

	connect(gameBoard, &GameBoard::initialized, [this](Tile* first)
	{
		firstClicked = first;
		emit startGame();
	});
	connect(gameBoard, &GameBoard::flagCountChanged, mineCounter, &MineCounter::setFlagCount);
	connect(gameBoard, &GameBoard::victory,this, &MainWindow::victory);
	connect(gameBoard, &GameBoard::defeat, this, &MainWindow::defeat);

	infoLayout->addWidget(mineCounter);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

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
	gameOverState = new QState;

	victoryState = new QState;
	defeatState = new QState;

	m_machine->addState(unstartedState);
	m_machine->addState(inProgressState);
	m_machine->addState(gameOverState);
	m_machine->addState(victoryState);
	m_machine->addState(defeatState);

	m_machine->setInitialState(unstartedState);

	unstartedState->addTransition(this, &MainWindow::startGame, inProgressState);

	inProgressState->addTransition(this, &MainWindow::victory, victoryState);
	inProgressState->addTransition(this, &MainWindow::defeat, defeatState);

	connect(inProgressState, &QState::entered, [this]()
	{
		
	});

	connect(victoryState, &QState::entered, [this]()
	{

	});

	connect(defeatState, &QState::entered, [this]()
	{
		
	});

	m_machine->start();
}

