#pragma once
#include <QPushButton>
#include <QList>
#include <QStateMachine>
#include <QSignalMapper>

struct TileLocation
{
	unsigned int row; 
	unsigned int column;
};

class Tile : public QPushButton
{
	Q_OBJECT
public:

	Tile(TileLocation location, QWidget* parent = nullptr);

	void addNeighbor(Tile* tile);
	TileLocation location() const;
	void placeMine(bool val);
	
	inline bool isMine() const;
	inline bool hasAdjacentMines() const;

	unsigned int adjacentMineCount() const;
	void incrementAdjacentMineCount();

	QList<Tile*>& neighbors();

	virtual void mousePressEvent(QMouseEvent* e) override;
	virtual QSize sizeHint() const override;

signals:

	void firstClick(Tile*);
	void leftClicked();
	void rightClicked();
	void bothClicked();
	void detonated();
	void reveal();

private:

	bool m_isMine;
	unsigned int m_adjacentMineCount;
	TileLocation m_location;
	QList<Tile*> m_neighbors;
	QSignalMapper m_mapper;
	static bool m_firstClick;

private:

	void createStateMachine();

	void setText();

	QStateMachine m_machine;

};