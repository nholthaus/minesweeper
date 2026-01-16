//--------------------------------------------------------------------------------------------------
//
/// @file   mainwindow_ext.cpp
/// @brief  Implementation of MainWindowExt with all extensions
//
//--------------------------------------------------------------------------------------------------

#include "mainwindow_ext.h"
#include "appinfo.h"
#include "customGameDialog.h"
#include "gameStatsDialog.h"
#include "highScoreDialog.h"
#include "leaderboardDialog.h"
#include "startMenuDialog.h"

#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QSignalTransition>
#include <QStatusBar>
#include <QStyleHints>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
MainWindowExt::MainWindowExt(QWidget* parent)
	: QMainWindow(parent)
	, m_versionChecker{"nholthaus", "minesweeper", APPINFO::version}
{
	setWindowIcon(QIcon(":/mine"));
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	// Initialize extensions first
	m_controller  = new GameController(this);
	m_hintSystem  = new HintSystem(this);
	m_leaderboard = new Leaderboard(this);

	setupStateMachine();
	setupMenus();
	setupExtensions();
	loadSettings();

	// Connect game signals
	connect(this, &MainWindowExt::defeat, this, &MainWindowExt::onDefeat);
	connect(this, &MainWindowExt::victory, this, &MainWindowExt::onVictory);

	connect(&m_versionChecker, &VersionChecker::newerVersionAvailable, this,
			[this](const QString& version, const QString& url)
			{
				QMessageBox msgBox(this);
				msgBox.setWindowTitle(QString("Version %1 Available").arg(version));
				msgBox.setTextFormat(Qt::RichText);
				msgBox.setText(QString("A new version of minesweeper is available!<br><a href='%1'>Click here to Download.</a>").arg(url));
				msgBox.exec();
			});

	this->layout()->setSizeConstraint(QLayout::SetFixedSize);

	// Apply initial theme
	onThemeChanged(m_controller->currentTheme());

	m_versionChecker.checkForNewerVersion();
}

//--------------------------------------------------------------------------------------------------
//  DESTRUCTOR
//--------------------------------------------------------------------------------------------------
MainWindowExt::~MainWindowExt() { saveSettings(); }

//--------------------------------------------------------------------------------------------------
//  SETUP UI
//--------------------------------------------------------------------------------------------------
void MainWindowExt::setupUI()
{
	// This is called from initialize()
}

//--------------------------------------------------------------------------------------------------
//  SETUP EXTENSIONS
//--------------------------------------------------------------------------------------------------
void MainWindowExt::setupExtensions()
{
	// Connect GameController signals
	connect(m_controller, &GameController::pauseRequested, this,
			[this]()
			{
				if (m_gameClock)
					m_gameClock->stop();
				if (m_pauseOverlay)
					m_pauseOverlay->showOverlay();
			});

	connect(m_controller, &GameController::resumeRequested, this,
			[this]()
			{
				if (m_gameClock)
					m_gameClock->start();
				if (m_pauseOverlay)
					m_pauseOverlay->hideOverlay();
			});

	connect(m_controller, &GameController::hintRequested, this,
			[this]()
			{
				if (m_hintSystem)
					m_hintSystem->provideHint();
			});

	connect(m_controller, &GameController::hintsRemainingChanged, this,
			[this](int remaining)
			{
				if (m_hintsLabel)
				{
					m_hintsLabel->setText(tr("Hints: %1").arg(remaining));
				}
				if (m_hintButton)
				{
					m_hintButton->setEnabled(remaining > 0);
				}
				if (m_hintAction)
				{
					m_hintAction->setEnabled(remaining > 0);
				}
			});

	connect(m_controller, &GameController::timeAttackStarted, this, &MainWindowExt::onTimeAttackStarted);
	connect(m_controller, &GameController::timeRemainingChanged, this, &MainWindowExt::onTimeRemainingChanged);
	connect(m_controller, &GameController::timeExpired, this, &MainWindowExt::onTimeExpired);

	connect(m_controller, &GameController::themeChanged, this, &MainWindowExt::onThemeChanged);
}

//--------------------------------------------------------------------------------------------------
//  SET DIFFICULTY
//--------------------------------------------------------------------------------------------------
void MainWindowExt::setDifficulty(GameEnums::Difficulty difficulty)
{
	m_controller->setDifficulty(difficulty);
	getDifficultySettings(difficulty, m_numRows, m_numCols, m_numMines);

	// Update menu checkmarks
	switch (difficulty)
	{
	case GameEnums::Difficulty::Beginner:
		m_beginnerAction->setChecked(true);
		break;
	case GameEnums::Difficulty::Intermediate:
		m_intermediateAction->setChecked(true);
		break;
	case GameEnums::Difficulty::Expert:
		m_expertAction->setChecked(true);
		break;
	case GameEnums::Difficulty::Custom:
		m_customAction->setChecked(true);
		break;
	}

	initialize();
	adjustSize();
}

