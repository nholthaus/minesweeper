#pragma once
#include <QPushButton>
#include <QList>
#include <QStateMachine>
#include <QState>
#include <QFinalState>
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
	
	bool isMine() const;
	bool isFlagged() const;
	bool isRevealed() const;
	bool isUnrevealed() const;
	bool hasAdjacentMines() const;

	unsigned int adjacentMineCount() const;
	unsigned int adjacentFlaggedCount() const;

	QList<Tile*>& neighbors();

	virtual void mousePressEvent(QMouseEvent* e) override;
	virtual void mouseReleaseEvent(QMouseEvent* e) override;
	virtual QSize sizeHint() const override;

	static QIcon blankIcon();
	static QIcon flagIcon();
	static QIcon mineIcon();
	static QIcon explosionIcon();
	static QIcon tadaIcon();

public slots:

	void incrementAdjacentFlaggedCount();
	void decrementAdjacentFlaggedCount();
	void incrementAdjacentMineCount();

signals:

	void firstClick(Tile*);
	void leftClicked();
	void rightClicked();
	void bothClicked();
	void unClicked();
	void detonated();
	void reveal();
	void revealed();
	void revealNeighbors();
	void preview();
	void unPreview();
	void flagged(bool);
	void unFlagged(bool);
	void disable();

private:

	void createStateMachine();
	void setText();

private:

	bool m_isMine;
	unsigned int m_adjacentMineCount;
	unsigned int m_adjacentFlaggedCount;
	TileLocation m_location;
	QList<Tile*> m_neighbors;
	QSignalMapper m_mapper;
	bool m_bothClicked;
	static bool m_firstClick;
	
	static const QString unrevealedStyleSheet;
	static const QString revealedStyleSheet;
	static const QString revealedWithNumberStylesheet;

	QStateMachine m_machine;
	QState* unrevealedState;
	QState* previewState;
	QState* flaggedState;
	QFinalState* revealedState;
	QFinalState* disabledState;
};