#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    explicit FileDownloader(QUrl imageUrl, QObject* parent = 0);
    virtual ~FileDownloader();

    void Execute();

    QByteArray GetDownloadedData() const { return _downloadedData; }

private:
    QUrl _url;
    QNetworkAccessManager _networkMgr;
    QByteArray _downloadedData;
};

#endif // FILEDOWNLOADER_H
