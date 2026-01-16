//--------------------------------------------------------------------------------------------------
//
/// @file   gameController.h
/// @brief  Central game controller managing state, mode, and extensions
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include "gameEnums.h"
#include "highScore.h"

#include <QObject>
#include <QTimer>

//--------------------------------------------------------------------------------------------------
//  FORWARD DECLARATIONS
//--------------------------------------------------------------------------------------------------
class GameBoard;

//--------------------------------------------------------------------------------------------------
//  STRUCT: CustomGameSettings
//--------------------------------------------------------------------------------------------------
struct CustomGameSettings
{
	quint32 rows  = 9;
	quint32 cols  = 9;
	quint32 mines = 10;

	bool isValid() const
	{
		// Minimum 5x5, maximum 50x50
		if (rows < 5 || rows > 50)
			return false;
		if (cols < 5 || cols > 50)
			return false;
		// Mines must be at least 1 and at most (rows*cols - 9) to allow safe first click
		if (mines < 1 || mines > (rows * cols - 9))
			return false;
		return true;
	}
};

//--------------------------------------------------------------------------------------------------
//  STRUCT: TimeAttackSettings
//--------------------------------------------------------------------------------------------------
struct TimeAttackSettings
{
	// Time limits in seconds for each difficulty in Time Attack mode
	static constexpr int beginnerTimeLimit	   = 120; // 2 minutes
	static constexpr int intermediateTimeLimit = 300; // 5 minutes
	static constexpr int expertTimeLimit	   = 600; // 10 minutes
	static constexpr int customTimeLimit	   = 600; // 10 minutes default for custom

	static int getTimeLimit(GameEnums::Difficulty difficulty)
	{
		switch (difficulty)
		{
		case GameEnums::Difficulty::Beginner:
			return beginnerTimeLimit;
		case GameEnums::Difficulty::Intermediate:
			return intermediateTimeLimit;
		case GameEnums::Difficulty::Expert:
			return expertTimeLimit;
		case GameEnums::Difficulty::Custom:
			return customTimeLimit;
		default:
			return expertTimeLimit;
		}
	}
};

//--------------------------------------------------------------------------------------------------
//  CLASS: GameController
//--------------------------------------------------------------------------------------------------
/// @brief  Central controller for game state and extensions
/// @note   This is an extension layer - it does NOT modify core gameplay logic
//--------------------------------------------------------------------------------------------------
class GameController : public QObject
{
	Q_OBJECT

public:

	explicit GameController(QObject* parent = nullptr);
	~GameController() override = default;

	//----------------------------------------------------------------------------------------------
	//  STATE ACCESSORS
	//----------------------------------------------------------------------------------------------
	[[nodiscard]] GameEnums::GameState	currentState() const { return m_state; }
	[[nodiscard]] GameEnums::GameMode	currentMode() const { return m_mode; }
	[[nodiscard]] GameEnums::Difficulty currentDifficulty() const { return m_difficulty; }
	[[nodiscard]] GameEnums::Theme		currentTheme() const { return m_theme; }
	[[nodiscard]] CustomGameSettings	customSettings() const { return m_customSettings; }
	[[nodiscard]] int					hintsRemaining() const { return m_hintsRemaining; }
	[[nodiscard]] int					timeRemaining() const { return m_timeRemaining; }
	[[nodiscard]] bool					isPaused() const { return m_state == GameEnums::GameState::Paused; }
	[[nodiscard]] bool					isTimeAttackMode() const { return m_mode == GameEnums::GameMode::TimeAttack; }

	//----------------------------------------------------------------------------------------------
	//  DIFFICULTY CONVERSION (for compatibility with existing HighScore::Difficulty)
	//----------------------------------------------------------------------------------------------
	static HighScore::Difficulty toHighScoreDifficulty(GameEnums::Difficulty difficulty);
	static GameEnums::Difficulty fromHighScoreDifficulty(HighScore::Difficulty difficulty);

public slots:
	//----------------------------------------------------------------------------------------------
	//  STATE TRANSITIONS
	//----------------------------------------------------------------------------------------------
	void startGame();
	void pauseGame();  // Time Freeze
	void resumeGame(); // Resume from pause
	void winGame();
	void loseGame();
	void resetGame();

	//----------------------------------------------------------------------------------------------
	//  SETTINGS
	//----------------------------------------------------------------------------------------------
	void setGameMode(GameEnums::GameMode mode);
	void setDifficulty(GameEnums::Difficulty difficulty);
	void setCustomSettings(const CustomGameSettings& settings);
	void setTheme(GameEnums::Theme theme);

	//----------------------------------------------------------------------------------------------
	//  EXTENSIONS
	//----------------------------------------------------------------------------------------------
	bool useHint();	   // Returns true if hint was used successfully
	void resetHints(); // Reset hint count for new game

signals:
	//----------------------------------------------------------------------------------------------
	//  STATE SIGNALS
	//----------------------------------------------------------------------------------------------
	void stateChanged(GameEnums::GameState newState);
	void modeChanged(GameEnums::GameMode newMode);
	void difficultyChanged(GameEnums::Difficulty newDifficulty);
	void themeChanged(GameEnums::Theme newTheme);

	//----------------------------------------------------------------------------------------------
	//  EXTENSION SIGNALS
	//----------------------------------------------------------------------------------------------
	void pauseRequested();	// UI should pause display
	void resumeRequested(); // UI should resume display
	void hintRequested();	// Board should reveal a safe tile
	void hintsRemainingChanged(int remaining);

	//----------------------------------------------------------------------------------------------
	//  TIME ATTACK SIGNALS
	//----------------------------------------------------------------------------------------------
	void timeAttackStarted(int timeLimit);
	void timeRemainingChanged(int secondsRemaining);
	void timeExpired(); // Time Attack: time ran out -> lose

private slots:
	void onTimeAttackTick();

private:

	void transitionTo(GameEnums::GameState newState);
	void startTimeAttack();
	void stopTimeAttack();

private:

	GameEnums::GameState  m_state	   = GameEnums::GameState::NotStarted;
	GameEnums::GameMode	  m_mode	   = GameEnums::GameMode::Normal;
	GameEnums::Difficulty m_difficulty = GameEnums::Difficulty::Beginner;
	GameEnums::Theme	  m_theme	   = GameEnums::Theme::System;

	CustomGameSettings m_customSettings;

	// Hint extension
	int					 m_hintsRemaining = 3;
	static constexpr int MAX_HINTS		  = 3;

	// Time Attack extension
	QTimer* m_timeAttackTimer = nullptr;
	int		m_timeRemaining	  = 0;
	int		m_timeLimit		  = 0;

	// Pause state tracking
	GameEnums::GameState m_stateBeforePause = GameEnums::GameState::NotStarted;
};
