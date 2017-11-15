#include "highScore.h"

#include <QDataStream>
#include <QVariant>

HighScore::HighScore(QString name, Difficulty difficulty, quint32 score, QDateTime date)
	: QObject()
	, m_name(name)
	, m_difficulty(difficulty)
	, m_score(score)
	, m_date(date)
{

}

HighScore::HighScore(const HighScore& other)
	: m_name(other.m_name)
	, m_difficulty(other.m_difficulty)
	, m_score(other.m_score)
	, m_date(other.m_date)
{

}

HighScore& HighScore::operator=(const HighScore& other)
{
	m_name = other.m_name;
	m_difficulty = other.m_difficulty;
	m_score = other.m_score;
	m_date = other.m_date;
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

QDateTime HighScore::date() const
{
	return m_date;
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

void HighScore::setDate(QDateTime date)
{
	m_date = date;
}

QDataStream& operator<<(QDataStream &out, const HighScore& highScore)
{
	out << highScore.name() << QVariant::fromValue(highScore.difficulty()).toString() << highScore.score() << highScore.date();
	return out;
}

QDataStream& operator>>(QDataStream &in, HighScore& highScore)
{
	QString name;
	QString difficulty;
	quint32 score;
	QDateTime date;

	in >> name;
	in >> difficulty;
	in >> score;
	in >> date;

	highScore.setName(name);
	highScore.setDifficultty(QVariant(difficulty).value<HighScore::Difficulty>());
	highScore.setScore(score);
	highScore.setDate(date);

	return in;
}

bool HighScore::operator==(const HighScore& rhs) const
{
	return (m_difficulty == rhs.m_difficulty &&
			m_score == rhs.m_score &&
			m_date == rhs.m_date);
}