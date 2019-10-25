#include "highScoreModel.h"
#include <cassert>

constexpr int MAX_HIGH_SCORES = 10;

HighScoreModel::HighScoreModel(HighScore::Difficulty difficulty, QObject* parent)
	: QAbstractItemModel(parent)
	, m_difficulty(difficulty)
{

}

HighScoreModel::HighScoreModel(const HighScoreModel& other)
	: m_difficulty(other.m_difficulty)
	, m_highScores(other.m_highScores)
{
	beginInsertRows(QModelIndex(), 0, other.m_highScores.size() - 1);
	beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
	m_difficulty = other.m_difficulty;
	m_highScores = other.m_highScores;
	endInsertRows();
	endInsertColumns();

	assert(rowCount() == other.rowCount());
	assert(columnCount() == other.columnCount());
}

HighScoreModel& HighScoreModel::operator=(const HighScoreModel& other)
{
	beginInsertRows(QModelIndex(), 0, other.m_highScores.size() - 1);
	beginInsertColumns(QModelIndex(), 0, columnCount() - 1);
	m_difficulty = other.m_difficulty;
	m_highScores = other.m_highScores;
	endInsertRows();
	endInsertColumns();
	return *this;
}

void HighScoreModel::addHighScore(HighScore score)
{
//	Q_ASSERT(score.difficulty() == difficulty());

	bool scoreInserted = false;

	beginInsertRows(QModelIndex(), rowCount(), rowCount());

	if (m_highScores.isEmpty())
	{
		m_highScores.append(score);
		return;
	}

	QVector<HighScore>::iterator existingScore;
	for (existingScore = m_highScores.begin(); existingScore != m_highScores.end(); ++existingScore)
	{
		if (score < *existingScore)
		{
			m_highScores.insert(existingScore, score);
			scoreInserted = true;
			break;
		}
	}

	// otherwise insert at end
	if(!scoreInserted)
		m_highScores.insert(m_highScores.end(), score);

	// prune to 10 high scores
	while (m_highScores.size() > MAX_HIGH_SCORES)
		m_highScores.removeLast();

	endInsertRows();
}

HighScore::Difficulty HighScoreModel::difficulty() const
{
	return m_difficulty;
}

void HighScoreModel::setDifficulty(HighScore::Difficulty difficulty)
{
	m_difficulty = difficulty;
}

QModelIndex HighScoreModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
	switch (column)
	{
	case Column::Name:
		return createIndex(row, column, Column::Name);
	case Column::Score:
		return createIndex(row, column, Column::Score);
	case Column::Date:
		return createIndex(row, column, Column::Date);
	default:
		return QModelIndex();
	}
}

QModelIndex HighScoreModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

int HighScoreModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return m_highScores.size();
}

int HighScoreModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return 3;
}

QVariant HighScoreModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case Column::Name:
			return m_highScores[index.row()].name();
		case Column::Score:
			return m_highScores[index.row()].score();
		case Column::Date:
			return m_highScores[index.row()].date();
		default:
			return QVariant();
		}
	}
	else if (role == Qt::TextAlignmentRole)
	{
		QFlags<Qt::AlignmentFlag> alignment;
		switch (index.column())
		{
		case Column::Name:
			alignment = Qt::AlignLeft | Qt::AlignVCenter;
		case Column::Score:
			alignment = Qt::AlignHCenter | Qt::AlignVCenter;
		case Column::Date:
			alignment = Qt::AlignHCenter | Qt::AlignVCenter;
		default:
			alignment = Qt::AlignHCenter | Qt::AlignVCenter;
		}

		return QVariant(alignment);
	}

	return QVariant();
}

Qt::ItemFlags HighScoreModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEnabled;
}

QVariant HighScoreModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role == Qt::DisplayRole)
	{
		if(orientation == Qt::Horizontal)
		{

			switch (section)
			{
			case Column::Name:
				return tr("Name");
			case Column::Score:
				return tr("Score");
			case Column::Date:
				return tr("Date");
			default:
				return QVariant();
			}
		}
		else if (orientation == Qt::Vertical)
		{
			return (section + 1);
		}
	}
	return QVariant();
}

const QVector<HighScore>& HighScoreModel::highScores() const
{
	return m_highScores;
}

void HighScoreModel::setHighScores(QVector<HighScore> scores)
{
	beginInsertRows(QModelIndex(), 0, scores.size() - 1);
	for (auto score : scores)
		addHighScore(score);
	endInsertRows();
}

bool HighScoreModel::isHighScore(int time) const
{
	return (rowCount() < MAX_HIGH_SCORES || time < index(rowCount() - 1, Score).data().toInt());
}

QDataStream& operator<<(QDataStream &out, const HighScoreModel& model)
{
	out << QVariant::fromValue(model.difficulty()).toString();
	out << model.highScores();
	return out;
}

QDataStream& operator>>(QDataStream &in, HighScoreModel& model)
{
	QString difficulty;
	QVector<HighScore> scores;

	in >> difficulty;
	in >> scores;

	model.setDifficulty(QVariant(difficulty).value<HighScore::Difficulty>());
	model.setHighScores(scores);
	return in;
}
