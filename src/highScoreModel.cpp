#include "highScoreModel.h"

HighScoreModel::HighScoreModel(QVector<HighScore> highScores, QObject* parent)
	: QAbstractItemModel(parent)
	, m_highScores(highScores)
{

}

void HighScoreModel::addHighScore(HighScore score)
{
	m_highScores.append(score);
	qSort(m_highScores);
}

Q_INVOKABLE QModelIndex HighScoreModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
	switch (column)
	{
	case Column::rank:
		return createIndex(row, column, nullptr);
	case Column::name:
		return createIndex(row, column, &m_highScores[row].name());
	case Column::difficulty:
		return createIndex(row, column, m_highScores[row].difficulty());
	case Column::score:
		return createIndex(row, column, m_highScores[row].score());
	default:
		return QModelIndex();
	}
}

Q_INVOKABLE QModelIndex HighScoreModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

Q_INVOKABLE int HighScoreModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return 10;
}

Q_INVOKABLE int HighScoreModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return 4;
}

Q_INVOKABLE QVariant HighScoreModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	switch (index.column())
	{
	case Column::rank:
		return index.row();
	case Column::name:
		return m_highScores[index.row()].name();
	case Column::difficulty:
		return m_highScores[index.row()].difficulty();
	case Column::score:
		return m_highScores[index.row()].score();
	default:
		return QVariant();
	}
}

const QVector<HighScore>& HighScoreModel::highScores() const
{
	return m_highScores;
}

void HighScoreModel::setHighScores(QVector<HighScore> scores)
{
	m_highScores = scores;
}

QDataStream& operator<<(QDataStream &out, const HighScoreModel& model)
{
	out << model.highScores();
	return out;
}

QDataStream& operator>>(QDataStream &in, HighScoreModel& model)
{
	QVector<HighScore> scores;
	in >> scores;
	model.setHighScores(scores);
	return in;
}
