#include "mineCounter.h"

MineCounter::MineCounter(QWidget* parent)
	: QLCDNumber(parent)
{
	this->setDigitCount(3);
	this->display(0);
	this->setStyleSheet(".QLCDNumber { border: 2px inset gray; background-color: black; color: red; }");
	this->setSegmentStyle(QLCDNumber::Flat);
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

QSize MineCounter::sizeHint() const
{
	return QSize(75, 35);
}

