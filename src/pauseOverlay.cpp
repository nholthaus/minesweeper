
//--------------------------------------------------------------------------------------------------
//
/// @file   pauseOverlay.cpp
/// @brief  Implementation of PauseOverlay
//
//--------------------------------------------------------------------------------------------------

#include "pauseOverlay.h"

#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include <QVBoxLayout>

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
PauseOverlay::PauseOverlay(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_TransparentForMouseEvents, false);
	setVisible(false);
	setupUI();
}

//--------------------------------------------------------------------------------------------------
//  SETUP UI
//--------------------------------------------------------------------------------------------------
// void PauseOverlay::setupUI()
// {
// 	auto* layout = new QVBoxLayout(this);
// 	layout->setAlignment(Qt::AlignCenter);

// 	// Pause text
// 	auto* pauseText = new QLabel(tr("PAUSED"), this);
// 	pauseText->setAlignment(Qt::AlignCenter);
// 	pauseText->setStyleSheet(R"(
//         QLabel {
//             font-size: 32px;
//             font-weight: bold;
//             color: white;
//             margin-bottom: 20px;
//         }
//     )");
// 	layout->addWidget(pauseText);

// 	// Resume button
// 	m_resumeButton = new QPushButton(tr("▶ Resume"), this);
// 	m_resumeButton->setMinimumSize(150, 50);
// 	m_resumeButton->setStyleSheet(R"(
//         QPushButton {
//             background-color: #4CAF50;
//             color: white;
//             font-size: 18px;
//             font-weight: bold;
//             border-radius: 10px;
//             padding: 10px 30px;
//         }
//         QPushButton:hover {
//             background-color: #45A049;
//         }
//         QPushButton:pressed {
//             background-color: #3D8B40;
//         }
//     )");
// 	connect(m_resumeButton, &QPushButton::clicked, this, &PauseOverlay::resumeClicked);
// 	layout->addWidget(m_resumeButton, 0, Qt::AlignCenter);

// 	// New Game button
// 	m_newGameButton = new QPushButton(tr("🎮 New Game"), this);
// 	m_newGameButton->setMinimumSize(150, 50);
// 	m_newGameButton->setStyleSheet(R"(
//         QPushButton {
//             background-color: #2196F3;
//             color: white;
//             font-size: 18px;
//             font-weight: bold;
//             border-radius: 10px;
//             padding: 10px 30px;
//             margin-top: 10px;
//         }
//         QPushButton:hover {
//             background-color: #1976D2;
//         }
//         QPushButton:pressed {
//             background-color: #1565C0;
//         }
//     )");
// 	connect(m_newGameButton, &QPushButton::clicked, this, &PauseOverlay::newGameClicked);
// 	layout->addWidget(m_newGameButton, 0, Qt::AlignCenter);
// }

void PauseOverlay::setupUI()
{
	auto* layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignCenter);
	layout->setSpacing(15); // Thêm khoảng cách giữa các widget

	// Pause text
	auto* pauseText = new QLabel(tr("PAUSED"), this);
	pauseText->setAlignment(Qt::AlignCenter);
	pauseText->setStyleSheet(R"(
        QLabel {
            font-size: 48px;
            font-weight: bold;
            color: white;
            background-color: rgba(0, 0, 0, 100);
            padding: 20px 40px;
            border-radius: 15px;
        }
    )");
	pauseText->setFixedWidth(300);
	layout->addWidget(pauseText, 0, Qt::AlignCenter);

	layout->addSpacing(20); // Thêm khoảng cách trước button

	// Resume button
	m_resumeButton = new QPushButton(tr("Resume"), this);
	m_resumeButton->setMinimumSize(200, 50);
	m_resumeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4CAF50;
            color: white;
            font-size: 18px;
            font-weight: bold;
            border-radius: 10px;
            padding: 10px 30px;
        }
        QPushButton:hover {
            background-color: #45A049;
        }
        QPushButton:pressed {
            background-color: #3D8B40;
        }
    )");
	connect(m_resumeButton, &QPushButton::clicked, this, &PauseOverlay::resumeClicked);
	layout->addWidget(m_resumeButton, 0, Qt::AlignCenter);

	// New Game button
	m_newGameButton = new QPushButton(tr("New Game"), this);
	m_newGameButton->setMinimumSize(200, 50);
	m_newGameButton->setStyleSheet(R"(
        QPushButton {
            background-color: #2196F3;
            color: white;
            font-size: 18px;
            font-weight: bold;
            border-radius: 10px;
            padding: 10px 30px;
        }
        QPushButton:hover {
            background-color: #1976D2;
        }
        QPushButton:pressed {
            background-color: #1565C0;
        }
    )");
	connect(m_newGameButton, &QPushButton::clicked, this, &PauseOverlay::newGameClicked);
	layout->addWidget(m_newGameButton, 0, Qt::AlignCenter);
}
//--------------------------------------------------------------------------------------------------
//  PAINT EVENT
//--------------------------------------------------------------------------------------------------
void PauseOverlay::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event)

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// Semi-transparent dark overlay
	painter.fillRect(rect(), QColor(0, 0, 0, 180));
}

//--------------------------------------------------------------------------------------------------
//  SHOW OVERLAY
//--------------------------------------------------------------------------------------------------
void PauseOverlay::showOverlay()
{
	if (parentWidget())
	{
		setGeometry(parentWidget()->rect());
	}
	show();
	raise();
}

//--------------------------------------------------------------------------------------------------
//  HIDE OVERLAY
//--------------------------------------------------------------------------------------------------
void PauseOverlay::hideOverlay() { hide(); }
