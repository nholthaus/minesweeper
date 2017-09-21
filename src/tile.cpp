#include "tile.h"

#include <QState>
#include <QFinalState>
#include <QMouseEvent>

Tile::Tile(TileLocation location, QWidget* parent /*= nullptr*/)
	: m_isMine(false)
	, m_adjacentMineCount(0)
	, m_location(location)
	, QPushButton(parent)
{
	createStateMachine();
}

void Tile::addNeighbor(Tile* tile)
{
	m_neighbors += tile;
}

TileLocation Tile::location() const
{
	return m_location;
}

void Tile::setMine(bool val)
{
	m_isMine = val;
	for (auto neighbor : m_neighbors)
		neighbor->incrementAdjacentMineCount();
}

bool Tile::isMine() const
{
	return m_isMine;
}

unsigned int Tile::adjacentMineCount() const
{
	return m_adjacentMineCount;
}

void Tile::incrementAdjacentMineCount()
{
	++m_adjacentMineCount;
}

void Tile::mousePressEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::LeftButton | Qt::RightButton)
		emit bothClicked();
	else if (e->buttons() == Qt::LeftButton)
		emit leftClicked();
	else if (e->buttons() == Qt::RightButton)
		emit rightClicked();
}

void Tile::createStateMachine()
{
	QState* unrevealedState = new QState;
	QState* flaggedState = new QState;
	QFinalState* revealedState = new QFinalState;

	unrevealedState->addTransition(this, &Tile::leftClicked, revealedState);
	unrevealedState->addTransition(this, &Tile::rightClicked, flaggedState);
}

