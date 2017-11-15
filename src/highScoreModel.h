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
/// @file	highScoreModel.h
/// @brief	Model for minesweeper high scores
//
//--------------------------------------------------------------------------------------------------

#pragma once

//-------------------------
//	INCLUDES
//-------------------------

#include <QAbstractItemModel>
#include <QDataStream>
#include <QVector>

#include "highScore.h"

//-------------------------
//	FORWARD DECLARATIONS
//-------------------------

class HighScoreModel : public QAbstractItemModel
{
public:

	enum Column
	{
		Name	= 0,
		Score	= 1,
		Date	= 2,
	};
	Q_ENUM(Column);

public:

	HighScoreModel() = default;
	HighScoreModel(HighScore::Difficulty difficulty, QObject* parent = nullptr);
	HighScoreModel(const HighScoreModel& other);
	HighScoreModel& operator=(const HighScoreModel& other);

	void addHighScore(HighScore score);
	HighScore::Difficulty difficulty() const;
	void setDifficulty(HighScore::Difficulty difficulty);

	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex &child) const override;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	const QVector<HighScore>& highScores() const;
	void setHighScores(QVector<HighScore> scores);
	bool isHighScore(int time) const;

private:

	HighScore::Difficulty m_difficulty;
	QVector<HighScore> m_highScores;
};

Q_DECLARE_METATYPE(HighScoreModel);

QDataStream &operator<<(QDataStream &out, const HighScoreModel&);
QDataStream &operator>>(QDataStream &in, HighScoreModel&);