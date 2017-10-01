#pragma once

#include "tile.h"
#include "gameboard.h"
#include "mineCounter.h"
#include "minetimer.h"

#include <QAction>
#include <QActionGroup>
#include <QMenu>
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

	void setupMenus();

signals:

	void startGame();
	void victory();
	void defeat();
	void startNewGame();

private:

	void initialize();
	void setupStateMachine();
	void saveSettings();
	void loadSettings();

private:

	QFrame* mainFrame;
	GameBoard* gameBoard;
	MineCounter* mineCounter;
	MineTimer* mineTimer;
	QPushButton* newGame;

	QMenu* gameMenu;
	QAction* newGameAction;
	QMenu* difficultyMenu;
	QActionGroup* difficultyActionGroup;
	QAction* beginnerAction;
	QAction* intermediateAction;
	QAction* expertAction;
	QAction* customAction;
	QAction* highScoreAction;
	QAction* exitAction;

	QMenu* helpMenu;
	QAction* aboutAction;

	QTimer* gameClock;

	QStateMachine* m_machine;
	QState* unstartedState;
	QState* inProgressState;
	QState* victoryState;
	QState* defeatState;

	Tile* firstClicked;

	quint32 numRows;
	quint32 numCols;
	quint32 numMines;
};