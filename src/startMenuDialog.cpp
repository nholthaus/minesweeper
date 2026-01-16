//--------------------------------------------------------------------------------------------------
//
/// @file   startMenuDialog.cpp
/// @brief  Implementation of StartMenuDialog
//
//--------------------------------------------------------------------------------------------------

#include "startMenuDialog.h"
#include "customGameDialog.h"
#include "themeManager.h"

#include <QComboBox>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
StartMenuDialog::StartMenuDialog(GameController* controller, StartMenuContext context, QWidget* parent)
	: QDialog(parent)
	, m_controller(controller)
	, m_context(context)
	, m_gameInProgress(controller->currentState() == GameEnums::GameState::Playing || controller->currentState() == GameEnums::GameState::Paused)
{
	setWindowTitle(tr("Minesweeper"));
	setModal(true);
	setMinimumSize(350, 400);

	setupUI();

	// 🔥 QUYẾT ĐỊNH TRANG KHỞI ĐỘNG DỰA TRÊN CONTEXT
	if (m_context == StartMenuContext::NewGame)
		m_stackedWidget->setCurrentWidget(m_modeSelectionPage);
	else
		m_stackedWidget->setCurrentWidget(m_mainPage);

	// Apply theme
	setStyleSheet(ThemeManager::instance().styleSheet());
}

//--------------------------------------------------------------------------------------------------
//  SETUP UI
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::setupUI()
{
	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(15);
	mainLayout->setContentsMargins(30, 30, 30, 30);

	// Title
	auto* titleLabel = new QLabel("💣 Minesweeper", this);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 28px;
            font-weight: bold;
            margin-bottom: 20px;
        }
    )");
	mainLayout->addWidget(titleLabel);

	// Stacked widget for different pages
	m_stackedWidget = new QStackedWidget(this);

	setupMainPage();
	setupModeSelectionPage();
	setupDifficultyPage();

	m_stackedWidget->addWidget(m_mainPage);
	m_stackedWidget->addWidget(m_modeSelectionPage);
	m_stackedWidget->addWidget(m_difficultyPage);

	mainLayout->addWidget(m_stackedWidget);

	// Theme selector at the bottom
	auto* themeLayout = new QHBoxLayout;
	auto* themeLabel  = new QLabel(tr("Theme:"), this);
	m_themeComboBox	  = new QComboBox(this);
	m_themeComboBox->addItem(tr("Light"), static_cast<int>(GameEnums::Theme::Light));
	m_themeComboBox->addItem(tr("Dark"), static_cast<int>(GameEnums::Theme::Dark));

	// Set current theme (adjust index since System was removed)
	auto currentTheme = m_controller->currentTheme();
	if (currentTheme == GameEnums::Theme::Light)
		m_themeComboBox->setCurrentIndex(0);
	else
		m_themeComboBox->setCurrentIndex(1);

	connect(m_themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StartMenuDialog::onThemeChanged);

	themeLayout->addStretch();
	themeLayout->addWidget(themeLabel);
	themeLayout->addWidget(m_themeComboBox);
	themeLayout->addStretch();
	mainLayout->addLayout(themeLayout);
}

//--------------------------------------------------------------------------------------------------
//  SETUP MAIN PAGE
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::setupMainPage()
{
	m_mainPage	 = new QWidget(this);
	auto* layout = new QVBoxLayout(m_mainPage);
	layout->setSpacing(15);

	// New Game button
	m_newGameButton = createStyledButton(tr("🎮 New Game"));
	connect(m_newGameButton, &QPushButton::clicked, [this]() { m_stackedWidget->setCurrentWidget(m_modeSelectionPage); });
	layout->addWidget(m_newGameButton);

	// Leaderboard button
	m_leaderboardButton = createStyledButton(tr("🏆 Leaderboard"));
	connect(m_leaderboardButton, &QPushButton::clicked,
			[this]()
			{
				emit leaderboardRequested();
				accept();
			});
	layout->addWidget(m_leaderboardButton);

	// Statistics button
	m_statisticsButton = createStyledButton(tr("📊 Statistics"));
	connect(m_statisticsButton, &QPushButton::clicked,
			[this]()
			{
				emit statisticsRequested();
				accept();
			});
	layout->addWidget(m_statisticsButton);

	layout->addStretch();
}

