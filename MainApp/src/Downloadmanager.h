#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include "UiProgressBar.h"

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	DownloadManager(QObject *parent = nullptr);

	void append(const QUrl &url);
	void append(const QStringList &urlList);
	QString saveFileName(const QUrl &url);

signals:
	void finished();

private slots:
	void startNextDownload();
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadFinished();
	void downloadReadyRead();

private:
	QNetworkAccessManager manager;
	QQueue<QUrl> downloadQueue;
	QNetworkReply *currentDownload;
	QFile output;
	QTime downloadTime;
	UiProgressBar progressBar;

	int downloadedCount;
	int totalCount;
};

