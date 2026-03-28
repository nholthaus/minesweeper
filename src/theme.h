#pragma once

#include <QGuiApplication>
#include <QPalette>
#include <QStyleHints>
#include <QtGlobal>

namespace Theme
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
using ColorScheme = Qt::ColorScheme;

inline ColorScheme currentColorScheme()
{
	return QGuiApplication::styleHints()->colorScheme();
}

inline bool isDark(ColorScheme colorScheme)
{
	return colorScheme == Qt::ColorScheme::Dark;
}
#else
enum class ColorScheme
{
	Light,
	Dark
};

inline ColorScheme currentColorScheme()
{
	return QGuiApplication::palette().color(QPalette::Window).lightness() < 128 ? ColorScheme::Dark : ColorScheme::Light;
}

inline bool isDark(ColorScheme colorScheme)
{
	return colorScheme == ColorScheme::Dark;
}
#endif
}
