#pragma once
#include <QLCDNumber>

class MineCounter : public QLCDNumber
{
public:
	MineCounter(QWidget* parent = nullptr);

	void setNumMines(int numMines);
	void setFlagCount(unsigned int flagCount);
	void setTheme(Qt::ColorScheme colorScheme);
	virtual QSize sizeHint() const override;

private:

	int m_totalMines;
	unsigned int m_flagCount;
};