//--------------------------------------------------------------------------------------------------
//
/// @file   leaderboardDialog.cpp
/// @brief  Implementation of LeaderboardDialog
//
//--------------------------------------------------------------------------------------------------

#include "leaderboardDialog.h"
#include "themeManager.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
LeaderboardDialog::LeaderboardDialog(Leaderboard* leaderboard, QWidget* parent)
	: QDialog(parent)
	, m_leaderboard(leaderboard)
{
	setWindowTitle(tr("Leaderboard"));
	setMinimumSize(500, 400);
	setupUI();
	updateTable();

	setStyleSheet(ThemeManager::instance().styleSheet());
}

//--------------------------------------------------------------------------------------------------
//  SETUP UI
//--------------------------------------------------------------------------------------------------
void LeaderboardDialog::setupUI()
{
	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(15);
	mainLayout->setContentsMargins(20, 20, 20, 20);

	// Title
	auto* titleLabel = new QLabel(tr("🏆 Leaderboard"), this);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 10px;");
	mainLayout->addWidget(titleLabel);

	// Mode selector
	auto* modeLayout = new QHBoxLayout;
	auto* modeLabel	 = new QLabel(tr("Game Mode:"), this);
	m_modeComboBox	 = new QComboBox(this);
	m_modeComboBox->addItem(tr("Normal"), static_cast<int>(GameEnums::GameMode::Normal));
	m_modeComboBox->addItem(tr("Time Attack"), static_cast<int>(GameEnums::GameMode::TimeAttack));
	connect(m_modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LeaderboardDialog::onModeChanged);

	modeLayout->addStretch();
	modeLayout->addWidget(modeLabel);
	modeLayout->addWidget(m_modeComboBox);
	modeLayout->addStretch();
	mainLayout->addLayout(modeLayout);

	// Tab widget for difficulties
	m_tabWidget = new QTabWidget(this);
	connect(m_tabWidget, &QTabWidget::currentChanged, this, &LeaderboardDialog::onTabChanged);

	// Create tables for each difficulty
	auto createTable = [this]() -> QPair<QTableView*, QStandardItemModel*>
	{
		auto* model = new QStandardItemModel(this);
		model->setHorizontalHeaderLabels({tr("Rank"), tr("Name"), tr("Time"), tr("Date")});

		auto* table = new QTableView(this);
		table->setModel(model);
		table->setSelectionBehavior(QAbstractItemView::SelectRows);
		table->setSelectionMode(QAbstractItemView::SingleSelection);
		table->setEditTriggers(QAbstractItemView::NoEditTriggers);
		table->horizontalHeader()->setStretchLastSection(true);
		table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		table->verticalHeader()->setVisible(false);
		table->setAlternatingRowColors(true);

		return {table, model};
	};

	auto [beginnerTable, beginnerModel] = createTable();
	m_beginnerTable						= beginnerTable;
	m_beginnerModel						= beginnerModel;
	m_tabWidget->addTab(m_beginnerTable, tr("😊 Beginner"));

	auto [intermediateTable, intermediateModel] = createTable();
	m_intermediateTable							= intermediateTable;
	m_intermediateModel							= intermediateModel;
	m_tabWidget->addTab(m_intermediateTable, tr("🤔 Intermediate"));

	auto [expertTable, expertModel] = createTable();
	m_expertTable					= expertTable;
	m_expertModel					= expertModel;
	m_tabWidget->addTab(m_expertTable, tr("😈 Expert"));

	auto [customTable, customModel] = createTable();
	m_customTable					= customTable;
	m_customModel					= customModel;
	m_tabWidget->addTab(m_customTable, tr("⚙️ Custom"));

	mainLayout->addWidget(m_tabWidget);

	// Close button
	auto* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch();
	auto* closeButton = new QPushButton(tr("Close"), this);
	connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
	buttonLayout->addWidget(closeButton);
	mainLayout->addLayout(buttonLayout);
}

