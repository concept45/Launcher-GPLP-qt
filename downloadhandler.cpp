#include "downloadhandler.h"

DownloadHandler::DownloadHandler(QWidget* parent) :
    QDialog(parent)
{
    manager = nullptr;
    reply = nullptr;
    file = nullptr;
    progressDialog = new QProgressDialog(this);

    connect(progressDialog, &QProgressDialog::canceled, [=]()
    {
        httpRequestAborted = true;
        reply->abort();
    });
}

DownloadHandler::~DownloadHandler()
{
}

void DownloadHandler::RequestDownload(QUrl url)
{
    manager = new QNetworkAccessManager(this);

    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();

    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName))
        QFile::rename(fileName, QString(fileName + "_backup"));

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Descargas"),
                      tr("Error al guardar el fichero %1: %2.")
                      .arg(fileName).arg(file->errorString()));
        delete file;
        file = nullptr;
        return;
    }

    httpRequestAborted = false;

    progressDialog->setWindowTitle(tr("Descarga"));
    progressDialog->setLabelText(tr("Descargando %1.").arg(fileName));

    ConnectAndStart(url);
}

// This will be called when download button is clicked
void DownloadHandler::ConnectAndStart(QUrl url)
{
    reply = manager->get(QNetworkRequest(url));

    connect(reply, QNetworkReply::readyRead, [=]()
    {
        if (file)
            file->write(reply->readAll());
    });

    connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesRead, qint64 totalBytes)
    {
        if (httpRequestAborted)
            return;

        progressDialog->setMaximum(totalBytes);
        progressDialog->setValue(bytesRead);
    });

    // Se emite al terminar
    connect(reply, QNetworkReply::finished, [=]()
    {
        if (httpRequestAborted)
        {
            if (file)
            {
                file->close();
                file->remove();
                delete file;
                file = nullptr;
            }

            reply->deleteLater();
            progressDialog->hide();
            return;
        }

        progressDialog->hide();
        file->flush();
        file->close();

        QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (reply->error())
        {
            file->remove();
            QMessageBox::information(this, tr("Descarga"), tr("Descarga fallida: %1.").arg(reply->errorString()));
        }
        else if (!redirectionTarget.isNull())
        {
            QUrl newUrl = url.resolved(redirectionTarget.toUrl());
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            ConnectAndStart(newUrl);
            return;
        }

        if (QFile::exists(QString(file->fileName() + "_backup")))
            QFile::remove(QString(file->fileName() + "_backup"));

        reply->deleteLater();
        reply = nullptr;
        delete file;
        file = nullptr;
        manager = nullptr;
        this->deleteLater();
    });
}
