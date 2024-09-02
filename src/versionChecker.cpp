#include "VersionChecker.h"

VersionChecker::VersionChecker(const QString& repoOwner, const QString& repoName, const QString& currentVersion, QObject* parent)
    : QObject(parent), m_repoOwner(repoOwner), m_repoName(repoName), m_networkManager(new QNetworkAccessManager(this))
{
    // Initialize the current version using the provided version string
    m_currentVersion = QVersionNumber::fromString(currentVersion);
}

void VersionChecker::checkForNewerVersion()
{
    // Construct the URL for the GitHub API to fetch the latest release information
    QString apiUrl = QString("https://api.github.com/repos/%1/%2/releases/latest")
                        .arg(m_repoOwner, m_repoName);

    // Correctly construct the QNetworkRequest object
    QUrl url(apiUrl);
    QNetworkRequest request(url);

    // Make a GET request to the API
    QNetworkReply* reply = m_networkManager->get(request);

    // Connect the finished signal to a lambda that will call onReplyFinished with the reply
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onReplyFinished(reply);
    });
}

void VersionChecker::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        // Emit an error signal if the request failed
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return;
    }

    // Parse the JSON response
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();

    // Extract the version string from the "tag_name" field
    QString latestVersionString = jsonObj["tag_name"].toString();

    // Compare the extracted version with the current version
    compareVersions(latestVersionString);

    // Clean up the network reply object
    reply->deleteLater();
}

void VersionChecker::compareVersions(const QString& latestVersionString)
{
    // Convert the latest version string to a QVersionNumber
    QVersionNumber latestVersion = QVersionNumber::fromString(latestVersionString);

    // Compare the current version with the latest version
    if (QVersionNumber::compare(latestVersion, m_currentVersion) > 0) {
        // Emit a signal if a newer version is available
        emit newerVersionAvailable(latestVersionString);
    } else {
        // Emit a signal if no newer version is found
        emit noNewerVersion();
    }
}