//--------------------------------------------------------------------------------------------------
//  GET DIFFICULTY SETTINGS
//--------------------------------------------------------------------------------------------------
void MainWindowExt::getDifficultySettings(GameEnums::Difficulty difficulty, quint32& rows, quint32& cols, quint32& mines)
{
	switch (difficulty)
	{
	case GameEnums::Difficulty::Beginner:
		rows  = 9;
		cols  = 9;
		mines = 10;
		break;
	case GameEnums::Difficulty::Intermediate:
		rows  = 16;
		cols  = 16;
		mines = 40;
		break;
	case GameEnums::Difficulty::Expert:
		rows  = 16;
		cols  = 30;
		mines = 99;
		break;
	case GameEnums::Difficulty::Custom:
		{
			auto settings = m_controller->customSettings();
			rows		  = settings.rows;
			cols		  = settings.cols;
			mines		  = settings.mines;
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------------
//  INITIALIZE
//--------------------------------------------------------------------------------------------------
// void MainWindowExt::initialize()
// {
// 	QFrame* newMainFrame	= new QFrame(this);
// 	auto	mainFrameLayout = new QVBoxLayout;
// 	auto	infoLayout		= new QHBoxLayout;

// 	// Core components (UNCHANGED from original)
// 	m_gameBoard		= new GameBoard(m_numRows, m_numCols, m_numMines, newMainFrame);
// 	m_mineCounter	= new MineCounter(newMainFrame);
// 	m_mineTimer		= new MineTimer(newMainFrame);
// 	m_newGameButton = new QPushButton(newMainFrame);
// 	m_gameClock		= new QTimer(this);

// 	// Set hint system board
// 	m_hintSystem->setGameBoard(m_gameBoard);

// 	m_mineCounter->setNumMines(m_numMines);

// 	// Connect core signals (UNCHANGED from original)
// 	connect(m_gameBoard, &GameBoard::initialized, this, &MainWindowExt::startGame, Qt::UniqueConnection);
// 	connect(m_gameBoard, &GameBoard::flagCountChanged, m_mineCounter, &MineCounter::setFlagCount, Qt::UniqueConnection);
// 	connect(m_gameBoard, &GameBoard::victory, this, &MainWindowExt::victory, Qt::UniqueConnection);
// 	connect(m_gameBoard, &GameBoard::defeat, this, &MainWindowExt::defeat, Qt::UniqueConnection);

// 	// New game button (smiley)
// 	m_newGameButton->setMinimumSize(35, 35);
// 	m_newGameButton->setIconSize(QSize(30, 30));
// 	m_newGameButton->setIcon(QIcon(":/emoji/smile"));
// 	connect(m_newGameButton, &QPushButton::clicked, this, &MainWindowExt::startNewGame, Qt::UniqueConnection);

// 	m_gameClock->setInterval(1000);
// 	connect(m_gameClock, &QTimer::timeout, m_mineTimer, &MineTimer::incrementTime, Qt::UniqueConnection);

// 	// Info bar layout
// 	infoLayout->addWidget(m_mineCounter);
// 	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));

// 	// Extension buttons
// 	m_pauseButton = new QPushButton("⏸", newMainFrame);
// 	m_pauseButton->setToolTip(tr("Pause (Space)"));
// 	m_pauseButton->setMinimumSize(35, 35);
// 	m_pauseButton->setMaximumSize(35, 35);
// 	connect(m_pauseButton, &QPushButton::clicked, this, &MainWindowExt::onPauseToggle);
// 	infoLayout->addWidget(m_pauseButton);

// 	infoLayout->addWidget(m_newGameButton);

// 	m_hintButton = new QPushButton("💡", newMainFrame);
// 	m_hintButton->setToolTip(tr("Hint (H)"));
// 	m_hintButton->setMinimumSize(35, 35);
// 	m_hintButton->setMaximumSize(35, 35);
// 	connect(m_hintButton, &QPushButton::clicked, this, &MainWindowExt::onHintRequested);
// 	infoLayout->addWidget(m_hintButton);

// 	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
// 	infoLayout->addWidget(m_mineTimer);

// 	mainFrameLayout->addLayout(infoLayout);

// 	// Mode indicator for Time Attack
// 	if (m_controller->currentMode() == GameEnums::GameMode::TimeAttack)
// 	{
// 		m_timeAttackLabel = new QLabel(tr("⚡ Time Attack"), newMainFrame);
// 		m_timeAttackLabel->setAlignment(Qt::AlignCenter);
// 		m_timeAttackLabel->setStyleSheet("QLabel { color: #FF5722; font-weight: bold; }");
// 		mainFrameLayout->addWidget(m_timeAttackLabel);
// 	}

// 	mainFrameLayout->addWidget(m_gameBoard);

// 	// Hints remaining indicator
// 	m_hintsLabel = new QLabel(tr("Hints: %1").arg(m_controller->hintsRemaining()), newMainFrame);
// 	m_hintsLabel->setAlignment(Qt::AlignCenter);
// 	mainFrameLayout->addWidget(m_hintsLabel);

// 	newMainFrame->setLayout(mainFrameLayout);

// 	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
// 	this->setCentralWidget(newMainFrame);

// 	// Create pause overlay
// 	m_pauseOverlay = new PauseOverlay(newMainFrame);
// 	connect(m_pauseOverlay, &PauseOverlay::resumeClicked, this, &MainWindowExt::onResumeFromPause);
// 	connect(m_pauseOverlay, &PauseOverlay::newGameClicked, this,
// 			[this]()
// 			{
// 				// Hide pause overlay and start new game
// 				m_pauseOverlay->hideOverlay();
// 				onRestartRequested();
// 			});

// 	std::swap(m_mainFrame, newMainFrame);
// 	delete newMainFrame;

// 	// Apply current theme
// 	applyTheme(ThemeManager::instance().colorScheme());

// 	// Reset controller state
// 	m_controller->resetGame();
// }

void MainWindowExt::initialize()
{
	QFrame* newMainFrame	= new QFrame(this);
	auto	mainFrameLayout = new QVBoxLayout;
	auto	infoLayout		= new QHBoxLayout;

	// Thêm margin để căn giữa
	mainFrameLayout->setContentsMargins(20, 20, 20, 20);
	mainFrameLayout->setSpacing(10);

	// Core components (UNCHANGED from original)
	m_gameBoard		= new GameBoard(m_numRows, m_numCols, m_numMines, newMainFrame);
	m_mineCounter	= new MineCounter(newMainFrame);
	m_mineTimer		= new MineTimer(newMainFrame);
	m_newGameButton = new QPushButton(newMainFrame);
	m_gameClock		= new QTimer(this);

	// Set hint system board
	m_hintSystem->setGameBoard(m_gameBoard);

	m_mineCounter->setNumMines(m_numMines);

	// Connect core signals (UNCHANGED from original)
	connect(m_gameBoard, &GameBoard::initialized, this, &MainWindowExt::startGame, Qt::UniqueConnection);
	connect(m_gameBoard, &GameBoard::flagCountChanged, m_mineCounter, &MineCounter::setFlagCount, Qt::UniqueConnection);
	connect(m_gameBoard, &GameBoard::victory, this, &MainWindowExt::victory, Qt::UniqueConnection);
	connect(m_gameBoard, &GameBoard::defeat, this, &MainWindowExt::defeat, Qt::UniqueConnection);

	// New game button (smiley)
	m_newGameButton->setMinimumSize(35, 35);
	m_newGameButton->setIconSize(QSize(30, 30));
	m_newGameButton->setIcon(QIcon(":/emoji/smile"));
	connect(m_newGameButton, &QPushButton::clicked, this, &MainWindowExt::startNewGame, Qt::UniqueConnection);

	m_gameClock->setInterval(1000);
	connect(m_gameClock, &QTimer::timeout, m_mineTimer, &MineTimer::incrementTime, Qt::UniqueConnection);

	// Info bar layout
	infoLayout->addWidget(m_mineCounter);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));

	// Extension buttons
	m_pauseButton = new QPushButton("⏸", newMainFrame);
	m_pauseButton->setToolTip(tr("Pause (Space)"));
	m_pauseButton->setMinimumSize(35, 35);
	m_pauseButton->setMaximumSize(35, 35);
	connect(m_pauseButton, &QPushButton::clicked, this, &MainWindowExt::onPauseToggle);
	infoLayout->addWidget(m_pauseButton);

	infoLayout->addWidget(m_newGameButton);

	m_hintButton = new QPushButton("💡", newMainFrame);
	m_hintButton->setToolTip(tr("Hint (H)"));
	m_hintButton->setMinimumSize(35, 35);
	m_hintButton->setMaximumSize(35, 35);
	connect(m_hintButton, &QPushButton::clicked, this, &MainWindowExt::onHintRequested);
	infoLayout->addWidget(m_hintButton);

	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
	infoLayout->addWidget(m_mineTimer);

	mainFrameLayout->addLayout(infoLayout);

	// Mode indicator for Time Attack
	if (m_controller->currentMode() == GameEnums::GameMode::TimeAttack)
	{
		m_timeAttackLabel = new QLabel(tr("⚡ Time Attack"), newMainFrame);
		m_timeAttackLabel->setAlignment(Qt::AlignCenter);
		m_timeAttackLabel->setStyleSheet("QLabel { color: #FF5722; font-weight: bold; }");
		mainFrameLayout->addWidget(m_timeAttackLabel);
	}

	// Căn giữa game board
	mainFrameLayout->addWidget(m_gameBoard, 0, Qt::AlignCenter);

	// Hints remaining indicator
	m_hintsLabel = new QLabel(tr("Hints: %1").arg(m_controller->hintsRemaining()), newMainFrame);
	m_hintsLabel->setAlignment(Qt::AlignCenter);
	mainFrameLayout->addWidget(m_hintsLabel);

	newMainFrame->setLayout(mainFrameLayout);

	// Căn giữa frame chính
	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	newMainFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	this->setCentralWidget(newMainFrame);

	// Create pause overlay
	m_pauseOverlay = new PauseOverlay(newMainFrame);
	connect(m_pauseOverlay, &PauseOverlay::resumeClicked, this, &MainWindowExt::onResumeFromPause);
	connect(m_pauseOverlay, &PauseOverlay::newGameClicked, this,
			[this]()
			{
				// Hide pause overlay and start new game
				m_pauseOverlay->hideOverlay();
				onRestartRequested();
			});

	std::swap(m_mainFrame, newMainFrame);
	delete newMainFrame;

	// Apply current theme
	applyTheme(ThemeManager::instance().colorScheme());

	// Reset controller state
	m_controller->resetGame();
}
//--------------------------------------------------------------------------------------------------
//  SETUP STATE MACHINE
//--------------------------------------------------------------------------------------------------
void MainWindowExt::setupStateMachine()
{
	m_machine = new QStateMachine(this);

	m_unstartedState  = new QState;
	m_inProgressState = new QState;
	m_pausedState	  = new QState;
	m_victoryState	  = new QState;
	m_defeatState	  = new QState;

	// State transitions (UNCHANGED core logic)
	m_unstartedState->addTransition(this, &MainWindowExt::startGame, m_inProgressState);

	m_inProgressState->addTransition(this, &MainWindowExt::victory, m_victoryState);
	m_inProgressState->addTransition(this, &MainWindowExt::defeat, m_defeatState);
	QSignalTransition* forfeitTransition = m_inProgressState->addTransition(this, &MainWindowExt::startNewGame, m_unstartedState);

	// Pause transitions (NEW)
	m_inProgressState->addTransition(m_controller, &GameController::pauseRequested, m_pausedState);
	m_pausedState->addTransition(m_controller, &GameController::resumeRequested, m_inProgressState);
	m_pausedState->addTransition(this, &MainWindowExt::startNewGame, m_unstartedState);

	m_victoryState->addTransition(this, &MainWindowExt::startNewGame, m_unstartedState);
	m_defeatState->addTransition(this, &MainWindowExt::startNewGame, m_unstartedState);

	// State entry handlers
	connect(m_unstartedState, &QState::entered, [this]() { initialize(); });

	connect(m_inProgressState, &QState::entered,
			[this]()
			{
				m_gameClock->start();
				m_controller->startGame();
			});

	connect(forfeitTransition, &QSignalTransition::triggered,
			[this]()
			{
				auto difficulty = GameController::toHighScoreDifficulty(m_controller->currentDifficulty());
				m_gameStats.addStat(difficulty, GameStats::Forfeit, m_mineTimer->time());
			});

	connect(m_pausedState, &QState::entered,
			[this]()
			{
				m_gameClock->stop();
				m_pauseButton->setText("▶");
			});

	connect(m_pausedState, &QState::exited, [this]() { m_pauseButton->setText("⏸"); });

	connect(m_victoryState, &QState::entered,
			[this]()
			{
				m_gameClock->stop();
				m_controller->winGame();
			});

	connect(m_defeatState, &QState::entered,
			[this]()
			{
				m_gameClock->stop();
				m_controller->loseGame();
			});

	m_machine->addState(m_unstartedState);
	m_machine->addState(m_inProgressState);
	m_machine->addState(m_pausedState);
	m_machine->addState(m_victoryState);
	m_machine->addState(m_defeatState);

	m_machine->setInitialState(m_unstartedState);
	m_machine->start();
}

//--------------------------------------------------------------------------------------------------
//  ON VICTORY
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onVictory()
{
	m_newGameButton->setIcon(QIcon(":/emoji/sunglasses"));

	auto difficulty	  = m_controller->currentDifficulty();
	auto mode		  = m_controller->currentMode();
	auto hsDifficulty = GameController::toHighScoreDifficulty(difficulty);

	m_gameStats.addStat(hsDifficulty, GameStats::Win, m_mineTimer->time());

	// Check for high score in leaderboard
	if (m_leaderboard->isHighScore(m_mineTimer->time(), difficulty, mode))
	{
		auto name = QInputDialog::getText(this, tr("Congratulations!"), tr("You've earned a high score!\nPlease enter your name:"));

		if (!name.isEmpty())
		{
			LeaderboardEntry entry;
			entry.name		 = name;
			entry.score		 = m_mineTimer->time();
			entry.date		 = QDateTime::currentDateTime();
			entry.difficulty = difficulty;
			entry.mode		 = mode;

			if (difficulty == GameEnums::Difficulty::Custom)
			{
				auto settings	  = m_controller->customSettings();
				entry.customRows  = settings.rows;
				entry.customCols  = settings.cols;
				entry.customMines = settings.mines;
			}

			m_leaderboard->addEntry(entry);

			// Show leaderboard
			LeaderboardDialog dialog(m_leaderboard, this);
			dialog.setActiveTab(difficulty);
			dialog.setActiveMode(mode);
			dialog.exec();
		}
	}
}

//--------------------------------------------------------------------------------------------------
//  ON DEFEAT
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onDefeat()
{
	m_newGameButton->setIcon(QIcon(":/emoji/injured"));

	auto hsDifficulty = GameController::toHighScoreDifficulty(m_controller->currentDifficulty());
	m_gameStats.addStat(hsDifficulty, GameStats::Loss, m_mineTimer->time());
}

//--------------------------------------------------------------------------------------------------
//  CLOSE EVENT
//--------------------------------------------------------------------------------------------------
void MainWindowExt::closeEvent(QCloseEvent* event)
{
	if (m_machine->configuration().contains(m_inProgressState) || m_machine->configuration().contains(m_pausedState))
	{
		auto hsDifficulty = GameController::toHighScoreDifficulty(m_controller->currentDifficulty());
		m_gameStats.addStat(hsDifficulty, GameStats::Forfeit, m_mineTimer->time());
	}
	saveSettings();
	QMainWindow::closeEvent(event);
}

//--------------------------------------------------------------------------------------------------
//  SETUP MENUS
//--------------------------------------------------------------------------------------------------
void MainWindowExt::setupMenus()
{
	// Game Menu
	m_gameMenu = new QMenu(tr("Game"));

	m_newGameAction = new QAction(tr("New Game..."));
	m_newGameAction->setShortcut(QKeySequence(Qt::Key_F2));
	connect(m_newGameAction, &QAction::triggered, this,
			[this]()
			{
				StartMenuDialog dialog(m_controller, StartMenuContext::NewGame, this);
				connect(&dialog, &StartMenuDialog::newGameRequested, this, &MainWindowExt::onNewGameRequested);
				connect(&dialog, &StartMenuDialog::restartRequested, this, &MainWindowExt::onRestartRequested);
				connect(&dialog, &StartMenuDialog::themeChangeRequested, this, &MainWindowExt::onThemeChanged);
				connect(&dialog, &StartMenuDialog::leaderboardRequested, this,
						[this]()
						{
							LeaderboardDialog dlg(m_leaderboard, this);
							dlg.setActiveTab(m_controller->currentDifficulty());
							dlg.setActiveMode(m_controller->currentMode());
							dlg.exec();
						});
				connect(&dialog, &StartMenuDialog::statisticsRequested, this,
						[this]()
						{
							auto	difficulty = GameController::toHighScoreDifficulty(m_controller->currentDifficulty());
							QString diffName   = QVariant::fromValue(difficulty).toString();
							QString tabName	   = diffName[0].toUpper() + diffName.mid(1).toLower();
							auto*	dlg		   = new GameStatsDialog(m_gameStats, this);
							dlg->setActiveTab(tabName);
							dlg->exec();
							dlg->deleteLater();
						});
				dialog.exec();
			});

	m_pauseAction = new QAction(tr("Pause"));
	m_pauseAction->setShortcut(QKeySequence(Qt::Key_Space));
	m_pauseAction->setCheckable(true);
	connect(m_pauseAction, &QAction::triggered, this, &MainWindowExt::onPauseToggle);

	// Difficulty submenu
	m_difficultyMenu		= new QMenu(tr("Difficulty"));
	m_difficultyActionGroup = new QActionGroup(m_difficultyMenu);

	m_beginnerAction = new QAction(tr("Beginner"), m_difficultyActionGroup);
	m_beginnerAction->setCheckable(true);
	connect(m_beginnerAction, &QAction::triggered, [this]() { setDifficulty(GameEnums::Difficulty::Beginner); });

	m_intermediateAction = new QAction(tr("Intermediate"), m_difficultyActionGroup);
	m_intermediateAction->setCheckable(true);
	connect(m_intermediateAction, &QAction::triggered, [this]() { setDifficulty(GameEnums::Difficulty::Intermediate); });

	m_expertAction = new QAction(tr("Expert"), m_difficultyActionGroup);
	m_expertAction->setCheckable(true);
	connect(m_expertAction, &QAction::triggered, [this]() { setDifficulty(GameEnums::Difficulty::Expert); });

	m_customAction = new QAction(tr("Custom..."), m_difficultyActionGroup);
	m_customAction->setCheckable(true);
	connect(m_customAction, &QAction::triggered,
			[this]()
			{
				CustomGameDialog dialog(m_controller->customSettings(), this);
				if (dialog.exec() == QDialog::Accepted)
				{
					m_controller->setCustomSettings(dialog.settings());
					setDifficulty(GameEnums::Difficulty::Custom);
				}
			});

	m_difficultyMenu->addAction(m_beginnerAction);
	m_difficultyMenu->addAction(m_intermediateAction);
	m_difficultyMenu->addAction(m_expertAction);
	m_difficultyMenu->addSeparator();
	m_difficultyMenu->addAction(m_customAction);

	// Mode submenu
	m_modeMenu		  = new QMenu(tr("Game Mode"));
	m_modeActionGroup = new QActionGroup(m_modeMenu);

	m_normalModeAction = new QAction(tr("Normal"), m_modeActionGroup);
	m_normalModeAction->setCheckable(true);
	m_normalModeAction->setChecked(true);
	connect(m_normalModeAction, &QAction::triggered,
			[this]()
			{
				m_controller->setGameMode(GameEnums::GameMode::Normal);
				emit startNewGame();
			});

	m_timeAttackModeAction = new QAction(tr("Time Attack"), m_modeActionGroup);
	m_timeAttackModeAction->setCheckable(true);
	connect(m_timeAttackModeAction, &QAction::triggered,
			[this]()
			{
				m_controller->setGameMode(GameEnums::GameMode::TimeAttack);
				emit startNewGame();
			});

	m_modeMenu->addAction(m_normalModeAction);
	m_modeMenu->addAction(m_timeAttackModeAction);

	m_hintAction = new QAction(tr("Hint"));
	m_hintAction->setShortcut(QKeySequence(Qt::Key_H));
	connect(m_hintAction, &QAction::triggered, this, &MainWindowExt::onHintRequested);

	m_leaderboardAction = new QAction(tr("Leaderboard..."));
	connect(m_leaderboardAction, &QAction::triggered,
			[this]()
			{
				LeaderboardDialog dialog(m_leaderboard, this);
				dialog.setActiveTab(m_controller->currentDifficulty());
				dialog.setActiveMode(m_controller->currentMode());
				dialog.exec();
			});

	m_statisticsAction = new QAction(tr("Statistics..."));
	connect(m_statisticsAction, &QAction::triggered,
			[this]()
			{
				auto	difficulty = GameController::toHighScoreDifficulty(m_controller->currentDifficulty());
				QString diffName   = QVariant::fromValue(difficulty).toString();
				QString tabName	   = diffName[0].toUpper() + diffName.mid(1).toLower();

				auto* dialog = new GameStatsDialog(m_gameStats, this);
				dialog->setActiveTab(tabName);
				dialog->exec();
				dialog->deleteLater();
			});

	m_exitAction = new QAction(tr("Exit"));
	connect(m_exitAction, &QAction::triggered, this, &QMainWindow::close);

	m_gameMenu->addAction(m_newGameAction);
	m_gameMenu->addAction(m_pauseAction);
	m_gameMenu->addSeparator();
	m_gameMenu->addAction(m_hintAction);
	m_gameMenu->addSeparator();
	m_gameMenu->addAction(m_exitAction);

	// View Menu
	m_viewMenu = new QMenu(tr("View"));

	m_themeMenu		   = new QMenu(tr("Theme"));
	m_themeActionGroup = new QActionGroup(m_themeMenu);

	m_lightThemeAction = new QAction(tr("Light"), m_themeActionGroup);
	m_lightThemeAction->setCheckable(true);
	m_lightThemeAction->setChecked(true);
	connect(m_lightThemeAction, &QAction::triggered, [this]() { onThemeChanged(GameEnums::Theme::Light); });

	m_darkThemeAction = new QAction(tr("Dark"), m_themeActionGroup);
	m_darkThemeAction->setCheckable(true);
	connect(m_darkThemeAction, &QAction::triggered, [this]() { onThemeChanged(GameEnums::Theme::Dark); });

	m_themeMenu->addAction(m_lightThemeAction);
	m_themeMenu->addAction(m_darkThemeAction);

	m_viewMenu->addMenu(m_themeMenu);

	// Help Menu
	m_helpMenu = new QMenu(tr("Help"));

	m_aboutAction = new QAction(tr("About..."));
	m_aboutAction->setIcon(QIcon(":/mine"));
	connect(m_aboutAction, &QAction::triggered,
			[this]()
			{
				QFile	licenseFile(":/LICENSE");
				QString licenseText;
				if (licenseFile.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					licenseText = licenseFile.readAll();
					licenseFile.close();
				}
				QMessageBox::about(this, "About Minesweeper",
								   QString("Minesweeper")
									   .append("\nVersion: ")
									   .append(APPINFO::version)
									   .append("\n\nExtended Edition with:\n• Custom Games\n• Time Attack Mode\n• Hints\n• Dark/Light Theme\n• Pause\n\n")
									   .append(licenseText));
			});

	m_aboutQtAction = new QAction(tr("About Qt..."));
	m_aboutQtAction->setIcon(style()->standardIcon(QStyle::SP_TitleBarMenuButton));
	connect(m_aboutQtAction, &QAction::triggered, [this]() { QMessageBox::aboutQt(this); });

	m_checkVersionAction = new QAction(tr("Check for Updates..."));
	connect(m_checkVersionAction, &QAction::triggered,
			[this]()
			{
				m_versionChecker.checkForNewerVersion();
				connect(
					&m_versionChecker, &VersionChecker::noNewerVersion, this,
					[this]() { QMessageBox::information(this, "No newer version", "You already have the latest version of Minesweeper installed."); },
					Qt::SingleShotConnection);
			});

	m_helpMenu->addAction(m_aboutAction);
	m_helpMenu->addAction(m_aboutQtAction);
	m_helpMenu->addSeparator();
	m_helpMenu->addAction(m_checkVersionAction);

	// Add menus to menu bar
	menuBar()->addMenu(m_gameMenu);
	menuBar()->addMenu(m_viewMenu);
	menuBar()->addMenu(m_helpMenu);
}

//--------------------------------------------------------------------------------------------------
//  ON NEW GAME REQUESTED
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onNewGameRequested(GameEnums::GameMode mode, GameEnums::Difficulty difficulty)
{
	m_controller->setGameMode(mode);

	// Update mode menu
	m_normalModeAction->setChecked(mode == GameEnums::GameMode::Normal);
	m_timeAttackModeAction->setChecked(mode == GameEnums::GameMode::TimeAttack);

	setDifficulty(difficulty);
}

//--------------------------------------------------------------------------------------------------
//  ON RESTART REQUESTED
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onRestartRequested() { emit startNewGame(); }

//--------------------------------------------------------------------------------------------------
//  ON PAUSE TOGGLE
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onPauseToggle()
{
	if (m_controller->isPaused())
	{
		m_controller->resumeGame();
		m_pauseAction->setChecked(false);
	}
	else if (m_controller->currentState() == GameEnums::GameState::Playing)
	{
		m_controller->pauseGame();
		m_pauseAction->setChecked(true);
	}
}

//--------------------------------------------------------------------------------------------------
//  ON RESUME FROM PAUSE
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onResumeFromPause()
{
	m_controller->resumeGame();
	m_pauseAction->setChecked(false);
}

//--------------------------------------------------------------------------------------------------
//  ON HINT REQUESTED
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onHintRequested()
{
	if (m_controller->currentState() == GameEnums::GameState::Playing)
	{
		m_controller->useHint();
	}
}

//--------------------------------------------------------------------------------------------------
//  TIME ATTACK HANDLERS
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onTimeAttackStarted(int timeLimit)
{
	Q_UNUSED(timeLimit)
	updateTimeDisplay();
}

void MainWindowExt::onTimeRemainingChanged(int secondsRemaining)
{
	Q_UNUSED(secondsRemaining)
	updateTimeDisplay();
}

void MainWindowExt::onTimeExpired() { QMessageBox::information(this, tr("Time's Up!"), tr("You ran out of time!\nBetter luck next time.")); }

void MainWindowExt::updateTimeDisplay()
{
	if (m_timeAttackLabel && m_controller->isTimeAttackMode())
	{
		int remaining = m_controller->timeRemaining();
		int minutes	  = remaining / 60;
		int seconds	  = remaining % 60;

		QString color = remaining <= 30 ? "#FF0000" : "#FF5722";
		m_timeAttackLabel->setText(tr("⚡ Time Attack: %1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
		m_timeAttackLabel->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; }").arg(color));
	}
}

//--------------------------------------------------------------------------------------------------
//  THEME
//--------------------------------------------------------------------------------------------------
void MainWindowExt::onThemeChanged(GameEnums::Theme theme)
{
	m_controller->setTheme(theme);
	ThemeManager::instance().setTheme(theme);

	// Update menu checkmarks
	// System theme removed - only Light and Dark available
	m_lightThemeAction->setChecked(theme == GameEnums::Theme::Light);
	m_darkThemeAction->setChecked(theme == GameEnums::Theme::Dark);

	applyTheme(ThemeManager::instance().colorScheme());
}

void MainWindowExt::applyTheme(Qt::ColorScheme colorScheme)
{
	if (m_gameBoard)
		m_gameBoard->setTheme(colorScheme);
	if (m_mineCounter)
		m_mineCounter->setTheme(colorScheme);
	if (m_mineTimer)
		m_mineTimer->setTheme(colorScheme);

	setStyleSheet(ThemeManager::instance().styleSheet());
}

void MainWindowExt::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::ThemeChange || event->type() == QEvent::StyleChange)
	{
		if (m_controller->currentTheme() == GameEnums::Theme::System)
		{
			if (auto* styleHints = QGuiApplication::styleHints())
			{
				applyTheme(styleHints->colorScheme());
			}
		}
	}
	QMainWindow::changeEvent(event);
}

