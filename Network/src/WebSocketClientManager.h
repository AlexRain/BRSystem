#pragma once

#include "NetworkDefine.h"
#include "network_global.h"
#include <QNetworkAccessManager>
#include <QThread>
#include <QtCore>
#include <QtNetwork>
#include <QtWebSockets/QWebSocket>


class NETWORK_EXPORT WebSocketClientManager :public QObject
{
	Q_OBJECT


public:

	WebSocketClientManager(QObject* parent = nullptr);

	~WebSocketClientManager();

protected:
	virtual void run();

public:

	enum webSocketState {
		stateNone = 0,
		disconnected,
		connected,
		reconnecting,
	};

	// �������� - �����url��ַ Э��ͷ ������ɺ�������
	bool startConnect();

	// �Ͽ�����
	void disconnect();

	//�������ӵ�URL
	void setConnectUrl(QString val);

	// ��Э����������Ҫ���͵�����
	bool addSendText(QString val);

	// ����websocket �� Sec��Websocket-Protocol ͷ
	void setSecWebSocketProtocolHeader(QString val);

	// �����������ݷ���ʱ���� ��λ��
	void setHeartbeatTimer(int val);

	// �����ж���Ҫ��������������ʧ�ܴ���
	void setReconnetHeartbeatTimerCount(int val);

	// ����������ʱ���� ��λ��
	void setReconnectTimer(int val);

	// �����ж϶Ͽ�����������
	void setDisconnectReconnectCount(int val);

	// ��ȡ��ǰ״̬
	webSocketState getCurrentState();

public :
	static WebSocketClientManager* instance()
	{
		static WebSocketClientManager WebSocketClientManager;
		return &WebSocketClientManager;
	}

	bool bindLogData(const QObject* receiver, const char* method);


signals:
	void showMsg(const int, const QString,const QString);

private slots:
	// ���ӳɹ�
	void onConnected();

	// ���ӶϿ�
	void onDisconnected();

	// ���յ� Э������
	void onTextMessageReceived(const QString& val);

	// ��ʱ��������
	void onSendHeartbeatText();

	// ��ʱ����
	void onReconnect();

private:
	webSocketState m_webSocketState;					// websocket ״̬
	bool					    m_bCloseByHand;						// �ֶ��ر�
	QMutex					m_mutex;							// ����ͬ��
	QLinkedList<QString>	m_sendTextLinkedList;				// Э������
	QString					m_strURL;							// URL��ַ
	QString					m_strSecWebsocketProtocol;			// Э��ͷ Ŀǰʹ�ô�Э��ͷ ����ҳ�˱���һ��
	QString					m_strHeartbeatText;					// ��������
	QWebSocket				m_websocket;						// websocket �ͻ���
	int						m_nHeartbeatTimer;					// �������ݷ���ʱ���� ��λms
	int						m_nReconnectHeartbatTimerCount;		// ��Ҫ��������������ʧ�ܴ���
	int						m_nReconnectTimer;					// ������ʱ���� ��λms
	int						m_nDisconnectReconnectCount;		// ����ʧ�ܴ���
	int						m_nHeartbeatFailedCount;			// ��������ʧ�ܴ���
	int						m_nReconnectFailedCount;			// ����ʧ�ܴ���
	QTimer					m_timerHeartbeat;					// �������Ͷ�ʱ��
	QTimer					m_timerReconnect;					// ����������ʱ��
	QThread thread;
};

