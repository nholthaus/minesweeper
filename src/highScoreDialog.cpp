#include "highScoreDialog.h"
#include <QLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>

HighScoreDialog::HighScoreDialog(const QMap<HighScore::Difficulty, HighScoreModel>& models, QWidget* parent)
	: QDialog(parent)
{
	this->setWindowTitle(tr("High Scores"));
	this->setLayout(new QVBoxLayout);

	tabWidget = new QTabWidget(this);
	this->layout()->addWidget(tabWidget);

	int modelWidth = 0;
	int modelHeight = 0;
	int scrollBarWidth = this->style()->pixelMetric(QStyle::PM_ScrollBarExtent) + 2;

	QList<QTableView*> views;

	for (const auto& model : models)
	{
		auto page = new QWidget;
		tabWidget->addTab(page, QVariant::fromValue(model.difficulty()).toString());
		page->setLayout(new QVBoxLayout);
		const auto view = new QTableView(this);
		view->setModel(const_cast<HighScoreModel*>(&model));
		page->layout()->addWidget(view);
		view->resizeColumnsToContents();
		view->resizeRowsToContents();

		QHeaderView *verticalHeader = view->verticalHeader();
		verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
		verticalHeader->setDefaultSectionSize(24);

		view->horizontalHeader()->setStretchLastSection(true);
		view->setShowGrid(false);
		views.append(view);
	}

	for (const auto view : views)
	{
		modelWidth = qMax(modelWidth, view->horizontalHeader()->length() + view->verticalHeader()->width() + scrollBarWidth);
		modelHeight = qMax(modelHeight, view->verticalHeader()->length() + view->horizontalHeader()->height() + 2);
	}

	for (const auto view : views)
		view->setFixedSize(modelWidth, modelHeight);

	this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void HighScoreDialog::setActiveTab(const QString& difficulty)
{
	for (int index = 0; index < tabWidget->count(); index++)
	{
		if (tabWidget->tabText(index) == difficulty)
		{
			tabWidget->setCurrentIndex(index);
			return;
		}
	}
}