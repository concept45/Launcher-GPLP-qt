#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>


class DownloadHandler : public QDialog
{
    Q_OBJECT

public:
    DownloadHandler(QWidget* parent = nullptr);
    ~DownloadHandler();

public:
    void RequestDownload(QUrl url);

private:
    void ConnectAndStart(QUrl url);

    QUrl url;
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
    QProgressDialog* progressDialog;
    QFile* file;
    bool httpRequestAborted;
    qint64 fileSize;

};

#endif // FILEDOWNLOADER_H
