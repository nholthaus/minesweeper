#pragma once
#include <QList>
#include <QFrame>
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

public slots:

	void placeMines(Tile* firstClicked);

signals:

	void initialized();
	void victory();
	void defeat();
	void flagCountChanged(unsigned int flagCount);

private:

	void createTiles();

	void defeatAnimation();

	void setupLayout();
	void addNeighbors();
	void checkVictory();

private:

	unsigned int m_numRows;
	unsigned int m_numCols;
	unsigned int m_numMines;

	QList<QList<Tile*>> m_tiles;

	QSet<Tile*> m_mines;
	QSet<Tile*> m_correctFlags;
	QSet<Tile*> m_incorrectFlags;
	QSet<Tile*> m_revealedTiles;

	QTimer* explosionTimer;

	bool m_defeat = false;
	bool m_victory = false;
};