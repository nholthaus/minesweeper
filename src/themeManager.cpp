//--------------------------------------------------------------------------------------------------
//
/// @file   themeManager.cpp
/// @brief  Implementation of ThemeManager
//
//--------------------------------------------------------------------------------------------------

#include "themeManager.h"

#include <QApplication>
#include <QGuiApplication>
#include <QStyleHints>

//--------------------------------------------------------------------------------------------------
//  SINGLETON INSTANCE
//--------------------------------------------------------------------------------------------------
ThemeManager& ThemeManager::instance()
{
	static ThemeManager instance;
	return instance;
}

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
ThemeManager::ThemeManager() { updateColors(); }

//--------------------------------------------------------------------------------------------------
//  SET THEME
//--------------------------------------------------------------------------------------------------
void ThemeManager::setTheme(GameEnums::Theme theme)
{
	if (m_currentTheme != theme)
	{
		m_currentTheme = theme;
		updateColors();
		emit themeChanged(theme);
		emit colorsChanged();
	}
}

//--------------------------------------------------------------------------------------------------
//  IS DARK MODE
//--------------------------------------------------------------------------------------------------
bool ThemeManager::isDarkMode() const
{
	switch (m_currentTheme)
	{
	case GameEnums::Theme::Light:
		return false;
	case GameEnums::Theme::Dark:
		return true;
	case GameEnums::Theme::System:
	default:
		// Safely check for system dark mode with null guard
		if (auto* styleHints = QGuiApplication::styleHints())
		{
			return styleHints->colorScheme() == Qt::ColorScheme::Dark;
		}
		return false; // Default to light if styleHints is not available
	}
}

//--------------------------------------------------------------------------------------------------
//  COLOR SCHEME
//--------------------------------------------------------------------------------------------------
Qt::ColorScheme ThemeManager::colorScheme() const { return isDarkMode() ? Qt::ColorScheme::Dark : Qt::ColorScheme::Light; }

//--------------------------------------------------------------------------------------------------
//  UPDATE COLORS
//--------------------------------------------------------------------------------------------------
void ThemeManager::updateColors() { m_colors = isDarkMode() ? darkThemeColors() : lightThemeColors(); }

//--------------------------------------------------------------------------------------------------
//  LIGHT THEME COLORS
//--------------------------------------------------------------------------------------------------
ThemeColors ThemeManager::lightThemeColors() const
{
	ThemeColors colors;

	// Main colors
	colors.background = QColor("#F5F5F5");
	colors.foreground = QColor("#212121");
	colors.primary	  = QColor("#1976D2");
	colors.secondary  = QColor("#757575");
	colors.accent	  = QColor("#FF5722");

	// Tile colors
	colors.tileUnrevealed	   = QColor("#BDBDBD");
	colors.tileUnrevealedHover = QColor("#9E9E9E");
	colors.tileRevealed		   = QColor("#E0E0E0");
	colors.tileMine			   = QColor("#D32F2F");
	colors.tileFlag			   = QColor("#FF5722");

	// Classic minesweeper number colors
	colors.number1 = QColor("#1976D2"); // Blue
	colors.number2 = QColor("#388E3C"); // Green
	colors.number3 = QColor("#D32F2F"); // Red
	colors.number4 = QColor("#7B1FA2"); // Purple
	colors.number5 = QColor("#800000"); // Maroon
	colors.number6 = QColor("#00838F"); // Teal
	colors.number7 = QColor("#212121"); // Black
	colors.number8 = QColor("#757575"); // Gray

	// UI colors
	colors.menuBackground	= QColor("#FFFFFF");
	colors.menuForeground	= QColor("#212121");
	colors.buttonBackground = QColor("#E0E0E0");
	colors.buttonForeground = QColor("#212121");
	colors.lcdBackground	= QColor("#1B1B1B");
	colors.lcdForeground	= QColor("#FF0000");
	colors.border			= QColor("#BDBDBD");

	return colors;
}

