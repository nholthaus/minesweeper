//--------------------------------------------------------------------------------------------------
//
/// @file   customGameDialog.h
/// @brief  Dialog for custom game settings
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include "gameController.h"

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

//--------------------------------------------------------------------------------------------------
//  CLASS: CustomGameDialog
//--------------------------------------------------------------------------------------------------
class CustomGameDialog : public QDialog
{
	Q_OBJECT

public:

	explicit CustomGameDialog(const CustomGameSettings& currentSettings, QWidget* parent = nullptr);

	[[nodiscard]] CustomGameSettings settings() const;

private slots:
	void validateSettings();

private:

	void setupUI();
	void updateMineRange();

private:

	QSpinBox*	 m_rowsSpinBox;
	QSpinBox*	 m_colsSpinBox;
	QSpinBox*	 m_minesSpinBox;
	QLabel*		 m_infoLabel;
	QPushButton* m_okButton;
	QPushButton* m_cancelButton;

	CustomGameSettings m_settings;
};
