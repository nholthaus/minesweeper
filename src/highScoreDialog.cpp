#include "highScoreDialog.h"
#include <QLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>

HighScoreDialog::HighScoreDialog(QMap<HighScore::Difficulty, HighScoreModel*> models, QWidget* parent)
	: QDialog(parent)
{
	this->setWindowTitle("High Scores");
	this->setLayout(new QVBoxLayout);

	tabWidget = new QTabWidget(this);
	this->layout()->addWidget(tabWidget);

	for (HighScoreModel* model : models.values())
	{
		auto page = new QWidget;
		tabWidget->addTab(page, QVariant::fromValue(model->difficulty()).toString());
		page->setLayout(new QVBoxLayout);
		auto view = new QTableView(this);
		view->setModel(model);
		page->layout()->addWidget(view);
		view->resizeColumnsToContents();

		QHeaderView *verticalHeader = view->verticalHeader();
		verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
		verticalHeader->setDefaultSectionSize(24);
	}
}

