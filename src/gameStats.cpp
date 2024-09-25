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