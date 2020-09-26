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

	// 启动连接 - 必须把url地址 协议头 设置完成后再启动
	bool startConnect();

	// 断开连接
	void disconnect();

	//设置链接的URL
	void setConnectUrl(QString val);

	// 向协议队列中添加要发送的内容
	bool addSendText(QString val);

	// 设置websocket 的 Sec—Websocket-Protocol 头
	void setSecWebSocketProtocolHeader(QString val);

	// 设置心跳数据发送时间间隔 单位秒
	void setHeartbeatTimer(int val);

	// 设置判断需要重连的心跳发送失败次数
	void setReconnetHeartbeatTimerCount(int val);

	// 设置重连的时间间隔 单位秒
	void setReconnectTimer(int val);

	// 设置判断断开的重连次数
	void setDisconnectReconnectCount(int val);

	// 获取当前状态
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
	// 连接成功
	void onConnected();

	// 连接断开
	void onDisconnected();

	// 接收到 协议内容
	void onTextMessageReceived(const QString& val);

	// 定时发送心跳
	void onSendHeartbeatText();

	// 定时重连
	void onReconnect();

private:
	webSocketState m_webSocketState;					// websocket 状态
	bool					    m_bCloseByHand;						// 手动关闭
	QMutex					m_mutex;							// 数据同步
	QLinkedList<QString>	m_sendTextLinkedList;				// 协议链表
	QString					m_strURL;							// URL地址
	QString					m_strSecWebsocketProtocol;			// 协议头 目前使用此协议头 与网页端保持一致
	QString					m_strHeartbeatText;					// 心跳内容
	QWebSocket				m_websocket;						// websocket 客户端
	int						m_nHeartbeatTimer;					// 心跳数据发送时间间隔 单位ms
	int						m_nReconnectHeartbatTimerCount;		// 需要重连的心跳发送失败次数
	int						m_nReconnectTimer;					// 重连的时间间隔 单位ms
	int						m_nDisconnectReconnectCount;		// 重连失败次数
	int						m_nHeartbeatFailedCount;			// 心跳发送失败次数
	int						m_nReconnectFailedCount;			// 重连失败次数
	QTimer					m_timerHeartbeat;					// 心跳发送定时器
	QTimer					m_timerReconnect;					// 断线重连定时器
	QThread thread;
};

