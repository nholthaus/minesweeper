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

#include <highScore.h>
#include <statistics.h>

#include <QObject>

//----------------------------------------------------------------------------------------------------------------------
//      CLASS: GameStats
//----------------------------------------------------------------------------------------------------------------------
/// @brief Stats about the player
//----------------------------------------------------------------------------------------------------------------------
class GameStats
{
	Q_GADGET
public:

	enum GameType
	{
		Win,
		Loss,
		Forfeit,
	};
	Q_ENUM(GameType)

	struct GameStatsData
	{
		Statistics<size_t> wins;
		Statistics<size_t> losses;
		Statistics<size_t> forfeits;
		Statistics<size_t> gamesPlayed;
	};

public:

	GameStats() = default;

	[[nodiscard]] size_t played(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] size_t wins(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] size_t losses(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] size_t forfeits(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] double winRate(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] double lossRate(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] double forfeitRate(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] size_t averageTimeToWin(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] size_t averageTimeToLoss(HighScore::Difficulty difficulty) const noexcept;
	[[nodiscard]] size_t averageTimeToForfeit(HighScore::Difficulty difficulty) const noexcept;

public slots:

	void addStat(HighScore::Difficulty difficulty, GameStats::GameType type, size_t time);

	friend QDataStream& operator<<(QDataStream& stream, const GameStats& stats);
	friend QDataStream& operator>>(QDataStream& stream, GameStats& stats);

private:

	std::map<HighScore::Difficulty, GameStatsData> stats;
};

#endif // GAMESTATS_H