#include "tile.h"

#include <thread>
#include <chrono>

#include <QState>
#include <QFinalState>
#include <QMouseEvent>
#include <QSizePolicy>

bool Tile::m_firstClick = false;

Tile::Tile(TileLocation location, QWidget* parent /*= nullptr*/)
	: m_isMine(false)
	, m_adjacentMineCount(0)
	, m_location(location)
	, QPushButton(parent)
{
	createStateMachine();
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QFile tilesheet(":/tilesheet");
	tilesheet.open(QIODevice::ReadOnly);
	this->setStyleSheet(tilesheet.readAll());
	setCheckable(true);
}

void Tile::addNeighbor(Tile* tile)
{
	m_neighbors += tile;
}

TileLocation Tile::location() const
{
	return m_location;
}

void Tile::placeMine(bool val)
{
	m_isMine = val;
	QPushButton::setText("X");
	for (auto neighbor : m_neighbors)
		neighbor->incrementAdjacentMineCount();
}

bool Tile::isMine() const
{
	return m_isMine;
}

bool Tile::hasAdjacentMines() const
{
	return m_adjacentMineCount;
}

unsigned int Tile::adjacentMineCount() const
{
	return m_adjacentMineCount;
}

void Tile::incrementAdjacentMineCount()
{
	++m_adjacentMineCount;
}

QList<Tile*>& Tile::neighbors()
{
	return m_neighbors;
}

void Tile::mousePressEvent(QMouseEvent* e)
{
	if (!m_firstClick)
	{
		m_firstClick = true;
		emit firstClick(this);
	}

// 	if (e->buttons() == Qt::LeftButton | Qt::RightButton)
// 		emit bothClicked();
	if (e->buttons() == Qt::LeftButton)
		emit leftClicked();
	else if (e->buttons() == Qt::RightButton)
		emit rightClicked();

//	QPushButton::mousePressEvent(e);
}

QSize Tile::sizeHint() const
{
	return QSize(20, 20);
}

void Tile::createStateMachine()
{
	QState* unrevealedState = new QState;
	QState* flaggedState = new QState;
	QFinalState* revealedState = new QFinalState;

	unrevealedState->addTransition(this, &Tile::leftClicked, revealedState);
	unrevealedState->addTransition(this, &Tile::reveal, revealedState);
	unrevealedState->addTransition(this, &Tile::rightClicked, flaggedState);

	connect(revealedState, &QState::entered, [this]()
	{
		this->setChecked(true);
		if (!isMine())
			setText();
		if (!hasAdjacentMines())
			for (auto neighbor : m_neighbors)
			{
				neighbor->reveal();
			}
	});

	m_machine.addState(unrevealedState);
	m_machine.addState(flaggedState);
	m_machine.addState(revealedState);

	m_machine.setInitialState(unrevealedState);
	m_machine.start();
}

void Tile::setText()
{
	static const QString stylesheet = 
		"Tile:pressed, Tile:checked"
		"{"
		"	color: %1;"
		"	font-weight: bold;"
		"	/*margin: -1, 0, -1, -1;*/"
		"	border: 1px solid lightgray;"
		"}";

	QString color;
	switch (m_adjacentMineCount)
	{
	case 1:
		color = "blue";
		break;
	case 2:
		color = "green";
		break;
	case 3:
		color = "red";
		break;
	case 4:
		color = "midnightblue";
		break;
	case 5:
		color = "maroon";
		break;
	case 6:
		color = "darkcyan";
		break;
	case 7:
		color = "black";
		break;
	case 8:
		color = "grey";
		break;
	default:
		break;
	}

	QPushButton::setStyleSheet(stylesheet.arg(color));
	if(m_adjacentMineCount)
		QPushButton::setText(QString::number(m_adjacentMineCount));
}

