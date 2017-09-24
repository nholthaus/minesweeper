#pragma once

#include "tile.h"
#include "gameboard.h"
#include "mineCounter.h"

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

private:

	void initialize();
	void setupStateMachine();

private:

	QFrame* mainFrame;
	GameBoard* gameBoard;
	MineCounter* mineCounter;
	QPushButton* newGame;

	QStateMachine* m_machine;
	QState* unstartedState;
	QState* inProgressState;
	QState* gameOverState;
	QState* victoryState;
	QState* defeatState;

	Tile* firstClicked;

};