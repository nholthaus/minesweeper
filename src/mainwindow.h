#pragma once

#include "tile.h"
#include "gameboard.h"
#include "mineCounter.h"
#include "minetimer.h"

#include <QFrame>
#include <QPushButton>
#include <QMainWindow>
#include <QStateMachine>
#include <QState>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:

	MainWindow(QWidget* parent = nullptr);

signals:

	void startGame();
	void victory();
	void defeat();
	void startNewGame();

private:

	void initialize();
	void setupStateMachine();

private:

	QFrame* mainFrame;
	GameBoard* gameBoard;
	MineCounter* mineCounter;
	MineTimer* mineTimer;
	QPushButton* newGame;

	QTimer* gameClock;

	QStateMachine* m_machine;
	QState* unstartedState;
	QState* inProgressState;
	QState* victoryState;
	QState* defeatState;

	Tile* firstClicked;

};