//--------------------------------------------------------------------------------------------------
//  DARK THEME COLORS
//--------------------------------------------------------------------------------------------------
ThemeColors ThemeManager::darkThemeColors() const
{
	ThemeColors colors;

	// Main colors
	colors.background = QColor("#1E1E1E");
	colors.foreground = QColor("#E0E0E0");
	colors.primary	  = QColor("#64B5F6");
	colors.secondary  = QColor("#9E9E9E");
	colors.accent	  = QColor("#FF7043");

	// Tile colors
	colors.tileUnrevealed	   = QColor("#424242");
	colors.tileUnrevealedHover = QColor("#616161");
	colors.tileRevealed		   = QColor("#2C2C2C");
	colors.tileMine			   = QColor("#EF5350");
	colors.tileFlag			   = QColor("#FF7043");

	// Brighter number colors for dark mode
	colors.number1 = QColor("#64B5F6"); // Light Blue
	colors.number2 = QColor("#81C784"); // Light Green
	colors.number3 = QColor("#EF5350"); // Light Red
	colors.number4 = QColor("#BA68C8"); // Light Purple
	colors.number5 = QColor("#E57373"); // Light Maroon
	colors.number6 = QColor("#4DD0E1"); // Light Teal
	colors.number7 = QColor("#E0E0E0"); // Light Gray
	colors.number8 = QColor("#BDBDBD"); // Gray

	// UI colors
	colors.menuBackground	= QColor("#2C2C2C");
	colors.menuForeground	= QColor("#E0E0E0");
	colors.buttonBackground = QColor("#424242");
	colors.buttonForeground = QColor("#E0E0E0");
	colors.lcdBackground	= QColor("#0D0D0D");
	colors.lcdForeground	= QColor("#FF4444");
	colors.border			= QColor("#424242");

	return colors;
}

//--------------------------------------------------------------------------------------------------
//  STYLE SHEET
//--------------------------------------------------------------------------------------------------
QString ThemeManager::styleSheet() const
{
	const auto& c = m_colors;

	return QString(R"(
        QMainWindow {
            background-color: %1;
        }
        QMenuBar {
            background-color: %2;
            color: %3;
            border-bottom: 1px solid %4;
        }
        QMenuBar::item:selected {
            background-color: %5;
        }
        QMenu {
            background-color: %2;
            color: %3;
            border: 1px solid %4;
        }
        QMenu::item:selected {
            background-color: %5;
        }
        QFrame {
            background-color: %1;
        }
        QPushButton {
            background-color: %6;
            color: %7;
            border: 1px solid %4;
            border-radius: 3px;
            padding: 5px 10px;
        }
        QPushButton:hover {
            background-color: %5;
        }
        QPushButton:pressed {
            background-color: %4;
        }
        QDialog {
            background-color: %1;
            color: %8;
        }
        QLabel {
            color: %8;
        }
        QGroupBox {
            color: %8;
            border: 1px solid %4;
            border-radius: 5px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QSpinBox {
            background-color: %6;
            color: %7;
            border: 1px solid %4;
            border-radius: 3px;
            padding: 3px;
        }
        QComboBox {
            background-color: %6;
            color: %7;
            border: 1px solid %4;
            border-radius: 3px;
            padding: 3px 10px;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox QAbstractItemView {
            background-color: %2;
            color: %3;
            selection-background-color: %5;
        }
    )")
		.arg(c.background.name())		// %1
		.arg(c.menuBackground.name())	// %2
		.arg(c.menuForeground.name())	// %3
		.arg(c.border.name())			// %4
		.arg(c.primary.name())			// %5
		.arg(c.buttonBackground.name()) // %6
		.arg(c.buttonForeground.name()) // %7
		.arg(c.foreground.name());		// %8
}

//--------------------------------------------------------------------------------------------------
//  PALETTE
//--------------------------------------------------------------------------------------------------
QPalette ThemeManager::palette() const
{
	QPalette	palette;
	const auto& c = m_colors;

	palette.setColor(QPalette::Window, c.background);
	palette.setColor(QPalette::WindowText, c.foreground);
	palette.setColor(QPalette::Base, c.menuBackground);
	palette.setColor(QPalette::AlternateBase, c.background);
	palette.setColor(QPalette::Text, c.foreground);
	palette.setColor(QPalette::Button, c.buttonBackground);
	palette.setColor(QPalette::ButtonText, c.buttonForeground);
	palette.setColor(QPalette::Highlight, c.primary);
	palette.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
	palette.setColor(QPalette::Link, c.primary);
	palette.setColor(QPalette::LinkVisited, c.secondary);

	return palette;
}
