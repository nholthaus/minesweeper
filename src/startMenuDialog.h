// //--------------------------------------------------------------------------------------------------
// //
// /// @file   startMenuDialog.h
// /// @brief  Modern start menu for game mode and difficulty selection
// //
// //--------------------------------------------------------------------------------------------------

// #pragma once

// #include "gameController.h"
// #include "gameEnums.h"

// #include <QButtonGroup>
// #include <QComboBox>
// #include <QDialog>
// #include <QHBoxLayout>
// #include <QLabel>
// #include <QPushButton>
// #include <QStackedWidget>
// #include <QVBoxLayout>

// //--------------------------------------------------------------------------------------------------
// //  CLASS: StartMenuDialog
// //--------------------------------------------------------------------------------------------------
// class StartMenuDialog : public QDialog
// {
// 	Q_OBJECT

// public:

// 	explicit StartMenuDialog(GameController* controller, QWidget* parent = nullptr);

// 	[[nodiscard]] GameEnums::GameMode	selectedMode() const { return m_selectedMode; }
// 	[[nodiscard]] GameEnums::Difficulty selectedDifficulty() const { return m_selectedDifficulty; }
// 	[[nodiscard]] CustomGameSettings	customSettings() const { return m_customSettings; }

// signals:
// 	void newGameRequested(GameEnums::GameMode mode, GameEnums::Difficulty difficulty);
// 	void restartRequested();
// 	void themeChangeRequested(GameEnums::Theme theme);

// private slots:
// 	void onModeSelected(GameEnums::GameMode mode);
// 	void onDifficultySelected(GameEnums::Difficulty difficulty);
// 	void onCustomClicked();
// 	void onStartClicked();
// 	void onRestartClicked();
// 	void onThemeChanged(int index);

// private:

// 	void		 setupUI();
// 	void		 setupMainPage();
// 	void		 setupModeSelectionPage();
// 	void		 setupDifficultyPage();
// 	void		 updateStartButton();
// 	QPushButton* createStyledButton(const QString& text, const QString& iconPath = QString());

// private:

// 	GameController* m_controller;

// 	QStackedWidget* m_stackedWidget;
// 	QWidget*		m_mainPage;
// 	QWidget*		m_modeSelectionPage;
// 	QWidget*		m_difficultyPage;

// 	// Main page buttons
// 	QPushButton* m_restartButton;
// 	QPushButton* m_newGameButton;

// 	// Mode buttons
// 	QPushButton* m_normalModeButton;
// 	QPushButton* m_timeAttackButton;

// 	// Difficulty buttons
// 	QPushButton* m_beginnerButton;
// 	QPushButton* m_intermediateButton;
// 	QPushButton* m_expertButton;
// 	QPushButton* m_customButton;

// 	// Theme selector
// 	QComboBox* m_themeComboBox;

// 	// Navigation
// 	QPushButton* m_backButton;
// 	QPushButton* m_startButton;

// 	// State
// 	GameEnums::GameMode	  m_selectedMode	   = GameEnums::GameMode::Normal;
// 	GameEnums::Difficulty m_selectedDifficulty = GameEnums::Difficulty::Beginner;
// 	CustomGameSettings	  m_customSettings;
// 	bool				  m_gameInProgress = false;
// };
#ifndef STARTMENUDIALOG_H
#define STARTMENUDIALOG_H

#include "gameController.h"
#include "gameEnums.h"

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

// Context để quyết định trang khởi động
enum class StartMenuContext
{
	MainMenu, // App mới mở → hiện Main Page
	NewGame	  // Bấm New Game → hiện Mode Selection
};

// CustomGameSettings đã định nghĩa trong gameController.h

class StartMenuDialog : public QDialog
{
	Q_OBJECT

public:

	explicit StartMenuDialog(GameController* controller, StartMenuContext context = StartMenuContext::MainMenu, QWidget* parent = nullptr);

	GameEnums::GameMode	  selectedMode() const { return m_selectedMode; }
	GameEnums::Difficulty selectedDifficulty() const { return m_selectedDifficulty; }
	CustomGameSettings	  customSettings() const { return m_customSettings; }

signals:
	void newGameRequested(GameEnums::GameMode mode, GameEnums::Difficulty difficulty);
	void restartRequested();
	void themeChangeRequested(GameEnums::Theme theme);
	void leaderboardRequested();
	void statisticsRequested();

private slots:
	void onModeSelected(GameEnums::GameMode mode);
	void onDifficultySelected(GameEnums::Difficulty difficulty);
	void onCustomClicked();
	void onStartClicked();
	void onRestartClicked();
	void onThemeChanged(int index);

private:

	void		 setupUI();
	void		 setupMainPage();
	void		 setupModeSelectionPage();
	void		 setupDifficultyPage();
	void		 updateStartButton();
	QPushButton* createStyledButton(const QString& text, const QString& iconPath = QString());

	GameController*		  m_controller;
	StartMenuContext	  m_context;
	bool				  m_gameInProgress	   = false;
	GameEnums::GameMode	  m_selectedMode	   = GameEnums::GameMode::Normal;
	GameEnums::Difficulty m_selectedDifficulty = GameEnums::Difficulty::Beginner;
	CustomGameSettings	  m_customSettings;

	// UI elements
	QStackedWidget* m_stackedWidget		= nullptr;
	QWidget*		m_mainPage			= nullptr;
	QWidget*		m_modeSelectionPage = nullptr;
	QWidget*		m_difficultyPage	= nullptr;

	QPushButton* m_newGameButton	  = nullptr;
	QPushButton* m_leaderboardButton  = nullptr;
	QPushButton* m_statisticsButton	  = nullptr;
	QPushButton* m_normalModeButton	  = nullptr;
	QPushButton* m_timeAttackButton	  = nullptr;
	QPushButton* m_beginnerButton	  = nullptr;
	QPushButton* m_intermediateButton = nullptr;
	QPushButton* m_expertButton		  = nullptr;
	QPushButton* m_customButton		  = nullptr;
	QPushButton* m_startButton		  = nullptr;
	QPushButton* m_backButton		  = nullptr;
	QComboBox*	 m_themeComboBox	  = nullptr;
};

#endif // STARTMENUDIALOG_H
