//--------------------------------------------------------------------------------------------------
//
/// @file   gameController.cpp
/// @brief  Implementation of the GameController class
//
//--------------------------------------------------------------------------------------------------

#include "gameController.h"

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
GameController::GameController(QObject* parent)
	: QObject(parent)
	, m_timeAttackTimer(new QTimer(this))
{
	m_timeAttackTimer->setInterval(1000);
	connect(m_timeAttackTimer, &QTimer::timeout, this, &GameController::onTimeAttackTick);
}

//--------------------------------------------------------------------------------------------------
//  DIFFICULTY CONVERSION
//--------------------------------------------------------------------------------------------------
HighScore::Difficulty GameController::toHighScoreDifficulty(GameEnums::Difficulty difficulty)
{
	switch (difficulty)
	{
	case GameEnums::Difficulty::Beginner:
		return HighScore::beginner;
	case GameEnums::Difficulty::Intermediate:
		return HighScore::intermediate;
	case GameEnums::Difficulty::Expert:
		return HighScore::expert;
	case GameEnums::Difficulty::Custom:
		return HighScore::custom;
	default:
		return HighScore::beginner;
	}
}

GameEnums::Difficulty GameController::fromHighScoreDifficulty(HighScore::Difficulty difficulty)
{
	switch (difficulty)
	{
	case HighScore::beginner:
		return GameEnums::Difficulty::Beginner;
	case HighScore::intermediate:
		return GameEnums::Difficulty::Intermediate;
	case HighScore::expert:
		return GameEnums::Difficulty::Expert;
	case HighScore::custom:
		return GameEnums::Difficulty::Custom;
	default:
		return GameEnums::Difficulty::Beginner;
	}
}

//--------------------------------------------------------------------------------------------------
//  STATE TRANSITIONS
//--------------------------------------------------------------------------------------------------
void GameController::transitionTo(GameEnums::GameState newState)
{
	if (m_state != newState)
	{
		m_state = newState;
		emit stateChanged(newState);
	}
}

void GameController::startGame()
{
	if (m_state == GameEnums::GameState::NotStarted)
	{
		transitionTo(GameEnums::GameState::Playing);

		if (m_mode == GameEnums::GameMode::TimeAttack)
		{
			startTimeAttack();
		}
	}
}

void GameController::pauseGame()
{
	if (m_state == GameEnums::GameState::Playing)
	{
		m_stateBeforePause = m_state;
		transitionTo(GameEnums::GameState::Paused);

		if (m_timeAttackTimer->isActive())
		{
			m_timeAttackTimer->stop();
		}

		emit pauseRequested();
	}
}

void GameController::resumeGame()
{
	if (m_state == GameEnums::GameState::Paused)
	{
		transitionTo(GameEnums::GameState::Playing);

		if (m_mode == GameEnums::GameMode::TimeAttack)
		{
			m_timeAttackTimer->start();
		}

		emit resumeRequested();
	}
}

void GameController::winGame()
{
	if (m_state == GameEnums::GameState::Playing)
	{
		stopTimeAttack();
		transitionTo(GameEnums::GameState::Won);
	}
}

void GameController::loseGame()
{
	if (m_state == GameEnums::GameState::Playing)
	{
		stopTimeAttack();
		transitionTo(GameEnums::GameState::Lost);
	}
}

void GameController::resetGame()
{
	stopTimeAttack();
	resetHints();
	transitionTo(GameEnums::GameState::NotStarted);
}

//--------------------------------------------------------------------------------------------------
//  SETTINGS
//--------------------------------------------------------------------------------------------------
void GameController::setGameMode(GameEnums::GameMode mode)
{
	if (m_mode != mode)
	{
		m_mode = mode;
		emit modeChanged(mode);
	}
}

void GameController::setDifficulty(GameEnums::Difficulty difficulty)
{
	if (m_difficulty != difficulty)
	{
		m_difficulty = difficulty;
		emit difficultyChanged(difficulty);
	}
}

void GameController::setCustomSettings(const CustomGameSettings& settings) { m_customSettings = settings; }

void GameController::setTheme(GameEnums::Theme theme)
{
	if (m_theme != theme)
	{
		m_theme = theme;
		emit themeChanged(theme);
	}
}

//--------------------------------------------------------------------------------------------------
//  HINT EXTENSION
//--------------------------------------------------------------------------------------------------
bool GameController::useHint()
{
	if (m_state != GameEnums::GameState::Playing)
		return false;

	if (m_hintsRemaining <= 0)
		return false;

	--m_hintsRemaining;
	emit hintsRemainingChanged(m_hintsRemaining);
	emit hintRequested();
	return true;
}

void GameController::resetHints()
{
	m_hintsRemaining = MAX_HINTS;
	emit hintsRemainingChanged(m_hintsRemaining);
}

//--------------------------------------------------------------------------------------------------
//  TIME ATTACK EXTENSION
//--------------------------------------------------------------------------------------------------
void GameController::startTimeAttack()
{
	m_timeLimit		= TimeAttackSettings::getTimeLimit(m_difficulty);
	m_timeRemaining = m_timeLimit;

	emit timeAttackStarted(m_timeLimit);
	emit timeRemainingChanged(m_timeRemaining);

	m_timeAttackTimer->start();
}

void GameController::stopTimeAttack()
{
	if (m_timeAttackTimer->isActive())
	{
		m_timeAttackTimer->stop();
	}
}

void GameController::onTimeAttackTick()
{
	if (m_timeRemaining > 0)
	{
		--m_timeRemaining;
		emit timeRemainingChanged(m_timeRemaining);

		if (m_timeRemaining == 0)
		{
			emit timeExpired();
			loseGame();
		}
	}
}
