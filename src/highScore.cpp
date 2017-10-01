#include "highScore.h"

#include <QDataStream>
#include <QVariant>

HighScore::HighScore(QString name, Difficulty difficulty, quint32 score)
	: QObject()
	, m_name(name)
	, m_difficulty(difficulty)
	, m_score(score)
{

}

HighScore::HighScore(const HighScore& other)
	: m_name(other.m_name)
	, m_difficulty(other.m_difficulty)
	, m_score(other.m_score)
{

}

HighScore& HighScore::operator=(const HighScore& other)
{
	m_name = other.m_name;
	m_difficulty = other.m_difficulty;
	m_score = other.m_score;
	return *this;
}

QString HighScore::name() const
{
	return m_name;
}

HighScore::Difficulty HighScore::difficulty() const
{
	return m_difficulty;
}

quint32 HighScore::score() const
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

void HighScore::setDifficultty(Difficulty difficulty)
{
	m_difficulty = difficulty;
}

void HighScore::setScore(quint32 score)
{
	m_score = score;
}

QDataStream& operator<<(QDataStream &out, const HighScore& highScore)
{
	out << highScore.name() << QVariant::fromValue(highScore.difficulty()).toString() << highScore.score();
	return out;
}

QDataStream& operator>>(QDataStream &in, HighScore& highScore)
{
	QString name;
	QString difficulty;
	quint32 score;

	in >> name;
	in >> difficulty;
	in >> score;

	highScore.setName(name);
	highScore.setDifficultty(QVariant(difficulty).value<HighScore::Difficulty>());
	highScore.setScore(score);

	return in;
}

bool HighScore::operator==(const HighScore& rhs) const
{
	return (m_difficulty == rhs.m_difficulty &&
			m_score == rhs.m_score);
}

