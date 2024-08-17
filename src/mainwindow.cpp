#include "appinfo.h"
#include "mainwindow.h"
#include "gameboard.h"
#include "mineCounter.h"
#include "minetimer.h"
#include "highScoreDialog.h"
#include "highScoreModel.h"

#include <QDebug>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFrame>
#include <QInputDialog>
#include <QTimer>
#include <QStatusBar>
#include <QSettings>
#include <QScopedArrayPointer>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainFrame(nullptr)
{
	this->setWindowIcon(QIcon(":/mine"));
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
	setupStateMachine();
	setupMenus();
	loadSettings();

	connect(this, &MainWindow::defeat, this, [this]()
	{
		newGame->setIcon(QIcon(":/emoji/injured"));
	});
	connect(this, &MainWindow::victory, this, [this]()
	{
		newGame->setIcon(QIcon(":/emoji/sunglasses"));
	});

	this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void MainWindow::setDifficulty(HighScore::Difficulty difficulty)
{
	this->difficulty = difficulty;

	switch (difficulty)
	{
	case HighScore::beginner:
		numRows = 9;
		numCols = 9;
		numMines = 10;
		beginnerAction->setChecked(true);
		break;
	case HighScore::intermediate:
		numRows = 16;
		numCols = 16;
		numMines = 40;
		intermediateAction->setChecked(true);
		break;
	case HighScore::expert:
		numRows = 16;
		numCols = 30;
		numMines = 99;
		expertAction->setChecked(true);
		break;
	case HighScore::custom:
		break;
	default:
		break;
	}
	
	initialize();
	adjustSize();
}

void MainWindow::initialize()
{
	QFrame* newMainFrame = new QFrame(this);
	auto mainFrameLayout = new QVBoxLayout;
	auto infoLayout = new QHBoxLayout;
	gameBoard = new GameBoard(numRows, numCols, numMines, newMainFrame);
	mineCounter = new MineCounter(newMainFrame);
	mineTimer = new MineTimer(newMainFrame);
	newGame = new QPushButton(newMainFrame);
	gameClock = new QTimer(this);

	mineCounter->setNumMines(numMines);

	connect(gameBoard, &GameBoard::initialized, this, &MainWindow::startGame, Qt::UniqueConnection);
	connect(gameBoard, &GameBoard::flagCountChanged, mineCounter, &MineCounter::setFlagCount, Qt::UniqueConnection);
	connect(gameBoard, &GameBoard::victory, this, &MainWindow::victory, Qt::UniqueConnection);
	connect(gameBoard, &GameBoard::defeat, this, &MainWindow::defeat, Qt::UniqueConnection);

	newGame->setMinimumSize(35, 35);
	newGame->setIconSize(QSize(30, 30));
	newGame->setIcon(QIcon(":/emoji/smile"));
	connect(newGame, &QPushButton::clicked, this, &MainWindow::startNewGame, Qt::UniqueConnection);

	gameClock->setInterval(1000);
	connect(gameClock, &QTimer::timeout, mineTimer, &MineTimer::incrementTime, Qt::UniqueConnection);

	infoLayout->addWidget(mineCounter);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
	infoLayout->addWidget(newGame);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
	infoLayout->addWidget(mineTimer);

	mainFrameLayout->addLayout(infoLayout);
	mainFrameLayout->addWidget(gameBoard);

	newMainFrame->setLayout(mainFrameLayout);

	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	this->setCentralWidget(newMainFrame);

	std::swap(mainFrame, newMainFrame);
	delete newMainFrame;
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
		onVictory();
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

void MainWindow::onVictory()
{
	if (!m_highScores[difficulty])
		m_highScores[difficulty] = new HighScoreModel(difficulty);

	if (m_highScores[difficulty]->isHighScore(mineTimer->time()))
	{
		auto name = QInputDialog::getText(this, tr("Congratulations!"), tr("You've earned a high score!<br>Please enter your name:"));
		m_highScores[difficulty]->addHighScore(HighScore(name, difficulty, mineTimer->time(), QDateTime::currentDateTime()));
		highScoreAction->trigger();
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	saveSettings();
}

void MainWindow::setupMenus()
{
	gameMenu = new QMenu(tr("Game"));

	newGameAction = new QAction(tr("New Game"));
	newGameAction->setShortcut(QKeySequence(Qt::Key_F2));
	connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);

	difficultyMenu = new QMenu(tr("Difficulty"));
	difficultyActionGroup = new QActionGroup(difficultyMenu);

	beginnerAction = new QAction(tr("Beginner"), difficultyActionGroup);
	beginnerAction->setCheckable(true);
	QObject::connect(beginnerAction, &QAction::triggered, [this]()
	{
		setDifficulty(HighScore::beginner);
	});

	intermediateAction = new QAction(tr("Intermediate"), difficultyActionGroup);
	intermediateAction->setCheckable(true);
	connect(intermediateAction, &QAction::triggered, [this]()
	{
		setDifficulty(HighScore::intermediate);
	});

	expertAction = new QAction(tr("Expert"), difficultyActionGroup);
	expertAction->setCheckable(true);
	connect(expertAction, &QAction::triggered, [this]()
	{
		setDifficulty(HighScore::expert);
	});

	difficultyMenu->addAction(beginnerAction);
	difficultyMenu->addAction(intermediateAction);
	difficultyMenu->addAction(expertAction);

	highScoreAction = new QAction(tr("High Scores..."));
	connect(highScoreAction, &QAction::triggered, this, [this]()
	{
		HighScoreDialog* dialog = new HighScoreDialog(m_highScores, this);
		dialog->setActiveTab(QVariant::fromValue(difficulty).toString());
		dialog->exec();
		dialog->deleteLater();
	}, Qt::QueuedConnection);

	exitAction = new QAction(tr("Exit"));
	connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

	gameMenu->addAction(newGameAction);
	gameMenu->addSeparator();
	gameMenu->addMenu(difficultyMenu);
	gameMenu->addAction(highScoreAction);
	gameMenu->addSeparator();
	gameMenu->addAction(exitAction);

	helpMenu = new QMenu(tr("Help"));

	aboutAction = new QAction(tr("About..."));

	helpMenu->addAction(aboutAction);

	this->menuBar()->addMenu(gameMenu);
	this->menuBar()->addMenu(helpMenu);
}

void MainWindow::saveSettings()
{
	QSettings settings(APPINFO::organization, APPINFO::name);
	settings.setValue("difficulty", QVariant::fromValue(difficulty).toString());
	settings.beginWriteArray("High Scores", static_cast<int>(m_highScores.size()));
	int i = 0;
	for (const auto model : m_highScores)
	{
		settings.setArrayIndex(i++);
		settings.setValue("model", QVariant::fromValue(*model));
	}
	settings.endArray();
}

void MainWindow::loadSettings()
{
	QSettings settings(APPINFO::organization, APPINFO::name);
	setDifficulty(settings.value("difficulty").value<HighScore::Difficulty>());
	const int size = settings.beginReadArray("High Scores");
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		auto* model = new HighScoreModel(settings.value("model").value<HighScoreModel>());
		m_highScores.insert(model->difficulty(), model);
	}
	settings.endArray();
}