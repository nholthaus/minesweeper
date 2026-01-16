//--------------------------------------------------------------------------------------------------
//
/// @file   themeManager.h
/// @brief  Theme manager for Dark/Light mode support
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include "gameEnums.h"

#include <QColor>
#include <QObject>
#include <QPalette>
#include <QString>

//--------------------------------------------------------------------------------------------------
//  STRUCT: ThemeColors
//--------------------------------------------------------------------------------------------------
struct ThemeColors
{
	// Main colors
	QColor background;
	QColor foreground;
	QColor primary;
	QColor secondary;
	QColor accent;

	// Tile colors
	QColor tileUnrevealed;
	QColor tileUnrevealedHover;
	QColor tileRevealed;
	QColor tileMine;
	QColor tileFlag;

	// Number colors
	QColor number1;
	QColor number2;
	QColor number3;
	QColor number4;
	QColor number5;
	QColor number6;
	QColor number7;
	QColor number8;

	// UI colors
	QColor menuBackground;
	QColor menuForeground;
	QColor buttonBackground;
	QColor buttonForeground;
	QColor lcdBackground;
	QColor lcdForeground;
	QColor border;
};

//--------------------------------------------------------------------------------------------------
//  CLASS: ThemeManager
//--------------------------------------------------------------------------------------------------
class ThemeManager : public QObject
{
	Q_OBJECT

public:

	static ThemeManager& instance();

	void						   setTheme(GameEnums::Theme theme);
	[[nodiscard]] GameEnums::Theme currentTheme() const { return m_currentTheme; }
	[[nodiscard]] bool			   isDarkMode() const;

	[[nodiscard]] const ThemeColors& colors() const { return m_colors; }
	[[nodiscard]] QString			 styleSheet() const;
	[[nodiscard]] QPalette			 palette() const;

	// Get color scheme for Qt components
	[[nodiscard]] Qt::ColorScheme colorScheme() const;

signals:
	void themeChanged(GameEnums::Theme theme);
	void colorsChanged();

private:

	ThemeManager();
	~ThemeManager() override = default;

	void		updateColors();
	ThemeColors lightThemeColors() const;
	ThemeColors darkThemeColors() const;

private:

	GameEnums::Theme m_currentTheme = GameEnums::Theme::System;
	ThemeColors		 m_colors;
};
