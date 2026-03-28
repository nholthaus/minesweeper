#pragma once
#include <QLCDNumber>

#include "theme.h"

class MineCounter : public QLCDNumber
{
public:
	MineCounter(QWidget* parent = nullptr);

	void setNumMines(int numMines);
	void setFlagCount(unsigned int flagCount);
	void setTheme(Theme::ColorScheme colorScheme);
	virtual QSize sizeHint() const override;

private:

	int m_totalMines;
	unsigned int m_flagCount;
};
