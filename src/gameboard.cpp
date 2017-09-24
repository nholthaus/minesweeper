#include "gameboard.h"

#include <algorithm>
#include <random>

#include <QGridLayout>
#include <QSpacerItem>
#include <QSet>

GameBoard::GameBoard(unsigned int numRows, unsigned int numCols, unsigned int numMines, QWidget* parent /*= nullptr*/)
	: m_numRows(numRows)
	, m_numCols(numCols)
	, m_numMines(numMines)
	, QFrame(parent)
{
	setupLayout();
	createTiles();
	addNeighbors();
}

void GameBoard::setupLayout()
{
	this->setAttribute(Qt::WA_LayoutUsesWidgetRect);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	auto layout = new QGridLayout;

	layout->setSpacing(0);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSizeConstraint(QLayout::SetFixedSize);

	this->setLayout(layout);
}

void GameBoard::createTiles()
{
	for (unsigned int r = 0; r < m_numRows; ++r)
	{
		m_tiles += QList<Tile*>{};
		for (unsigned int c = 0; c < m_numCols; ++c)
		{
			// add a new tile to the row
			m_tiles[r] += new Tile({ r, c }, this);
			static_cast<QGridLayout*>(this->layout())->addWidget(m_tiles[r][c], r, c);
			connect(m_tiles[r][c], &Tile::firstClick, this, &GameBoard::placeMines);
		}
	}
	m_tiles[0][0]->setDown(true);
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

void GameBoard::placeMines(Tile* firstClicked)
{
	// get a flat list of tiles
	QList<Tile*> tiles;
	QSet<Tile*> doneUse;
	doneUse += firstClicked;
	doneUse += QSet<Tile*>::fromList(firstClicked->neighbors());

	for (unsigned int r = 0; r < m_numRows; ++r)
	{
		m_tiles += QList<Tile*>{};
		for (unsigned int c = 0; c < m_numCols; ++c)
		{
			// add a new tile to the row (unless it's the first one clicked)
			if (auto tile = m_tiles[r][c]; !doneUse.contains(tile))
				tiles += tile;
		}
	}

	// shuffle it
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(tiles.begin(), tiles.end(), g);

	for (unsigned int i = 0; i < m_numMines; ++i)
		tiles[i]->placeMine(true);
}
