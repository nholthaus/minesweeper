#pragma once
#include <QLCDNumber>

#include "theme.h"

class MineTimer : public QLCDNumber
{
public:
	MineTimer(QWidget* parent = nullptr);

	void incrementTime();
	void reset();
	int time() const;
	void setTheme(Theme::ColorScheme colorScheme);
	virtual QSize sizeHint() const override;

private:

	int m_seconds;
};
