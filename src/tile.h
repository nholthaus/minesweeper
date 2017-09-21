#pragma once
#include <QPushButton>
#include <QVector>
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
	void setMine(bool val);
	bool isMine() const;

	unsigned int adjacentMineCount() const;
	void incrementAdjacentMineCount();

	virtual void mousePressEvent(QMouseEvent* e) override;

signals:

	void leftClicked();
	void rightClicked();
	void bothClicked();
	void detonated();

private:

	bool m_isMine;
	unsigned int m_adjacentMineCount;
	TileLocation m_location;
	QVector<Tile*> m_neighbors;
	QSignalMapper m_mapper;

private:

	void createStateMachine();

	QStateMachine m_machine;

};