//--------------------------------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files (the "Software"), to deal in the Software without 
// restriction, including without limitation the rights to use, copy, modify, merge, publish, 
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or 
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//--------------------------------------------------------------------------------------------------
//
// Copyright (c) 2017 Nic Holthaus
// 
//--------------------------------------------------------------------------------------------------
//
// ATTRIBUTION:
//
//
//--------------------------------------------------------------------------------------------------
//
/// @file	highScore.h
/// @brief	Class to contain a minesweeper high score
//
//--------------------------------------------------------------------------------------------------

#pragma once

//-------------------------
//	INCLUDES
//-------------------------

#include <QString>
#include <QDataStream>
#include <QDateTime>

//-------------------------
//	FORWARD DECLARATIONS
//-------------------------

class HighScoreModel;

//--------------------------------------------------------------------------------------------------
//	CLASS HIGHSCORE
//--------------------------------------------------------------------------------------------------
class HighScore : public QObject
{
	Q_OBJECT

public:

	enum Difficulty
	{
		beginner,
		intermediate,
		expert,
		custom,
	};
	Q_ENUM(Difficulty);

public:

	HighScore() = default;
	HighScore(QString name, Difficulty difficulty, quint32 score, QDateTime date);
	HighScore(const HighScore& other);
	HighScore& operator=(const HighScore& other);

	QString name() const;
	Difficulty difficulty() const;
	quint32 score() const;
	QDateTime date() const;

	void setName(QString name);
	void setDifficultty(Difficulty difficulty);
	void setScore(quint32 score);
	void setDate(QDateTime date);

	bool operator<(const HighScore& rhs) const;
	bool operator==(const HighScore& rhs) const;

	friend HighScoreModel;

private:

	QString	m_name;
	Difficulty	m_difficulty;
	quint32	m_score;
	QDateTime m_date;
};

Q_DECLARE_METATYPE(HighScore);

QDataStream &operator<<(QDataStream &out, const HighScore&);
QDataStream &operator>>(QDataStream &in, HighScore&);