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
	this->setWindowTitle(QString::fromLocal8Bit("�������"));
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


void UiProgressBar::readContent()    //����ʱ�򱾵��ļ���д������
{
	file->write(currentDownload->readAll());
}

void UiProgressBar::replyFinished(QNetworkReply*)    //ɾ��ָ�룬���º͹ر��ļ�
{
	if (currentDownload->error() == QNetworkReply::NoError)
	{
		currentDownload->deleteLater();
		file->flush();
		file->close();
	}
	else
	{
		DialogMsg::question(this, tr("warning"), QString::fromLocal8Bit("����ʧ��"), QMessageBox::Ok);
	}
	this->accept();

}

void UiProgressBar::loadProgress(qint64 bytesSent, qint64 bytesTotal)    //���½�����
{
	qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
	ui.progressBar->setMaximum(bytesTotal); //���ֵ
	ui.progressBar->setValue(bytesSent);  //��ǰֵ
}

void UiProgressBar::loadError(QNetworkReply::NetworkError)    //�����еĴ������
{
	qDebug() << "Error: " << currentDownload->error();
}


void UiProgressBar::UnDecodezip(QString fileName)
{
	qDebug() << "unzip " << fileName;
	JlCompress::extractDir(fileName, QApplication::applicationDirPath());//��ѹ��
}
