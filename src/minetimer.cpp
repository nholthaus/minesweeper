#include "minetimer.h"

MineTimer::MineTimer(QWidget* parent /*= nullptr*/)
	: QLCDNumber(parent)
	, m_seconds(0)
{
	this->setDigitCount(4);
	this->display(0);
	this->setStyleSheet(".QLCDNumber { border: 2px inset gray; background-color: black; color: red; }");
	this->setSegmentStyle(QLCDNumber::Flat);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void MineTimer::incrementTime()
{
	display(++m_seconds);
}

QSize MineTimer::sizeHint() const
{
	return QSize(65, 35);
}

