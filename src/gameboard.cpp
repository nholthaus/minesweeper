#include "gameboard.h"

#include <algorithm>
#include <random>

GameBoard::GameBoard(unsigned int numRows, unsigned int numCols, unsigned int numMines, QWidget* parent /*= nullptr*/)
	: m_numRows(numRows)
	, m_numCols(numCols)
	, QWidget(parent)
{
	createTiles();
	addNeighbors();
	placeMines(numMines, TODO);
}

void GameBoard::createTiles()
{
	for (unsigned int r = 0; r < m_numRows; ++r)
	{
		m_tiles += QVector<Tile*>{};
		for (unsigned int c = 0; c < m_numCols; ++c)
		{
			// add a new tile to the row
			m_tiles[r] += new Tile({ r, c }, this);
		}
	}
}

void GameBoard::addNeighbors()
{
	for (unsigned int r = 0; r < m_numRows; ++r)
	{
		for (unsigned int c = 0; c < m_numCols; ++c)
		{
			// add a new tile to the row
			auto* tile = m_tiles[r][c];
			auto lastRow = m_numRows - 1;
			auto lastCol = m_numCols - 1;

			if (r && c)						tile->addNeighbor(m_tiles[r - 1][c - 1]);				// top left
			if (r)							tile->addNeighbor(m_tiles[r - 1][c]);					// top
			if (r && c < lastCol)			tile->addNeighbor(m_tiles[r - 1][c + 1]);				// top right
			if (c < lastCol)				tile->addNeighbor(m_tiles[r][c + 1]);					// right
			if (r < lastRow && c < lastCol)	tile->addNeighbor(m_tiles[r + 1][c + 1]);				// bottom right
			if (r < lastRow)				tile->addNeighbor(m_tiles[r + 1][c]);					// bottom
			if (r < lastRow && c)			tile->addNeighbor(m_tiles[r + 1][c - 1]);				// bottom left
			if (c)							tile->addNeighbor(m_tiles[r][c - 1]);					// left
		}
	}
}

void GameBoard::placeMines(unsigned int numMines, Tile* firstClicked)
{
	// get a flat list of tiles
	QVector<Tile*> tiles;
	for (unsigned int r = 0; r < m_numRows; ++r)
	{
		m_tiles += QVector<Tile*>{};
		for (unsigned int c = 0; c < m_numCols; ++c)
		{
			// add a new tile to the row (unless it's the first one clicked)
			if (auto tile = m_tiles[r][c]; tile != firstClicked)
				tiles += tile;
		}
	}

	// shuffle it
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(tiles.begin(), tiles.end(), g);

	for (unsigned int i = 0; i < numMines; ++i)
		tiles[i]->setMine(true);

}
