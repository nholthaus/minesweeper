//--------------------------------------------------------------------------------------------------
//
/// @file   mainwindow_ext.h
/// @brief  Extended MainWindow with all new features integrated
/// @note   This extends the existing MainWindow without modifying core gameplay
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include "gameController.h"
#include "gameEnums.h"
#include "gameStats.h"
#include "gameboard.h"
#include "highScoreModel.h"
#include "hintSystem.h"
#include "leaderboard.h"
#include "mineCounter.h"
#include "minetimer.h"
#include "pauseOverlay.h"
#include "themeManager.h"
#include "tile.h"

#include <QAction>
#include <QActionGroup>
#include <QFrame>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QState>
#include <QStateMachine>
#include <QToolBar>

#include "versionChecker.h"

//--------------------------------------------------------------------------------------------------
//  CLASS: MainWindowExt
//--------------------------------------------------------------------------------------------------
class MainWindowExt : public QMainWindow
{
	Q_OBJECT

public:

	MainWindowExt(QWidget* parent = nullptr);
	~MainWindowExt() override;

signals:
	void startGame();
	void victory();
	void defeat();
	void startNewGame();

protected slots:
	void onVictory();
	void onDefeat();

protected:

	void closeEvent(QCloseEvent* event) override;
	void changeEvent(QEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private slots:
	//----------------------------------------------------------------------------------------------
	//  GAME CONTROL
	//----------------------------------------------------------------------------------------------
	void onNewGameRequested(GameEnums::GameMode mode, GameEnums::Difficulty difficulty);
	void onRestartRequested();
	void onPauseToggle();
	void onResumeFromPause();
	void onHintRequested();

	//----------------------------------------------------------------------------------------------
	//  TIME ATTACK
	//----------------------------------------------------------------------------------------------
	void onTimeAttackStarted(int timeLimit);
	void onTimeRemainingChanged(int secondsRemaining);
	void onTimeExpired();

	//----------------------------------------------------------------------------------------------
	//  THEME
	//----------------------------------------------------------------------------------------------
	void onThemeChanged(GameEnums::Theme theme);

private:

	//----------------------------------------------------------------------------------------------
	//  INITIALIZATION
	//----------------------------------------------------------------------------------------------
	void setupUI();
	void setupMenus();
	void setupToolBar();
	void setupStateMachine();
	void setupExtensions();
	void initialize();

	//----------------------------------------------------------------------------------------------
	//  DIFFICULTY
	//----------------------------------------------------------------------------------------------
	void setDifficulty(GameEnums::Difficulty difficulty);
	void getDifficultySettings(GameEnums::Difficulty difficulty, quint32& rows, quint32& cols, quint32& mines);

	//----------------------------------------------------------------------------------------------
	//  SETTINGS
	//----------------------------------------------------------------------------------------------
	void saveSettings();
	void loadSettings();

	//----------------------------------------------------------------------------------------------
	//  THEME
	//----------------------------------------------------------------------------------------------
	void applyTheme(Qt::ColorScheme colorScheme);
	void updateTimeDisplay();

private:

	//----------------------------------------------------------------------------------------------
	//  UI COMPONENTS
	//----------------------------------------------------------------------------------------------
	QFrame*		 m_mainFrame	 = nullptr;
	GameBoard*	 m_gameBoard	 = nullptr;
	MineCounter* m_mineCounter	 = nullptr;
	MineTimer*	 m_mineTimer	 = nullptr;
	QPushButton* m_newGameButton = nullptr;

	// Extended UI
	QPushButton*  m_pauseButton		= nullptr;
	QPushButton*  m_hintButton		= nullptr;
	QLabel*		  m_hintsLabel		= nullptr;
	QLabel*		  m_modeLabel		= nullptr;
	QLabel*		  m_timeAttackLabel = nullptr;
	PauseOverlay* m_pauseOverlay	= nullptr;

	//----------------------------------------------------------------------------------------------
	//  MENUS
	//----------------------------------------------------------------------------------------------
	QMenu*		  m_gameMenu			  = nullptr;
	QAction*	  m_newGameAction		  = nullptr;
	QAction*	  m_restartAction		  = nullptr;
	QAction*	  m_pauseAction			  = nullptr;
	QMenu*		  m_difficultyMenu		  = nullptr;
	QActionGroup* m_difficultyActionGroup = nullptr;
	QAction*	  m_beginnerAction		  = nullptr;
	QAction*	  m_intermediateAction	  = nullptr;
	QAction*	  m_expertAction		  = nullptr;
	QAction*	  m_customAction		  = nullptr;
	QMenu*		  m_modeMenu			  = nullptr;
	QActionGroup* m_modeActionGroup		  = nullptr;
	QAction*	  m_normalModeAction	  = nullptr;
	QAction*	  m_timeAttackModeAction  = nullptr;
	QAction*	  m_hintAction			  = nullptr;
	QAction*	  m_leaderboardAction	  = nullptr;
	QAction*	  m_statisticsAction	  = nullptr;
	QAction*	  m_exitAction			  = nullptr;

	QMenu*		  m_viewMenu		 = nullptr;
	QMenu*		  m_themeMenu		 = nullptr;
	QActionGroup* m_themeActionGroup = nullptr;
	QAction*	  m_lightThemeAction = nullptr;
	QAction*	  m_darkThemeAction	 = nullptr;

	QMenu*	 m_helpMenu			  = nullptr;
	QAction* m_aboutAction		  = nullptr;
	QAction* m_aboutQtAction	  = nullptr;
	QAction* m_checkVersionAction = nullptr;

	//----------------------------------------------------------------------------------------------
	//  STATE MACHINE (existing structure preserved)
	//----------------------------------------------------------------------------------------------
	QTimer*		   m_gameClock		 = nullptr;
	QStateMachine* m_machine		 = nullptr;
	QState*		   m_unstartedState	 = nullptr;
	QState*		   m_inProgressState = nullptr;
	QState*		   m_pausedState	 = nullptr;
	QState*		   m_victoryState	 = nullptr;
	QState*		   m_defeatState	 = nullptr;

	Tile* m_firstClicked = nullptr;

	//----------------------------------------------------------------------------------------------
	//  GAME SETTINGS
	//----------------------------------------------------------------------------------------------
	quint32 m_numRows  = 9;
	quint32 m_numCols  = 9;
	quint32 m_numMines = 10;

	//----------------------------------------------------------------------------------------------
	//  EXTENSIONS
	//----------------------------------------------------------------------------------------------
	GameController* m_controller  = nullptr;
	HintSystem*		m_hintSystem  = nullptr;
	Leaderboard*	m_leaderboard = nullptr;
	GameStats		m_gameStats;

	// Compatibility with existing high score system
	QMap<HighScore::Difficulty, HighScoreModel> m_highScores;

	VersionChecker m_versionChecker;
};
