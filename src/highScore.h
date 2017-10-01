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

//-------------------------
//	FORWARD DECLARATIONS
//-------------------------

//--------------------------------------------------------------------------------------------------
//	CLASS HIGHSCORE
//--------------------------------------------------------------------------------------------------
class HighScore
{
public:

	HighScore() = default;
	HighScore(QString name, QString difficulty, qint32 score);

	QString name() const;
	QString difficulty() const;
	qint32 score() const;

	bool operator<(const HighScore& rhs) const;
	bool operator==(const HighScore& rhs) const;

	friend QDataStream;

protected:

	void setName(QString name);
	void setDifficultty(QString difficulty);
	void setScore(int score);

private:

	QString	m_name;
	QString	m_difficulty;
	qint32	m_score;
};

QDataStream &operator<<(QDataStream &out, const HighScore&);
QDataStream &operator>>(QDataStream &in, HighScore&);