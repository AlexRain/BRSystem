#include "UiProgressBar.h"
#include "DialogMsg.h"
#include <QByteArray>
#include <stdio.h>
#include <QDir>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <JlCompress.h>


UiProgressBar::UiProgressBar(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("插件下载"));
	ui.progressBar->setValue(0);
}


UiProgressBar::~UiProgressBar()
{

}
void UiProgressBar::setUiTitle(QString title)
{
	this->setWindowTitle(title);
}
void UiProgressBar::setUrl(QString url)
{
	download_url = QUrl(url);
}


void UiProgressBar::download(QString fileName)
{
	file = new QFile(fileName);
	qDebug() << "fileName" << fileName;
	file->open(QIODevice::WriteOnly);

	QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
	accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
	qDebug() << "download_url" << download_url;
	QNetworkRequest request(download_url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	currentDownload = accessManager->get(request);


	connect((QObject *)currentDownload, SIGNAL(readyRead()), this, SLOT(readContent()));
	connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	connect(currentDownload, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(loadError(QNetworkReply::NetworkError)));
	connect(currentDownload, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(loadProgress(qint64, qint64)));
}


void UiProgressBar::readContent()    //下载时向本地文件中写入数据
{
	file->write(currentDownload->readAll());
}

void UiProgressBar::replyFinished(QNetworkReply*)    //删除指针，更新和关闭文件
{
	if (currentDownload->error() == QNetworkReply::NoError)
	{
		currentDownload->deleteLater();
		file->flush();
		file->close();
	}
	else
	{
		DialogMsg::question(this, tr("warning"), QString::fromLocal8Bit("导出失败"), QMessageBox::Ok);
	}
	this->accept();

}

void UiProgressBar::loadProgress(qint64 bytesSent, qint64 bytesTotal)    //更新进度条
{
	qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
	ui.progressBar->setMaximum(bytesTotal); //最大值
	ui.progressBar->setValue(bytesSent);  //当前值
}

void UiProgressBar::loadError(QNetworkReply::NetworkError)    //传输中的错误输出
{
	qDebug() << "Error: " << currentDownload->error();
}


void UiProgressBar::UnDecodezip(QString fileName)
{
	qDebug() << "unzip " << fileName;
	JlCompress::extractDir(fileName, QApplication::applicationDirPath());//解压缩
}
