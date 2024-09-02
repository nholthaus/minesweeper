#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVersionNumber>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class VersionChecker : public QObject
{
	Q_OBJECT

public:

	explicit VersionChecker(const QString& repoOwner, const QString& repoName, const QString& currentVersion, QObject* parent = nullptr);
	void     checkForNewerVersion();

signals:

	void newerVersionAvailable(const QString& latestVersion, const QString& latestUrl);
	void noNewerVersion();
	void errorOccurred(const QString& errorString);

private slots:

	void onReplyFinished(QNetworkReply* reply);

private:

	QNetworkAccessManager* m_networkManager;
	QString                m_repoOwner;
	QString                m_repoName;
	QVersionNumber         m_currentVersion;

	bool isNewer(const QString& latestVersionString) const;
};

#endif // VERSIONCHECKER_H