#include "filedownloader.h"
#include <QFile>
#include <QCoreApplication>
#include <thread>

FileDownloader::FileDownloader(QUrl imageUrl, QObject* parent) :
    QObject(parent)
{
    _url = imageUrl;
}

FileDownloader::~FileDownloader()
{
}

void FileDownloader::Execute()
{
    qDebug(_url.toString().toString().toStdString().c_str());
    QNetworkRequest request(_url);
    QNetworkReply* reply = _networkMgr.get(request);
    while (!reply->isFinished())
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    _downloadedData = reply->readAll();
    reply->deleteLater();
    //emit downloaded();
    qDebug(_downloadedData.toString().toStdString().c_str());
    QString path = QCoreApplication::applicationDirPath() + "/prueba.txt";
    QFile localFile(path);
    if (!localFile.open(QIODevice::WriteOnly))
    {
        qDebug("error");
        return;
    }

    localFile.write(_downloadedData.toString().toStdString().c_str());
    localFile.close();
}
