#include "mineCounter.h"
MineCounter::MineCounter(QWidget* parent)
	: QLCDNumber(parent)
{
	this->setDigitCount(3);
	this->display(0);
	this->setSegmentStyle(QLCDNumber::Flat);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setTheme(Theme::currentColorScheme());
	this->setToolTip("Mines Remaining");
}

void MineCounter::setNumMines(int numMines)
{
	m_totalMines = numMines;
	display(m_totalMines);
}

void MineCounter::setFlagCount(unsigned int flagCount)
{
	m_flagCount = flagCount;
	display((int)m_totalMines - (int)flagCount);
}

void MineCounter::setTheme(Theme::ColorScheme colorScheme)
{
	if (Theme::isDark(colorScheme))
		this->setStyleSheet(".QLCDNumber { border: 2px inset #303030; background-color: black; color: red; }");
	else
		this->setStyleSheet(".QLCDNumber { border: 2px inset gray; background-color: black; color: red; }");
}

QSize MineCounter::sizeHint() const
{
	return QSize(65, 35);
}
