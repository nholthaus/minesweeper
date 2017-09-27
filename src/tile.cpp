#include "tile.h"

#include <thread>
#include <chrono>

#include <QDebug>
#include <QState>
#include <QFinalState>
#include <QMouseEvent>
#include <QSizePolicy>

bool Tile::m_firstClick = false;
const QString Tile::unrevealedStyleSheet =
"Tile"
"{"
"	border: 1px solid darkgray;"
"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);"
"	border - radius: 1px;"
"}";
const QString Tile::revealedStyleSheet =
"Tile"
"{"
"	border: 1px solid lightgray;"
"}";
const QString Tile::revealedWithNumberStylesheet =
"Tile"
"{"
"	color: %1;"
"	font-weight: bold;"
"	border: 1px solid lightgray;"
"}";

QIcon Tile::blankIcon()
{
	static QIcon icon = QIcon();
	return icon;
}

QIcon Tile::flagIcon()
{
	static QIcon icon = QIcon(QPixmap(":/flag").scaled(QSize(20, 20)));
	return icon;
}

QIcon Tile::mineIcon()
{
	static QIcon icon = QIcon(QPixmap(":/mine").scaled(QSize(20, 20)));
	return icon;
}

QIcon Tile::explosionIcon()
{
	static QIcon icon = QIcon(QPixmap(":/explosion").scaled(QSize(20, 20)));
	return icon;
}

QIcon Tile::tadaIcon()
{
	static QIcon icon = QIcon(QPixmap(":/tada").scaled(QSize(22, 22)));
	return icon;
}

QIcon Tile::wrongIcon()
{
	static QIcon icon = QIcon(QPixmap(":/wrong").scaled(QSize(22, 22)));
	return icon;
}

Tile::Tile(TileLocation location, QWidget* parent /*= nullptr*/)
	: m_isMine(false)
	, m_adjacentMineCount(0)
	, m_adjacentFlaggedCount(0)
	, m_location(location)
	, QPushButton(parent)
{
	createStateMachine();
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setCheckable(true);
	setMouseTracking(true);
}

Tile::~Tile()
{
	m_firstClick = false;
	delete unrevealedState;
	delete previewState;
	delete flaggedState;
	delete revealedState;
	delete disabledState;
}

void Tile::addNeighbor(Tile* tile)
{
	m_neighbors += tile;
	connect(this, &Tile::revealNeighbors, tile, &Tile::reveal, Qt::QueuedConnection);
	connect(this, &Tile::unPreviewNeighbors, tile, &Tile::unPreview, Qt::QueuedConnection);
}

TileLocation Tile::location() const
{
	return m_location;
}

void Tile::placeMine(bool val)
{
	m_isMine = val;
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

bool Tile::isFlagged() const
{
	return m_machine.configuration().contains(flaggedState);
}

bool Tile::isRevealed() const
{
	return m_machine.configuration().contains(revealedState);
}

bool Tile::isUnrevealed() const
{
	return m_machine.configuration().contains(unrevealedState);
}

unsigned int Tile::adjacentFlaggedCount() const
{
	return m_adjacentFlaggedCount;
}

void Tile::incrementAdjacentFlaggedCount()
{
	++m_adjacentFlaggedCount;
}

void Tile::decrementAdjacentFlaggedCount()
{
	--m_adjacentFlaggedCount;
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

	m_bothClicked = false;

	if (e->buttons() == (Qt::LeftButton | Qt::RightButton))
	{
		emit bothClicked();
		m_buttons = Qt::LeftButton | Qt::RightButton;
	}
	else if (e->buttons() == Qt::LeftButton)
		m_buttons = Qt::LeftButton;
	else if (e->buttons() == Qt::RightButton)
		m_buttons = Qt::RightButton;
}

void Tile::mouseReleaseEvent(QMouseEvent* e)
{
	if (m_buttons == (Qt::LeftButton | Qt::RightButton))
		emit unClicked();
	else if (m_buttons == Qt::LeftButton)
		emit leftClicked();
	else if (m_buttons == Qt::RightButton)
		emit rightClicked();
}

void Tile::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() == (Qt::LeftButton | Qt::RightButton))
	{
		if (!this->rect().contains(this->mapFromGlobal(QCursor::pos())))
			emit unPreview();
	}
}