void MainWindowExt::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);

	// Update pause overlay size
	if (m_pauseOverlay && m_mainFrame)
	{
		m_pauseOverlay->setGeometry(m_mainFrame->rect());
	}
}

//--------------------------------------------------------------------------------------------------
//  SAVE/LOAD SETTINGS
//--------------------------------------------------------------------------------------------------
void MainWindowExt::saveSettings()
{
	QSettings settings(APPINFO::organization, APPINFO::name);

	// Save difficulty
	settings.setValue("difficulty", static_cast<int>(m_controller->currentDifficulty()));
	settings.setValue("gameMode", static_cast<int>(m_controller->currentMode()));
	settings.setValue("theme", static_cast<int>(m_controller->currentTheme()));

	// Save custom settings
	auto custom = m_controller->customSettings();
	settings.setValue("custom/rows", custom.rows);
	settings.setValue("custom/cols", custom.cols);
	settings.setValue("custom/mines", custom.mines);

	// Save leaderboard
	{
		QByteArray	data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		m_leaderboard->save(stream);
		settings.setValue("leaderboard", data);
	}

	// Save game stats
	{
		QByteArray	data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream << m_gameStats;
		settings.setValue("stats", data);
	}
}

void MainWindowExt::loadSettings()
{
	QSettings settings(APPINFO::organization, APPINFO::name);

	// Load custom settings
	CustomGameSettings custom;
	custom.rows	 = settings.value("custom/rows", 9).toUInt();
	custom.cols	 = settings.value("custom/cols", 9).toUInt();
	custom.mines = settings.value("custom/mines", 10).toUInt();
	m_controller->setCustomSettings(custom);

	// Load game mode
	auto mode = static_cast<GameEnums::GameMode>(settings.value("gameMode", 0).toInt());
	m_controller->setGameMode(mode);
	m_normalModeAction->setChecked(mode == GameEnums::GameMode::Normal);
	m_timeAttackModeAction->setChecked(mode == GameEnums::GameMode::TimeAttack);

	// Load theme
	auto theme = static_cast<GameEnums::Theme>(settings.value("theme", 0).toInt());
	onThemeChanged(theme);

	// Load difficulty and initialize
	auto difficulty = static_cast<GameEnums::Difficulty>(settings.value("difficulty", 0).toInt());
	setDifficulty(difficulty);

	// Load leaderboard
	{
		QByteArray data = settings.value("leaderboard").toByteArray();
		if (!data.isEmpty())
		{
			QDataStream stream(&data, QIODevice::ReadOnly);
			m_leaderboard->load(stream);
		}
	}

	// Load game stats
	{
		QByteArray data = settings.value("stats").toByteArray();
		if (!data.isEmpty())
		{
			QDataStream stream(&data, QIODevice::ReadOnly);
			stream >> m_gameStats;
		}
	}
}
