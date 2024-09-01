#include "mineCounter.h"
#include <QGuiApplication>
#include <QStyleHints>

MineCounter::MineCounter(QWidget* parent)
	: QLCDNumber(parent)
{
	this->setDigitCount(3);
	this->display(0);
	this->setSegmentStyle(QLCDNumber::Flat);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark)
		this->setStyleSheet(".QLCDNumber { border: 2px inset #303030; background-color: black; color: red; }");
	else
		this->setStyleSheet(".QLCDNumber { border: 2px inset gray; background-color: black; color: red; }");
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
	return QSize(65, 35);
}