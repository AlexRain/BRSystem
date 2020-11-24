
#include <QString>
#include <QDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFileInfo>
#include <QUrl>
#include "ui_UiProgressBar.h"

class UiProgressBar :public QDialog {
	Q_OBJECT
public:
	UiProgressBar(QWidget* parent);
	~UiProgressBar();
	void setUiTitle(QString title);
	void setUrl(QString url);
	void download(QString fileName);
	static void UnDecodezip(QString fileName);
	

private slots:
	void readContent();
	void replyFinished(QNetworkReply*);
	void loadError(QNetworkReply::NetworkError);
	void loadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
	Ui::UiProgressBar ui;
	QNetworkReply *currentDownload;
	QFile *file;
	QUrl download_url;
};