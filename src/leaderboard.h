//--------------------------------------------------------------------------------------------------
//
/// @file   leaderboard.h
/// @brief  Extended leaderboard with game mode and difficulty filtering
//
//--------------------------------------------------------------------------------------------------

#pragma once

#include "gameEnums.h"
#include "highScore.h"
#include "highScoreModel.h"

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QVector>

//--------------------------------------------------------------------------------------------------
//  STRUCT: LeaderboardEntry
//--------------------------------------------------------------------------------------------------
struct LeaderboardEntry
{
	QString				  name;
	quint32				  score; // Time in seconds
	QDateTime			  date;
	GameEnums::Difficulty difficulty;
	GameEnums::GameMode	  mode;

	// Custom game settings (only valid if difficulty == Custom)
	quint32 customRows	= 0;
	quint32 customCols	= 0;
	quint32 customMines = 0;

	bool operator<(const LeaderboardEntry& rhs) const
	{
		return score < rhs.score; // Lower time is better
	}
};

Q_DECLARE_METATYPE(LeaderboardEntry)

QDataStream& operator<<(QDataStream& out, const LeaderboardEntry& entry);
QDataStream& operator>>(QDataStream& in, LeaderboardEntry& entry);

//--------------------------------------------------------------------------------------------------
//  CLASS: Leaderboard
//--------------------------------------------------------------------------------------------------
/// @brief  Extended leaderboard supporting filtering by mode and difficulty
//--------------------------------------------------------------------------------------------------
class Leaderboard : public QObject
{
	Q_OBJECT

public:

	static constexpr int MAX_ENTRIES_PER_CATEGORY = 10;

	explicit Leaderboard(QObject* parent = nullptr);

	//----------------------------------------------------------------------------------------------
	//  ENTRY MANAGEMENT
	//----------------------------------------------------------------------------------------------
	void addEntry(const LeaderboardEntry& entry);
	bool isHighScore(quint32 time, GameEnums::Difficulty difficulty, GameEnums::GameMode mode) const;

	//----------------------------------------------------------------------------------------------
	//  FILTERING
	//----------------------------------------------------------------------------------------------
	[[nodiscard]] QVector<LeaderboardEntry> getEntries(GameEnums::Difficulty difficulty, GameEnums::GameMode mode) const;

	[[nodiscard]] QVector<LeaderboardEntry> getAllEntries() const;

	//----------------------------------------------------------------------------------------------
	//  COMPATIBILITY WITH EXISTING HIGH SCORE SYSTEM
	//----------------------------------------------------------------------------------------------
	void										importFromHighScoreModels(const QMap<HighScore::Difficulty, HighScoreModel>& models);
	QMap<HighScore::Difficulty, HighScoreModel> exportToHighScoreModels() const;

	//----------------------------------------------------------------------------------------------
	//  PERSISTENCE
	//----------------------------------------------------------------------------------------------
	void save(QDataStream& stream) const;
	void load(QDataStream& stream);
	void clear();

signals:
	void entryAdded(const LeaderboardEntry& entry);
	void leaderboardCleared();

private:

	using CategoryKey = QPair<GameEnums::Difficulty, GameEnums::GameMode>;

	[[nodiscard]] CategoryKey makeKey(GameEnums::Difficulty difficulty, GameEnums::GameMode mode) const;
	void					  sortAndTrim(CategoryKey key);

private:

	QMap<CategoryKey, QVector<LeaderboardEntry>> m_entries;
};