//--------------------------------------------------------------------------------------------------
//  SETUP MODE SELECTION PAGE
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::setupModeSelectionPage()
{
	m_modeSelectionPage = new QWidget(this);
	auto* layout		= new QVBoxLayout(m_modeSelectionPage);
	layout->setSpacing(15);

	auto* modeLabel = new QLabel(tr("Select Mode"), this);
	modeLabel->setAlignment(Qt::AlignCenter);
	modeLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
	layout->addWidget(modeLabel);

	// Normal mode
	m_normalModeButton = createStyledButton(tr("⏱️ Normal\nClassic Minesweeper - No time limit"));
	m_normalModeButton->setCheckable(true);
	m_normalModeButton->setChecked(true);
	connect(m_normalModeButton, &QPushButton::clicked, [this]() { onModeSelected(GameEnums::GameMode::Normal); });
	layout->addWidget(m_normalModeButton);

	// Time Attack mode
	m_timeAttackButton = createStyledButton(tr("⚡ Time Attack\nComplete before time runs out!"));
	m_timeAttackButton->setCheckable(true);
	connect(m_timeAttackButton, &QPushButton::clicked, [this]() { onModeSelected(GameEnums::GameMode::TimeAttack); });
	layout->addWidget(m_timeAttackButton);

	layout->addStretch();

	// Back button
	auto* navLayout = new QHBoxLayout;
	m_backButton	= new QPushButton(tr("← Back"), this);
	connect(m_backButton, &QPushButton::clicked, [this]() { m_stackedWidget->setCurrentWidget(m_mainPage); });
	navLayout->addWidget(m_backButton);
	navLayout->addStretch();
	layout->addLayout(navLayout);
}

//--------------------------------------------------------------------------------------------------
//  SETUP DIFFICULTY PAGE
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::setupDifficultyPage()
{
	m_difficultyPage = new QWidget(this);
	auto* layout	 = new QVBoxLayout(m_difficultyPage);
	layout->setSpacing(10);

	auto* diffLabel = new QLabel(tr("Select Difficulty"), this);
	diffLabel->setAlignment(Qt::AlignCenter);
	diffLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
	layout->addWidget(diffLabel);

	// Beginner
	m_beginnerButton = createStyledButton(tr("😊 Beginner\n9 × 9 • 10 mines"));
	m_beginnerButton->setCheckable(true);
	m_beginnerButton->setChecked(true);
	connect(m_beginnerButton, &QPushButton::clicked, [this]() { onDifficultySelected(GameEnums::Difficulty::Beginner); });
	layout->addWidget(m_beginnerButton);

	// Intermediate
	m_intermediateButton = createStyledButton(tr("🤔 Intermediate\n16 × 16 • 40 mines"));
	m_intermediateButton->setCheckable(true);
	connect(m_intermediateButton, &QPushButton::clicked, [this]() { onDifficultySelected(GameEnums::Difficulty::Intermediate); });
	layout->addWidget(m_intermediateButton);

	// Expert
	m_expertButton = createStyledButton(tr("😈 Expert\n16 × 30 • 99 mines"));
	m_expertButton->setCheckable(true);
	connect(m_expertButton, &QPushButton::clicked, [this]() { onDifficultySelected(GameEnums::Difficulty::Expert); });
	layout->addWidget(m_expertButton);

	// Custom
	m_customButton = createStyledButton(tr("⚙️ Custom\nChoose your own settings"));
	m_customButton->setCheckable(true);
	connect(m_customButton, &QPushButton::clicked, this, &StartMenuDialog::onCustomClicked);
	layout->addWidget(m_customButton);

	layout->addStretch();

	// Navigation
	auto* navLayout	 = new QHBoxLayout;
	auto* backButton = new QPushButton(tr("← Back"), this);
	connect(backButton, &QPushButton::clicked, [this]() { m_stackedWidget->setCurrentWidget(m_modeSelectionPage); });
	navLayout->addWidget(backButton);

	navLayout->addStretch();

	m_startButton = new QPushButton(tr("Start Game →"), this);
	m_startButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4CAF50;
            color: white;
            font-weight: bold;
            padding: 10px 20px;
            border-radius: 5px;
        }
        QPushButton:hover {
            background-color: #45A049;
        }
    )");
	connect(m_startButton, &QPushButton::clicked, this, &StartMenuDialog::onStartClicked);
	navLayout->addWidget(m_startButton);

	layout->addLayout(navLayout);
}