QSize Tile::sizeHint() const
{
	return QSize(20, 20);
}

void Tile::createStateMachine()
{
	unrevealedState = new QState;
	previewState = new QState;
	previewNeighborsState = new QState;
	flaggedState = new QState;
	revealedState = new QState;
	revealNeighborsState = new QState;
	disabledState = new QFinalState;

	unrevealedState->addTransition(this, &Tile::leftClicked, revealedState);
	unrevealedState->addTransition(this, &Tile::rightClicked, flaggedState);
	unrevealedState->addTransition(this, &Tile::reveal, revealedState);
	unrevealedState->addTransition(this, &Tile::preview, previewState);
	unrevealedState->addTransition(this, &Tile::disable, disabledState);

	previewState->addTransition(this, &Tile::reveal, revealedState);
	previewState->addTransition(this, &Tile::unPreview, unrevealedState);
	previewState->addTransition(this, &Tile::disable, disabledState);

	flaggedState->addTransition(this, &Tile::rightClicked, unrevealedState);

	revealedState->addTransition(this, &Tile::bothClicked, previewNeighborsState);

	previewNeighborsState->addTransition(this, &Tile::unClicked, revealNeighborsState);
	previewNeighborsState->addTransition(this, &Tile::unPreview, revealedState);

	revealNeighborsState->addTransition(this, &Tile::reveal, revealedState);

	connect(unrevealedState, &QState::entered, [this]()
	{
		this->setIcon(blankIcon());
		this->setStyleSheet(unrevealedStyleSheet);
			
	});

	connect(previewState, &QState::entered, [this]()
	{
		this->setStyleSheet(revealedStyleSheet);
	});	

	connect(previewNeighborsState, &QState::entered, [this]()
	{
		for (auto neighbor : m_neighbors)
			neighbor->preview();
	});

	connect(revealNeighborsState, &QState::entered, [this]()
	{
		if (m_adjacentFlaggedCount == m_adjacentMineCount && m_adjacentMineCount)
			revealNeighbors();
		else
			unPreviewNeighbors();
		emit reveal();
	});

	connect(revealedState, &QState::entered, [this]()
	{
		unPreviewNeighbors();
		this->setIcon(blankIcon());
		this->setChecked(true);
		if (!isMine())
		{
			setText();
			if (!hasAdjacentMines())
				revealNeighbors();
			emit revealed();
		}
		else
		{
			emit detonated();
			this->setStyleSheet(revealedStyleSheet);
			QPushButton::setText("");
			setIcon(mineIcon());
		}
	});

	connect(flaggedState, &QState::entered, [this]()
	{
		this->setIcon(flagIcon());
		for (auto neighbor : m_neighbors)
			neighbor->incrementAdjacentFlaggedCount();
		emit flagged(m_isMine);
	});

	connect(flaggedState, &QState::exited, [this]()
	{
		for (auto neighbor : m_neighbors)
			neighbor->decrementAdjacentFlaggedCount();
		emit unFlagged(m_isMine);
	});

	connect(disabledState, &QState::entered, [this]()
	{

	});

	m_machine.addState(unrevealedState);
	m_machine.addState(previewState);
	m_machine.addState(previewNeighborsState);
	m_machine.addState(flaggedState);
	m_machine.addState(revealedState);
	m_machine.addState(revealNeighborsState);
	m_machine.addState(disabledState);

	m_machine.setInitialState(unrevealedState);
	m_machine.start();
}

void Tile::setText()
{
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

	QPushButton::setStyleSheet(revealedWithNumberStylesheet.arg(color));
	if(m_adjacentMineCount)
		QPushButton::setText(QString::number(m_adjacentMineCount));
}

