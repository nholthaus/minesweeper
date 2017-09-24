#pragma once
#include <QLCDNumber>

class MineTimer : public QLCDNumber
{
public:
	MineTimer(QWidget* parent = nullptr);

	void incrementTime();
	virtual QSize sizeHint() const override;

private:

	int m_seconds;
};
