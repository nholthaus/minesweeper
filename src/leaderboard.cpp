//--------------------------------------------------------------------------------------------------
//
/// @file   leaderboard.cpp
/// @brief  Implementation of Leaderboard
//
//--------------------------------------------------------------------------------------------------

#include "leaderboard.h"
#include "gameController.h"

#include <algorithm>

//--------------------------------------------------------------------------------------------------
//  DATA STREAM OPERATORS
//--------------------------------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& out, const LeaderboardEntry& entry)
{
	out << entry.name << entry.score << entry.date << static_cast<int>(entry.difficulty) << static_cast<int>(entry.mode) << entry.customRows << entry.customCols
		<< entry.customMines;
	return out;
}

QDataStream& operator>>(QDataStream& in, LeaderboardEntry& entry)
{
	int difficulty, mode;
	in >> entry.name >> entry.score >> entry.date >> difficulty >> mode >> entry.customRows >> entry.customCols >> entry.customMines;

	entry.difficulty = static_cast<GameEnums::Difficulty>(difficulty);
	entry.mode		 = static_cast<GameEnums::GameMode>(mode);
	return in;
}

//--------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//--------------------------------------------------------------------------------------------------
Leaderboard::Leaderboard(QObject* parent)
	: QObject(parent)
{
}

//--------------------------------------------------------------------------------------------------
//  ADD ENTRY
//--------------------------------------------------------------------------------------------------
void Leaderboard::addEntry(const LeaderboardEntry& entry)
{
	CategoryKey key = makeKey(entry.difficulty, entry.mode);
	m_entries[key].append(entry);
	sortAndTrim(key);
	emit entryAdded(entry);
}

//--------------------------------------------------------------------------------------------------
//  IS HIGH SCORE
//--------------------------------------------------------------------------------------------------
bool Leaderboard::isHighScore(quint32 time, GameEnums::Difficulty difficulty, GameEnums::GameMode mode) const
{
	CategoryKey key = makeKey(difficulty, mode);

	if (!m_entries.contains(key))
		return true;

	const auto& entries = m_entries[key];

	if (entries.size() < MAX_ENTRIES_PER_CATEGORY)
		return true;

	// Check if time is better than the worst entry
	return time < entries.last().score;
}

//--------------------------------------------------------------------------------------------------
//  GET ENTRIES
//--------------------------------------------------------------------------------------------------
QVector<LeaderboardEntry> Leaderboard::getEntries(GameEnums::Difficulty difficulty, GameEnums::GameMode mode) const
{
	CategoryKey key = makeKey(difficulty, mode);
	return m_entries.value(key);
}

//--------------------------------------------------------------------------------------------------
//  GET ALL ENTRIES
//--------------------------------------------------------------------------------------------------
QVector<LeaderboardEntry> Leaderboard::getAllEntries() const
{
	QVector<LeaderboardEntry> allEntries;

	for (const auto& entries : m_entries)
	{
		allEntries.append(entries);
	}

	std::sort(allEntries.begin(), allEntries.end());
	return allEntries;
}

//--------------------------------------------------------------------------------------------------
//  IMPORT FROM HIGH SCORE MODELS
//--------------------------------------------------------------------------------------------------
void Leaderboard::importFromHighScoreModels(const QMap<HighScore::Difficulty, HighScoreModel>& models)
{
	for (auto it = models.constBegin(); it != models.constEnd(); ++it)
	{
		GameEnums::Difficulty difficulty = GameController::fromHighScoreDifficulty(it.key());

		for (const HighScore& hs : it.value().highScores())
		{
			LeaderboardEntry entry;
			entry.name		 = hs.name();
			entry.score		 = hs.score();
			entry.date		 = hs.date();
			entry.difficulty = difficulty;
			entry.mode		 = GameEnums::GameMode::Normal; // Original system was Normal mode only

			addEntry(entry);
		}
	}
}

//--------------------------------------------------------------------------------------------------
//  EXPORT TO HIGH SCORE MODELS
//--------------------------------------------------------------------------------------------------
QMap<HighScore::Difficulty, HighScoreModel> Leaderboard::exportToHighScoreModels() const
{
	QMap<HighScore::Difficulty, HighScoreModel> models;

	// Only export Normal mode entries (for compatibility)
	for (auto difficulty : {GameEnums::Difficulty::Beginner, GameEnums::Difficulty::Intermediate, GameEnums::Difficulty::Expert})
	{
		HighScore::Difficulty hsDiff = GameController::toHighScoreDifficulty(difficulty);
		models.insert(hsDiff, HighScoreModel(hsDiff));

		auto entries = getEntries(difficulty, GameEnums::GameMode::Normal);
		for (const auto& entry : entries)
		{
			HighScore hs(entry.name, hsDiff, entry.score, entry.date);
			models[hsDiff].addHighScore(hs);
		}
	}

	return models;
}

//--------------------------------------------------------------------------------------------------
//  SAVE
//--------------------------------------------------------------------------------------------------
void Leaderboard::save(QDataStream& stream) const
{
	stream << static_cast<qint32>(m_entries.size());

	for (auto it = m_entries.constBegin(); it != m_entries.constEnd(); ++it)
	{
		stream << static_cast<int>(it.key().first)	// difficulty
			   << static_cast<int>(it.key().second) // mode
			   << it.value();						// entries
	}
}

//--------------------------------------------------------------------------------------------------
//  LOAD
//--------------------------------------------------------------------------------------------------
void Leaderboard::load(QDataStream& stream)
{
	m_entries.clear();

	qint32 size;
	stream >> size;

	for (qint32 i = 0; i < size; ++i)
	{
		int						  difficulty, mode;
		QVector<LeaderboardEntry> entries;

		stream >> difficulty >> mode >> entries;

		CategoryKey key = makeKey(static_cast<GameEnums::Difficulty>(difficulty), static_cast<GameEnums::GameMode>(mode));
		m_entries[key]	= entries;
	}
}

//--------------------------------------------------------------------------------------------------
//  CLEAR
//--------------------------------------------------------------------------------------------------
void Leaderboard::clear()
{
	m_entries.clear();
	emit leaderboardCleared();
}

//--------------------------------------------------------------------------------------------------
//  MAKE KEY
//--------------------------------------------------------------------------------------------------
Leaderboard::CategoryKey Leaderboard::makeKey(GameEnums::Difficulty difficulty, GameEnums::GameMode mode) const { return qMakePair(difficulty, mode); }

//--------------------------------------------------------------------------------------------------
//  SORT AND TRIM
//--------------------------------------------------------------------------------------------------
void Leaderboard::sortAndTrim(CategoryKey key)
{
	if (!m_entries.contains(key))
		return;

	auto& entries = m_entries[key];

	// Sort by score (ascending - lower time is better)
	std::sort(entries.begin(), entries.end());

	// Trim to max entries
	while (entries.size() > MAX_ENTRIES_PER_CATEGORY)
	{
		entries.removeLast();
	}
}
