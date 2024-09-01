#include "gameboard.h"

#include <algorithm>
#include <random>

#include <QGridLayout>
#include <QSet>
#include <QTimer>
#include <QFuture>
#include <QtConcurrent>

GameBoard::GameBoard(unsigned int numRows, unsigned int numCols, unsigned int numMines, QWidget* parent /*= nullptr*/)
	: m_numRows(numRows)
	, m_numCols(numCols)
	, m_numMines(numMines)
	, QFrame(parent)
	, explosionTimer(new QTimer(this))
{
	setupLayout();
	createTiles();
	addNeighbors();

	connect(this, &GameBoard::victory, [this]()
	{
		explosionTimer->setProperty("victory", true);
	});
	connect(this, &GameBoard::defeat, [this]()
	{
		explosionTimer->setProperty("victory", false);
	});

	connect(explosionTimer, &QTimer::timeout, [this]()
	{
		if (m_mines.isEmpty())
		{
			explosionTimer->stop();
			return;
		}

		Tile* mine = m_mines.values().front();
		m_mines.remove(mine);

		if (explosionTimer->property("victory").toBool())
			mine->setIcon(Tile::tadaIcon());
		else
		{
			if (!m_correctFlags.contains(mine))
				mine->setIcon(Tile::explosionIcon());
		}
	});
}

void GameBoard::setupLayout()
{
	this->setAttribute(Qt::WA_LayoutUsesWidgetRect);
	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	auto layout = new QGridLayout;

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSizeConstraint(QLayout::SetFixedSize);

	this->setLayout(layout);
}

QList<QPair<unsigned int, unsigned int>> GameBoard::generateTileIndices(unsigned int rows, unsigned int columns)
{
	QList<QPair<unsigned int, unsigned int>> indices;

	auto generate = [&indices](unsigned int rMax, unsigned int cMax, unsigned int startR, unsigned int endR)
	{
		QList<QPair<unsigned int, unsigned int>> localIndices;
		for (unsigned int r = startR; r < endR; ++r)
		{
			for (unsigned int c = 0; c < cMax; ++c)
			{
				localIndices.append(qMakePair(r, c));
			}
		}
		return localIndices;
	};

	unsigned int                                             threadCount = QThread::idealThreadCount();
	QList<QFuture<QList<QPair<unsigned int, unsigned int>>>> futures;
	unsigned int                                             rowsPerThread = rows / threadCount;

	for (unsigned int i = 0; i < threadCount; ++i)
	{
		unsigned int startR = i * rowsPerThread;
		unsigned int endR   = (i == threadCount - 1) ? rows : startR + rowsPerThread;
		futures.append(QtConcurrent::run(generate, rows, columns, startR, endR));
	}

	for (auto& future : futures)
	{
		indices += future.result();
	}

	return indices;
}

void GameBoard::createTile(QPair<unsigned int, unsigned int> tileIndex)
{
	auto r = tileIndex.first;
	auto c = tileIndex.second;

	// add a new tile to the row
	m_tiles[r] += new Tile({r, c}, this);
	static_cast<QGridLayout*>(this->layout())->addWidget(m_tiles[r][c], r, c);
	connect(m_tiles[r][c], &Tile::firstClick, this, &GameBoard::placeMines);
	connect(m_tiles[r][c], &Tile::flagged, [this, tile = m_tiles[r][c]](bool isMine)
	{
		if (isMine)
			m_correctFlags.insert(tile);
		else
			m_incorrectFlags.insert(tile);
		checkVictory();
	});
	connect(m_tiles[r][c], &Tile::unFlagged, [this, tile = m_tiles[r][c]](bool isMine)
	{
		if (isMine)
			m_correctFlags.remove(tile);
		else
			m_incorrectFlags.remove(tile);
		checkVictory();
	});
	connect(m_tiles[r][c], &Tile::revealed, [this, tile = m_tiles[r][c]]()
	{
		m_revealedTiles.insert(tile);
		checkVictory();
	});
	connect(m_tiles[r][c], &Tile::detonated, this, &GameBoard::defeatAnimation);
	connect(this, &GameBoard::defeat, m_tiles[r][c], &Tile::disable);
	connect(this, &GameBoard::victory, m_tiles[r][c], &Tile::disable);
}

