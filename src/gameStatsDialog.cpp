// ---------------------------------------------------------------------------------------------------------------------
//
/// @file       gameStatsDialog.cpp
/// @author     Nic Holthaus
/// @date       9/25/2024
/// @copyright  (c) 2024 STR. The use of this software is subject to the terms and conditions outlined
///             in the LICENSE file. By using this software, the user agrees to be bound by the terms and
///             conditions set forth in the LICENSE file.
//
// ---------------------------------------------------------------------------------------------------------------------
//
/// @details    Implementation file for `gameStatsDialog.h`.
//
// ---------------------------------------------------------------------------------------------------------------------

//----------------------------
//  INCLUDES
//----------------------------

#include "gameStatsDialog.h"

#include <QSizePolicy>
#include <QTabBar>

#include <utility>

//======================================================================================================================
//      MEMBER FUNCTIONS
//======================================================================================================================

GameStatsDialog::GameStatsDialog(GameStats stats, QWidget* parent)
	: QDialog{parent}
	, m_stats{std::move(stats)}
	, m_tabWidget{new QTabWidget(this)}
{
	m_tabWidget->addTab(new QFrame, "Beginner");
	m_tabWidget->addTab(new QFrame, "Intermediate");
	m_tabWidget->addTab(new QFrame, "Expert");

	m_tabWidget->tabBar()->setUsesScrollButtons(false);

	this->setLayout(new QVBoxLayout());
	this->layout()->addWidget(m_tabWidget);
	this->setWindowTitle("Statistics");
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// For each difficulty
	for (auto difficulty : {HighScore::beginner, HighScore::intermediate, HighScore::expert})
	{
		auto* tab = m_tabWidget->widget(difficulty);
		tab->setLayout(new QGridLayout(this));

		auto* tabLayout = dynamic_cast<QGridLayout*>(tab->layout());

		// List all the stats
		int row = 0;
		tabLayout->addWidget(new QLabel("Games Played:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.played(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QFrame(this), ++row, 0);
		tabLayout->addWidget(new QLabel("Wins:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.wins(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("Win Rate:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.winRate(difficulty), 'f', 1), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("%", this), row, 2);
		tabLayout->addWidget(new QLabel("Avg. Time to Win:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.averageTimeToWin(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("seconds", this), row, 2);
		tabLayout->addWidget(new QFrame(this), ++row, 0);
		tabLayout->addWidget(new QLabel("Losses:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.losses(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("Loss Rate:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.lossRate(difficulty), 'f', 1), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("%", this), row, 2);
		tabLayout->addWidget(new QLabel("Avg. Time to Loss:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.averageTimeToLoss(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("seconds", this), row, 2);
		tabLayout->addWidget(new QFrame(this), ++row, 0);
		tabLayout->addWidget(new QLabel("Forfeits:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.forfeits(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("Forfeit Rate:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.forfeitRate(difficulty), 'f', 1), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("%", this), row, 2);
		tabLayout->addWidget(new QLabel("Avg. Time to Forfeit:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.averageTimeToForfeit(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("seconds", this), row, 2);
	}

	this->adjustSize();
}

void GameStatsDialog::setActiveTab(const QString& activeTab) const
{
	for (int index = 0; index < m_tabWidget->count(); index++)
	{
		if (m_tabWidget->tabText(index) == activeTab)
		{
			m_tabWidget->setCurrentIndex(index);
			return;
		}
	}
}