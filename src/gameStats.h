// ---------------------------------------------------------------------------------------------------------------------
//
/// @file       gameStats.h
/// @author     Nic Holthaus
/// @date       9/24/2024
///
/// @copyright The MIT License (MIT)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
/// and associated documentation files (the "Software"), to deal in the Software without
/// restriction, including without limitation the rights to use, copy, modify, merge, publish,
/// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all copies or
/// substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
/// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
/// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
/// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// ---------------------------------------------------------------------------------------------------------------------
//
/// @brief      Definition of the `GameStats` Class.
/// @details
//
// ---------------------------------------------------------------------------------------------------------------------

#ifndef GAMESTATS_H
#define GAMESTATS_H

//----------------------------
//  INCLUDES
//----------------------------

#include <statistics.h>
#include <highScore.h>

#include <QObject>

//----------------------------------------------------------------------------------------------------------------------
//      CLASS: GameStats
//----------------------------------------------------------------------------------------------------------------------
/// @brief Stats about the player
//----------------------------------------------------------------------------------------------------------------------
class GameStats : public QObject
{
	Q_OBJECT
public:

	enum GameType
	{
		Win,
		Loss,
		Forfeit,
	};

public:

public slots:

	void addStat(HighScore::Difficulty difficulty, GameStats::GameType type, size_t time);

private:

	struct GameStatsData
	{
		Statistics<size_t>    wins;
		Statistics<size_t>    losses;
		Statistics<size_t>    forfeits;
		Statistics<size_t>    gamesPlayed;
	};

	std::map<HighScore::Difficulty, GameStatsData> stats;
};

#endif //GAMESTATS_H