//--------------------------------------------------------------------------------------------------
//  SET ACTIVE TAB
//--------------------------------------------------------------------------------------------------
void LeaderboardDialog::setActiveTab(GameEnums::Difficulty difficulty)
{
	switch (difficulty)
	{
	case GameEnums::Difficulty::Beginner:
		m_tabWidget->setCurrentIndex(0);
		break;
	case GameEnums::Difficulty::Intermediate:
		m_tabWidget->setCurrentIndex(1);
		break;
	case GameEnums::Difficulty::Expert:
		m_tabWidget->setCurrentIndex(2);
		break;
	case GameEnums::Difficulty::Custom:
		m_tabWidget->setCurrentIndex(3);
		break;
	}
}

//--------------------------------------------------------------------------------------------------
//  SET ACTIVE MODE
//--------------------------------------------------------------------------------------------------
void LeaderboardDialog::setActiveMode(GameEnums::GameMode mode)
{
	int index = m_modeComboBox->findData(static_cast<int>(mode));
	if (index >= 0)
	{
		m_modeComboBox->setCurrentIndex(index);
	}
}

//--------------------------------------------------------------------------------------------------
//  ON MODE CHANGED
//--------------------------------------------------------------------------------------------------
void LeaderboardDialog::onModeChanged(int index)
{
	Q_UNUSED(index)
	updateTable();
}

//--------------------------------------------------------------------------------------------------
//  ON TAB CHANGED
//--------------------------------------------------------------------------------------------------
void LeaderboardDialog::onTabChanged(int index)
{
	Q_UNUSED(index)
	// Table is already populated, nothing to do
}

//--------------------------------------------------------------------------------------------------
//  UPDATE TABLE
//--------------------------------------------------------------------------------------------------
void LeaderboardDialog::updateTable()
{
	auto mode = static_cast<GameEnums::GameMode>(m_modeComboBox->currentData().toInt());

	populateTable(GameEnums::Difficulty::Beginner, mode);
	populateTable(GameEnums::Difficulty::Intermediate, mode);
	populateTable(GameEnums::Difficulty::Expert, mode);
	populateTable(GameEnums::Difficulty::Custom, mode);
}

//--------------------------------------------------------------------------------------------------
//  POPULATE TABLE
//--------------------------------------------------------------------------------------------------
void LeaderboardDialog::populateTable(GameEnums::Difficulty difficulty, GameEnums::GameMode mode)
{
	QStandardItemModel* model = nullptr;

	switch (difficulty)
	{
	case GameEnums::Difficulty::Beginner:
		model = m_beginnerModel;
		break;
	case GameEnums::Difficulty::Intermediate:
		model = m_intermediateModel;
		break;
	case GameEnums::Difficulty::Expert:
		model = m_expertModel;
		break;
	case GameEnums::Difficulty::Custom:
		model = m_customModel;
		break;
	}

	if (!model)
		return;

	model->removeRows(0, model->rowCount());

	auto entries = m_leaderboard->getEntries(difficulty, mode);

	for (int i = 0; i < entries.size(); ++i)
	{
		const auto& entry = entries[i];

		QList<QStandardItem*> row;

		// Rank
		auto* rankItem = new QStandardItem(QString::number(i + 1));
		rankItem->setTextAlignment(Qt::AlignCenter);
		if (i == 0)
			rankItem->setIcon(QIcon("🥇"));
		else if (i == 1)
			rankItem->setIcon(QIcon("🥈"));
		else if (i == 2)
			rankItem->setIcon(QIcon("🥉"));
		row.append(rankItem);

		// Name
		row.append(new QStandardItem(entry.name));

		// Time (format as MM:SS)
		int	  minutes  = entry.score / 60;
		int	  seconds  = entry.score % 60;
		auto* timeItem = new QStandardItem(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
		timeItem->setTextAlignment(Qt::AlignCenter);
		row.append(timeItem);

		// Date
		auto* dateItem = new QStandardItem(entry.date.toString("yyyy-MM-dd hh:mm"));
		row.append(dateItem);

		model->appendRow(row);
	}

	if (entries.isEmpty())
	{
		model->appendRow({new QStandardItem(tr("No scores yet")), new QStandardItem(""), new QStandardItem(""), new QStandardItem("")});
	}
}
