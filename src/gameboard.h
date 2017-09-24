#pragma once
#include <QList>
#include <QFrame>

#include "tile.h"

class GameBoard : public QFrame
{
	Q_OBJECT

public:

	GameBoard(unsigned int numRows, unsigned int numCols, unsigned int numMines, QWidget* parent = nullptr);

	unsigned int NumCols() const { return m_numCols; }
	unsigned int NumRows() const { return m_numRows; }

private:

	void createTiles();

	void setupLayout();

	void addNeighbors();
	void placeMines(Tile* firstClicked);

private:

	unsigned int m_numRows;
	unsigned int m_numCols;
	unsigned int m_numMines;

	QList<QList<Tile*>> m_tiles;

};