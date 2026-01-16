//--------------------------------------------------------------------------------------------------
//
/// @file   pauseOverlay.h
/// @brief  Overlay widget shown when game is paused
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include <QLabel>
#include <QPushButton>
#include <QWidget>

//--------------------------------------------------------------------------------------------------
//  CLASS: PauseOverlay
//--------------------------------------------------------------------------------------------------
class PauseOverlay : public QWidget
{
	Q_OBJECT

public:

	explicit PauseOverlay(QWidget* parent = nullptr);

	void showOverlay();
	void hideOverlay();

signals:
	void resumeClicked();
	void newGameClicked();

protected:

	void paintEvent(QPaintEvent* event) override;

private:

	void setupUI();

private:

	QLabel*		 m_pauseLabel;
	QPushButton* m_resumeButton;
	QPushButton* m_newGameButton;
};
