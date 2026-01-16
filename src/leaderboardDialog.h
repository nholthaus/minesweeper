//--------------------------------------------------------------------------------------------------
//
/// @file   leaderboardDialog.h
/// @brief  Dialog for viewing leaderboard with mode/difficulty filtering
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include "gameEnums.h"
#include "leaderboard.h"

#include <QComboBox>
#include <QDialog>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTableView>

//--------------------------------------------------------------------------------------------------
//  CLASS: LeaderboardDialog
//--------------------------------------------------------------------------------------------------
class LeaderboardDialog : public QDialog
{
	Q_OBJECT

public:

	explicit LeaderboardDialog(Leaderboard* leaderboard, QWidget* parent = nullptr);

	void setActiveTab(GameEnums::Difficulty difficulty);
	void setActiveMode(GameEnums::GameMode mode);

private slots:
	void onModeChanged(int index);
	void onTabChanged(int index);
	void updateTable();

private:

	void setupUI();
	void populateTable(GameEnums::Difficulty difficulty, GameEnums::GameMode mode);

private:

	Leaderboard* m_leaderboard;
	QComboBox*	 m_modeComboBox;
	QTabWidget*	 m_tabWidget;

	// One table per difficulty
	QTableView* m_beginnerTable;
	QTableView* m_intermediateTable;
	QTableView* m_expertTable;
	QTableView* m_customTable;

	QStandardItemModel* m_beginnerModel;
	QStandardItemModel* m_intermediateModel;
	QStandardItemModel* m_expertModel;
	QStandardItemModel* m_customModel;
};
