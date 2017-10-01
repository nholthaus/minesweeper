#include "highScore.h"

#include <QDataStream>

HighScore::HighScore(QString name, QString difficulty, qint32 score)
	: m_name(name)
	, m_difficulty(difficulty)
	, m_score(score)
{

}

QString HighScore::name() const
{
	return m_name;
}

QString HighScore::difficulty() const
{
	return m_difficulty;
}

int HighScore::score() const
{
	return m_score;
}

bool HighScore::operator<(const HighScore& rhs) const
{
	return m_score < rhs.m_score;
}

void HighScore::setName(QString name)
{
	m_name = name;
}

void HighScore::setDifficultty(QString difficulty)
{
	m_difficulty = difficulty;
}

void HighScore::setScore(qint32 score)
{
	m_score = score;
}

QDataStream& operator<<(QDataStream &out, const HighScore& score)
{
	out << score.name() << score.difficulty() << score.score();
	return out;
}

QDataStream& operator>>(QDataStream &in, HighScore& highScore)
{
	QString name;
	QString difficulty;
	qint32 score;

	in >> name;
	in >> difficulty;
	in >> score;

	return in;

}

bool HighScore::operator==(const HighScore& rhs) const
{
	return (m_difficulty == rhs.m_difficulty &&
			m_score == rhs.m_score);
}

