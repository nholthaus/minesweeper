// ---------------------------------------------------------------------------------------------------------------------
//
/// @file       gameStats.cpp
/// @author     Nic Holthaus
/// @date       9/24/2024
/// @copyright  (c) 2024 STR. The use of this software is subject to the terms and conditions outlined
///             in the LICENSE file. By using this software, the user agrees to be bound by the terms and
///             conditions set forth in the LICENSE file.
//
// ---------------------------------------------------------------------------------------------------------------------
//
/// @details    Implementation file for `gameStats.h`.
//
// ---------------------------------------------------------------------------------------------------------------------

//----------------------------
//  INCLUDES
//----------------------------

#include "gameStats.h"

//======================================================================================================================
//      MEMBER FUNCTIONS
//======================================================================================================================\

void GameStats::addStat(HighScore::Difficulty difficulty, GameType type, size_t time)
{
	switch (type)
	{
	case Forfeit:
		stats[difficulty].forfeits.insert(time);
		break;
	case Loss:
		stats[difficulty].losses.insert(time);
		break;
	case Win:
		stats[difficulty].wins.insert(time);
		break;
	}
	stats[difficulty].gamesPlayed.insert(time);
}

QDataStream& operator<<(QDataStream& stream, const GameStats& stats)
{
	stream << stats.stats.size();
	for (auto it = stats.stats.begin(); it != stats.stats.end(); ++it)
	{
		stream << it->first << it->second.wins << it->second.losses << it->second.forfeits << it->second.gamesPlayed;
	}
	return stream;
}

QDataStream& operator>>(QDataStream& stream, GameStats& stats)
{
	// Check if the stream is valid before attempting to read
	if (stream.status() != QDataStream::Ok)
		return stream;

	stats.stats.clear(); // Clear any existing data
	size_t size;
	stream >> size;

	for (size_t i = 0; i < size; ++i)
	{
		HighScore::Difficulty	 difficulty;
		GameStats::GameStatsData data;

		// Attempt to read data. If any part fails, return to avoid corrupted data
		stream >> difficulty;
		if (stream.status() != QDataStream::Ok)
			return stream;

		stream >> data.wins >> data.losses >> data.forfeits >> data.gamesPlayed;
		if (stream.status() != QDataStream::Ok)
			return stream;

		// Add the read data to the map
		stats.stats[difficulty] = data;
	}

	return stream;
}

size_t GameStats::played(HighScore::Difficulty difficulty) const noexcept { return this->stats.at(difficulty).gamesPlayed.count(); }

size_t GameStats::wins(HighScore::Difficulty difficulty) const noexcept { return this->stats.at(difficulty).wins.count(); }

size_t GameStats::losses(HighScore::Difficulty difficulty) const noexcept { return this->stats.at(difficulty).losses.count(); }

size_t GameStats::forfeits(HighScore::Difficulty difficulty) const noexcept { return this->stats.at(difficulty).forfeits.count(); }

double GameStats::winRate(HighScore::Difficulty difficulty) const noexcept { return 100.0 * (double)wins(difficulty) / (double)played(difficulty); }

double GameStats::lossRate(HighScore::Difficulty difficulty) const noexcept { return 100.0 * (double)losses(difficulty) / (double)played(difficulty); }

double GameStats::forfeitRate(HighScore::Difficulty difficulty) const noexcept { return 100.0 * (double)forfeits(difficulty) / (double)played(difficulty); }

size_t GameStats::averageTimeToWin(HighScore::Difficulty difficulty) const noexcept { return this->stats.at(difficulty).wins.mean(); }

size_t GameStats::averageTimeToLoss(HighScore::Difficulty difficulty) const noexcept { return this->stats.at(difficulty).losses.mean(); }

size_t GameStats::averageTimeToForfeit(HighScore::Difficulty difficulty) const noexcept { return this->stats.at(difficulty).forfeits.mean(); }