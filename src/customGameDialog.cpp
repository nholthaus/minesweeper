//--------------------------------------------------------------------------------------------------
//
/// @file   customGameDialog.cpp
/// @brief  Implementation of CustomGameDialog
//
//--------------------------------------------------------------------------------------------------

#include "customGameDialog.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
CustomGameDialog::CustomGameDialog(const CustomGameSettings& currentSettings, QWidget* parent)
	: QDialog(parent)
	, m_settings(currentSettings)
{
	setWindowTitle(tr("Custom Game"));
	setModal(true);
	setupUI();
}

//--------------------------------------------------------------------------------------------------
//  SETUP UI
//--------------------------------------------------------------------------------------------------
void CustomGameDialog::setupUI()
{
	auto* mainLayout = new QVBoxLayout(this);

	// Settings group
	auto* settingsGroup = new QGroupBox(tr("Board Settings"), this);
	auto* formLayout	= new QFormLayout(settingsGroup);

	// Rows
	m_rowsSpinBox = new QSpinBox(this);
	m_rowsSpinBox->setRange(5, 50);
	m_rowsSpinBox->setValue(static_cast<int>(m_settings.rows));
	m_rowsSpinBox->setSuffix(tr(" rows"));
	formLayout->addRow(tr("Height:"), m_rowsSpinBox);

	// Columns
	m_colsSpinBox = new QSpinBox(this);
	m_colsSpinBox->setRange(5, 50);
	m_colsSpinBox->setValue(static_cast<int>(m_settings.cols));
	m_colsSpinBox->setSuffix(tr(" columns"));
	formLayout->addRow(tr("Width:"), m_colsSpinBox);

	// Mines
	m_minesSpinBox = new QSpinBox(this);
	m_minesSpinBox->setRange(1, 999);
	m_minesSpinBox->setValue(static_cast<int>(m_settings.mines));
	m_minesSpinBox->setSuffix(tr(" mines"));
	formLayout->addRow(tr("Mines:"), m_minesSpinBox);

	settingsGroup->setLayout(formLayout);
	mainLayout->addWidget(settingsGroup);

	// Info label
	m_infoLabel = new QLabel(this);
	m_infoLabel->setWordWrap(true);
	m_infoLabel->setStyleSheet("QLabel { color: gray; font-style: italic; }");
	mainLayout->addWidget(m_infoLabel);

	// Buttons
	auto* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch();

	m_okButton = new QPushButton(tr("OK"), this);
	m_okButton->setDefault(true);
	connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);

	m_cancelButton = new QPushButton(tr("Cancel"), this);
	connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);

	buttonLayout->addWidget(m_okButton);
	buttonLayout->addWidget(m_cancelButton);
	mainLayout->addLayout(buttonLayout);

	// Connect signals for validation
	connect(m_rowsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CustomGameDialog::validateSettings);
	connect(m_colsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CustomGameDialog::validateSettings);
	connect(m_minesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CustomGameDialog::validateSettings);

	// Initial validation
	validateSettings();

	setMinimumWidth(300);
}

//--------------------------------------------------------------------------------------------------
//  UPDATE MINE RANGE
//--------------------------------------------------------------------------------------------------
void CustomGameDialog::updateMineRange()
{
	int rows	 = m_rowsSpinBox->value();
	int cols	 = m_colsSpinBox->value();
	int maxMines = rows * cols - 9; // Leave space for first click area

	m_minesSpinBox->setMaximum(maxMines > 0 ? maxMines : 1);

	QString info = tr("Board size: %1 x %2 = %3 cells\nMaximum mines: %4").arg(rows).arg(cols).arg(rows * cols).arg(maxMines > 0 ? maxMines : 1);
	m_infoLabel->setText(info);
}

//--------------------------------------------------------------------------------------------------
//  VALIDATE SETTINGS
//--------------------------------------------------------------------------------------------------
void CustomGameDialog::validateSettings()
{
	updateMineRange();

	m_settings.rows	 = static_cast<quint32>(m_rowsSpinBox->value());
	m_settings.cols	 = static_cast<quint32>(m_colsSpinBox->value());
	m_settings.mines = static_cast<quint32>(m_minesSpinBox->value());

	m_okButton->setEnabled(m_settings.isValid());
}

//--------------------------------------------------------------------------------------------------
//  GET SETTINGS
//--------------------------------------------------------------------------------------------------
CustomGameSettings CustomGameDialog::settings() const { return m_settings; }
