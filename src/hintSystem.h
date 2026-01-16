//--------------------------------------------------------------------------------------------------
//
/// @file   hintSystem.h
/// @brief  Hint system extension for revealing safe tiles
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include <QList>
#include <QObject>
#include <QPair>

//--------------------------------------------------------------------------------------------------
//  FORWARD DECLARATIONS
//--------------------------------------------------------------------------------------------------
class GameBoard;
class Tile;

//--------------------------------------------------------------------------------------------------
//  CLASS: HintSystem
//--------------------------------------------------------------------------------------------------
/// @brief  Provides hints by identifying and revealing safe tiles
/// @note   This is an extension layer - it does NOT modify core gameplay logic
///         It only reads tile state and triggers reveal on safe tiles
//--------------------------------------------------------------------------------------------------
class HintSystem : public QObject
{
	Q_OBJECT

public:

	explicit HintSystem(QObject* parent = nullptr);

	void setGameBoard(GameBoard* board);

	/// @brief  Find and reveal a safe tile
	/// @return true if a hint was provided, false if no safe tile available
	bool provideHint();

	/// @brief  Get number of safe unrevealed tiles
	[[nodiscard]] int countSafeTiles() const;

signals:
	void hintProvided(int row, int col);
	void noHintAvailable();

private:

	/// @brief  Find all unrevealed, non-mine, non-flagged tiles
	[[nodiscard]] QList<Tile*> findSafeTiles() const;

	/// @brief  Prioritize tiles adjacent to revealed numbers
	[[nodiscard]] Tile* findBestHintTile() const;

private:

	GameBoard* m_board = nullptr;
};