//--------------------------------------------------------------------------------------------------
//  CREATE STYLED BUTTON
//--------------------------------------------------------------------------------------------------
QPushButton* StartMenuDialog::createStyledButton(const QString& text, const QString& iconPath)
{
	auto* button = new QPushButton(text, this);
	button->setMinimumHeight(60);
	button->setStyleSheet(R"(
        QPushButton {
            text-align: left;
            padding: 15px;
            border-radius: 8px;
            font-size: 14px;
        }
        QPushButton:checked {
            border: 2px solid #4CAF50;
        }
    )");

	if (!iconPath.isEmpty())
	{
		button->setIcon(QIcon(iconPath));
		button->setIconSize(QSize(32, 32));
	}

	return button;
}

//--------------------------------------------------------------------------------------------------
//  MODE SELECTED
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::onModeSelected(GameEnums::GameMode mode)
{
	m_selectedMode = mode;

	m_normalModeButton->setChecked(mode == GameEnums::GameMode::Normal);
	m_timeAttackButton->setChecked(mode == GameEnums::GameMode::TimeAttack);

	// Go to difficulty selection
	m_stackedWidget->setCurrentWidget(m_difficultyPage);

	// Update difficulty button text for Time Attack mode
	if (mode == GameEnums::GameMode::TimeAttack)
	{
		m_beginnerButton->setText(tr("😊 Beginner\n9 × 9 • 10 mines • 2:00 limit"));
		m_intermediateButton->setText(tr("🤔 Intermediate\n16 × 16 • 40 mines • 5:00 limit"));
		m_expertButton->setText(tr("😈 Expert\n16 × 30 • 99 mines • 10:00 limit"));
		m_customButton->setText(tr("⚙️ Custom\nChoose your own settings • 10:00 limit"));
	}
	else
	{
		m_beginnerButton->setText(tr("😊 Beginner\n9 × 9 • 10 mines"));
		m_intermediateButton->setText(tr("🤔 Intermediate\n16 × 16 • 40 mines"));
		m_expertButton->setText(tr("😈 Expert\n16 × 30 • 99 mines"));
		m_customButton->setText(tr("⚙️ Custom\nChoose your own settings"));
	}
}

//--------------------------------------------------------------------------------------------------
//  DIFFICULTY SELECTED
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::onDifficultySelected(GameEnums::Difficulty difficulty)
{
	m_selectedDifficulty = difficulty;

	m_beginnerButton->setChecked(difficulty == GameEnums::Difficulty::Beginner);
	m_intermediateButton->setChecked(difficulty == GameEnums::Difficulty::Intermediate);
	m_expertButton->setChecked(difficulty == GameEnums::Difficulty::Expert);
	m_customButton->setChecked(difficulty == GameEnums::Difficulty::Custom);
}

//--------------------------------------------------------------------------------------------------
//  CUSTOM CLICKED
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::onCustomClicked()
{
	CustomGameDialog dialog(m_customSettings, this);
	if (dialog.exec() == QDialog::Accepted)
	{
		m_customSettings	 = dialog.settings();
		m_selectedDifficulty = GameEnums::Difficulty::Custom;
		onDifficultySelected(GameEnums::Difficulty::Custom);
	}
	else
	{
		// Revert to previous selection if cancelled
		m_customButton->setChecked(m_selectedDifficulty == GameEnums::Difficulty::Custom);
	}
}

//--------------------------------------------------------------------------------------------------
//  START CLICKED
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::onStartClicked()
{
	emit newGameRequested(m_selectedMode, m_selectedDifficulty);
	accept();
}

//--------------------------------------------------------------------------------------------------
//  RESTART CLICKED
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::onRestartClicked()
{
	emit restartRequested();
	accept();
}

//--------------------------------------------------------------------------------------------------
//  THEME CHANGED
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::onThemeChanged(int index)
{
	auto theme = static_cast<GameEnums::Theme>(m_themeComboBox->itemData(index).toInt());
	emit themeChangeRequested(theme);

	// Update dialog style immediately
	ThemeManager::instance().setTheme(theme);
	setStyleSheet(ThemeManager::instance().styleSheet());
}

//--------------------------------------------------------------------------------------------------
//  UPDATE START BUTTON
//--------------------------------------------------------------------------------------------------
void StartMenuDialog::updateStartButton()
{
	QString modeText = GameEnums::gameModeToString(m_selectedMode);
	QString diffText = GameEnums::difficultyToString(m_selectedDifficulty);
	m_startButton->setToolTip(tr("Start %1 game on %2").arg(modeText, diffText));
}
