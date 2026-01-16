//--------------------------------------------------------------------------------------------------
//
/// @file   gameEnums.h
/// @brief  Game state, mode, and difficulty enumerations for Minesweeper extensions
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include <QObject>
#include <QString>

//--------------------------------------------------------------------------------------------------
//  NAMESPACE: GameEnums
//--------------------------------------------------------------------------------------------------
namespace GameEnums
{
	Q_NAMESPACE

	//----------------------------------------------------------------------------------------------
	/// @brief  Game states for the state machine
	//----------------------------------------------------------------------------------------------
	enum class GameState
	{
		NotStarted, ///< Game not yet started
		Playing,	///< Game in progress
		Paused,		///< Game paused (Time Freeze)
		Won,		///< Player won
		Lost		///< Player lost
	};
	Q_ENUM_NS(GameState)

	//----------------------------------------------------------------------------------------------
	/// @brief  Game modes
	//----------------------------------------------------------------------------------------------
	enum class GameMode
	{
		Normal,	   ///< Classic mode - no time limit
		TimeAttack ///< Complete within time limit
	};
	Q_ENUM_NS(GameMode)

	//----------------------------------------------------------------------------------------------
	/// @brief  Extended difficulty levels (compatible with existing HighScore::Difficulty)
	//----------------------------------------------------------------------------------------------
	enum class Difficulty
	{
		Beginner,
		Intermediate,
		Expert,
		Custom
	};
	Q_ENUM_NS(Difficulty)

	//----------------------------------------------------------------------------------------------
	/// @brief  Theme modes
	//----------------------------------------------------------------------------------------------
	enum class Theme
	{
		Light,
		Dark,
		System ///< Follow system theme
	};
	Q_ENUM_NS(Theme)

	//----------------------------------------------------------------------------------------------
	//  UTILITY FUNCTIONS
	//----------------------------------------------------------------------------------------------

	inline QString difficultyToString(Difficulty difficulty)
	{
		switch (difficulty)
		{
		case Difficulty::Beginner:
			return QStringLiteral("Beginner");
		case Difficulty::Intermediate:
			return QStringLiteral("Intermediate");
		case Difficulty::Expert:
			return QStringLiteral("Expert");
		case Difficulty::Custom:
			return QStringLiteral("Custom");
		default:
			return QStringLiteral("Unknown");
		}
	}

	inline QString gameModeToString(GameMode mode)
	{
		switch (mode)
		{
		case GameMode::Normal:
			return QStringLiteral("Normal");
		case GameMode::TimeAttack:
			return QStringLiteral("Time Attack");
		default:
			return QStringLiteral("Unknown");
		}
	}

	inline QString gameStateToString(GameState state)
	{
		switch (state)
		{
		case GameState::NotStarted:
			return QStringLiteral("Not Started");
		case GameState::Playing:
			return QStringLiteral("Playing");
		case GameState::Paused:
			return QStringLiteral("Paused");
		case GameState::Won:
			return QStringLiteral("Won");
		case GameState::Lost:
			return QStringLiteral("Lost");
		default:
			return QStringLiteral("Unknown");
		}
	}

} // namespace GameEnums