void GameBoard::createTiles()
{
	m_tiles.resize(m_numRows);
	m_tileIndices = generateTileIndices(m_numRows, m_numCols);

	for (auto& tileIndex : m_tileIndices)
	{
		createTile(tileIndex);
	}

	m_tiles[0][0]->setDown(true);
}

void GameBoard::addNeighbor(QPair<unsigned int, unsigned int> tileIndex)
{
	auto r = tileIndex.first;
	auto c = tileIndex.second;

	// add a new tile to the row
	auto* tile    = m_tiles[r][c];
	auto  lastRow = m_numRows - 1;
	auto  lastCol = m_numCols - 1;

	if (r && c)
		tile->addNeighbor(m_tiles[r - 1][c - 1]); // top left
	if (r)
		tile->addNeighbor(m_tiles[r - 1][c]); // top
	if (r && c < lastCol)
		tile->addNeighbor(m_tiles[r - 1][c + 1]); // top right
	if (c < lastCol)
		tile->addNeighbor(m_tiles[r][c + 1]); // right
	if (r < lastRow && c < lastCol)
		tile->addNeighbor(m_tiles[r + 1][c + 1]); // bottom right
	if (r < lastRow)
		tile->addNeighbor(m_tiles[r + 1][c]); // bottom
	if (r < lastRow && c)
		tile->addNeighbor(m_tiles[r + 1][c - 1]); // bottom left
	if (c)
		tile->addNeighbor(m_tiles[r][c - 1]); // left
}

void GameBoard::addNeighbors()
{
	QtConcurrent::blockingMap(m_tileIndices, [this](QPair<unsigned int, unsigned int> p) { this->addNeighbor(p); });
}

void GameBoard::checkVictory()
{
	// when victory is accomplished, there may be more than 1 tile that are unrevealed, so make sure
	// to only run this code once
	if (!m_victory)
	{
		emit flagCountChanged(m_correctFlags.size() + m_incorrectFlags.size());
		if ((m_revealedTiles.size() == m_numCols * m_numRows - m_numMines) && m_incorrectFlags.isEmpty())
		{
			emit victory();
			m_victory = true;
			QTimer::singleShot(0, explosionTimer, [this]()
			{
				explosionTimer->start(25);
			});
		}
	}
}

void GameBoard::defeatAnimation()
{
	Tile* sender = dynamic_cast<Tile*>(this->sender());
	QTimer::singleShot(350, this, [sender]()
	{
		sender->setIcon(Tile::explosionIcon());
	});
	QTimer::singleShot(500, this, [this]()
	{
		for (auto wrong : m_incorrectFlags)
		{
			wrong->setIcon(Tile::wrongIcon());
		}
		for (auto mine : m_mines)
		{
			disconnect(mine, &Tile::detonated, this, &GameBoard::defeatAnimation);
			if (!mine->isFlagged())
				mine->reveal();
		}
		emit defeat();
	});

	QTimer::singleShot(1000, explosionTimer, [this]()
	{
		explosionTimer->start(25);
	});
}

void GameBoard::placeMines(Tile* firstClicked)
{
	// get a flat list of tiles
	QList<Tile*> tiles;
	QSet<Tile*>  doneUse;
	doneUse += firstClicked;
	doneUse += QSet<Tile*>(firstClicked->neighbors().cbegin(), firstClicked->neighbors().cend());

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
	std::mt19937       g(rd());

	std::shuffle(tiles.begin(), tiles.end(), g);

	for (unsigned int i = 0; i < m_numMines; ++i)
	{
		tiles[i]->placeMine(true);
		m_mines.insert(tiles[i]);
	}

	emit initialized();
}