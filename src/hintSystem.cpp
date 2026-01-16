//--------------------------------------------------------------------------------------------------
//
/// @file   hintSystem.cpp
/// @brief  Implementation of HintSystem
//
//--------------------------------------------------------------------------------------------------

#include "hintSystem.h"
#include "gameboard.h"
#include "tile.h"

#include <QRandomGenerator>

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
HintSystem::HintSystem(QObject* parent)
	: QObject(parent)
{
}

//--------------------------------------------------------------------------------------------------
//  SET GAME BOARD
//--------------------------------------------------------------------------------------------------
void HintSystem::setGameBoard(GameBoard* board) { m_board = board; }

//--------------------------------------------------------------------------------------------------
//  PROVIDE HINT
//--------------------------------------------------------------------------------------------------
bool HintSystem::provideHint()
{
	if (!m_board)
	{
		emit noHintAvailable();
		return false;
	}

	Tile* hintTile = findBestHintTile();

	if (hintTile)
	{
		// Trigger the reveal signal on the tile
		// This uses the existing tile reveal mechanism without modifying core logic
		emit hintTile->reveal();
		emit hintProvided(hintTile->location().row, hintTile->location().column);
		return true;
	}

	emit noHintAvailable();
	return false;
}

//--------------------------------------------------------------------------------------------------
//  COUNT SAFE TILES
//--------------------------------------------------------------------------------------------------
int HintSystem::countSafeTiles() const { return findSafeTiles().size(); }

//--------------------------------------------------------------------------------------------------
//  FIND SAFE TILES
//--------------------------------------------------------------------------------------------------
QList<Tile*> HintSystem::findSafeTiles() const
{
	QList<Tile*> safeTiles;

	if (!m_board)
		return safeTiles;

	// Use the tileAt accessor to iterate through all tiles
	// This reads tile state without modifying core logic
	for (unsigned int r = 0; r < m_board->numRows(); ++r)
	{
		for (unsigned int c = 0; c < m_board->numCols(); ++c)
		{
			Tile* tile = m_board->tileAt(r, c);
			if (tile && !tile->isMine() && !tile->isRevealed() && !tile->isFlagged())
			{
				safeTiles.append(tile);
			}
		}
	}

	return safeTiles;
}

//--------------------------------------------------------------------------------------------------
//  FIND BEST HINT TILE
//--------------------------------------------------------------------------------------------------
Tile* HintSystem::findBestHintTile() const
{
	QList<Tile*> safeTiles = findSafeTiles();

	if (safeTiles.isEmpty())
		return nullptr;

	// Priority 1: Tiles adjacent to revealed numbered tiles (most helpful)
	QList<Tile*> adjacentToNumbers;

	for (Tile* tile : safeTiles)
	{
		for (Tile* neighbor : tile->neighbors())
		{
			if (neighbor->isRevealed() && neighbor->hasAdjacentMines())
			{
				adjacentToNumbers.append(tile);
				break;
			}
		}
	}

	if (!adjacentToNumbers.isEmpty())
	{
		// Pick a random one from the best candidates
		int index = QRandomGenerator::global()->bounded(adjacentToNumbers.size());
		return adjacentToNumbers[index];
	}

	// Priority 2: Any safe tile (random)
	int index = QRandomGenerator::global()->bounded(safeTiles.size());
	return safeTiles[index];
}
