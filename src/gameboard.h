#pragma once
#include <QVector>
#include <QWidget>

#include "tile.h"

class GameBoard : public QWidget
{
	Q_OBJECT

public:

	GameBoard(unsigned int numRows, unsigned int numCols, unsigned int numMines, QWidget* parent = nullptr);

	unsigned int NumCols() const { return m_numCols; }
	unsigned int NumRows() const { return m_numRows; }

private:

	void createTiles();
	void addNeighbors();
	void placeMines(unsigned int numMines, Tile* firstClicked);

private:

	unsigned int m_numRows;
	unsigned int m_numCols;

	QVector<QVector<Tile*>> m_tiles;

};