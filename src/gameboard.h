#pragma once
#include <QFrame>
#include <QList>
#include <QSet>

#include "tile.h"

class GameBoard : public QFrame
{
	Q_OBJECT

public:

	GameBoard(unsigned int numRows, unsigned int numCols, unsigned int numMines, QWidget* parent = nullptr);

	unsigned int numCols() const { return m_numCols; }
	unsigned int numRows() const { return m_numRows; }
	unsigned int numMines() const { return m_numMines; }

	// Extension accessor - read-only access to tiles for hint system
	// Does NOT modify core gameplay logic
	Tile* tileAt(unsigned int row, unsigned int col) const
	{
		if (row < m_numRows && col < m_numCols && row < static_cast<unsigned int>(m_tiles.size()))
		{
			if (col < static_cast<unsigned int>(m_tiles[row].size()))
				return m_tiles[row][col];
		}
		return nullptr;
	}

public slots:

	void placeMines(Tile* firstClicked);
	void setTheme(Qt::ColorScheme colorScheme);

signals:

	void initialized();
	void victory();
	void defeat();
	void flagCountChanged(unsigned int flagCount);

private:

	QList<QPair<unsigned int, unsigned int>> generateTileIndices(unsigned int rows, unsigned int columns);

	void createTile(QPair<unsigned int, unsigned int> tileIndex);
	void createTiles();

	void defeatAnimation();

	void setupLayout();
	void addNeighbor(QPair<unsigned int, unsigned int> tileIndex);
	void addNeighbors();
	void checkVictory();

private:

	unsigned int m_numRows;
	unsigned int m_numCols;
	unsigned int m_numMines;

	QList<QList<Tile*>>						 m_tiles;
	QList<QPair<unsigned int, unsigned int>> m_tileIndices;

	QSet<Tile*> m_mines;
	QSet<Tile*> m_correctFlags;
	QSet<Tile*> m_incorrectFlags;
	QSet<Tile*> m_revealedTiles;

	QTimer* explosionTimer;

	bool m_defeat  = false;
	bool m_victory = false;
};
