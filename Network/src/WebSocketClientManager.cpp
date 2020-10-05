#include "WebSocketClientManager.h"
WebSocketClientManager::WebSocketClientManager(QObject* parent)
	: QObject(parent)
	,m_bCloseByHand(false)
	, m_webSocketState(stateNone)
	, m_nHeartbeatTimer(5000)
	, m_nReconnectHeartbatTimerCount(3)
	, m_nReconnectTimer(5000)
	, m_nDisconnectReconnectCount(3)
	, m_nHeartbeatFailedCount(0)
	, m_nReconnectFailedCount(0)
	, m_strURL(localWsServer)
	, m_strSecWebsocketProtocol(tr("ws"))
	, m_strHeartbeatText(tr("ping"))
{
	connect(&m_websocket, SIGNAL(connected()), this, SLOT(onConnected()));
	connect(&m_websocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	connect(&m_websocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
	connect(&m_timerHeartbeat, SIGNAL(timeout()), this, SLOT(onSendHeartbeatText()));
	connect(&m_timerReconnect, SIGNAL(timeout()), this, SLOT(onReconnect()));
}

WebSocketClientManager::~WebSocketClientManager()
{
	if (disconnected != m_webSocketState)
	{
		disconnect();
	}
}


void WebSocketClientManager::run()
{
	while (true)
	{
		if (m_webSocketState == connected)
		{
			m_mutex.lock();
			if (m_sendTextLinkedList.size())
			{
				QString strVal = m_sendTextLinkedList.first();
				if (m_websocket.sendTextMessage(strVal) == strVal.toLocal8Bit().length())
				{
					m_sendTextLinkedList.erase(m_sendTextLinkedList.begin());
				}
				m_mutex.unlock();
			}
			else
			{
				m_mutex.unlock();
			}

		}
		else if (m_bCloseByHand)
		{
			break;
		}
	}
}


bool WebSocketClientManager::startConnect()
{
	bool bRet = false;

	// 如果URL地址 或者 特定的协议头为空 返回false
	if (!m_strURL.isEmpty() && !m_strSecWebsocketProtocol.isEmpty())
	{
		if (!thread.isRunning())
		{
			thread.start();
		}
		QNetworkRequest request;
		request.setUrl(QUrl(m_strURL));
		QByteArray byteHeader = "Protocol";
		request.setRawHeader(byteHeader, m_strSecWebsocketProtocol.toLocal8Bit());
		m_websocket.open(request);
		bRet = true;
	}

	return bRet;

}

void WebSocketClientManager::disconnect()
{
	m_bCloseByHand = true;
	m_websocket.close();
	m_timerHeartbeat.stop();
	m_timerReconnect.stop();
}


void WebSocketClientManager::setConnectUrl(QString val)
{
	m_strURL = val;
}

bool WebSocketClientManager::addSendText(QString val)
{
	bool bRet = false;
	if (m_webSocketState == disconnected || m_webSocketState == stateNone)
	{
		bRet = false;
	}
	else
	{
		m_mutex.lock();
		m_sendTextLinkedList.append(val);
		m_mutex.unlock();
		bRet = true;
	}
	return bRet;
}

void WebSocketClientManager::setSecWebSocketProtocolHeader(QString val)
{
	m_strSecWebsocketProtocol = val;
}

void WebSocketClientManager::setHeartbeatTimer(int val)
{
	if (val > 5000)
	{
		m_nHeartbeatTimer = val;
	}
}

void WebSocketClientManager::setReconnetHeartbeatTimerCount(int val)
{
	if (val > 3)
	{
		m_nReconnectHeartbatTimerCount = val;
	}
}


void WebSocketClientManager::setReconnectTimer(int val)
{
	if (val > 5000)
	{
		m_nReconnectTimer = val;
	}
}

void WebSocketClientManager::setDisconnectReconnectCount(int val)
{
	if (val > 3)
	{
		m_nDisconnectReconnectCount = val;
	}
}


WebSocketClientManager::webSocketState WebSocketClientManager::getCurrentState()
{
	return m_webSocketState;
}

void WebSocketClientManager::onConnected()
{
	emit showMsg(int(0), QString(""), QString::fromLocal8Bit("服务器已连接"));
	// 如果是重连成功 停止重连的定时Timer
	if (m_webSocketState == reconnecting)
	{
		m_timerReconnect.stop();
	}
	m_webSocketState = connected;
	m_timerHeartbeat.start(m_nHeartbeatTimer);
	m_nReconnectFailedCount = 0;
	m_nHeartbeatFailedCount = 0;
}


void WebSocketClientManager::onDisconnected()
{
	// 如果不是手动关闭 则需要重连
	if (!m_bCloseByHand)
	{
		m_timerHeartbeat.stop();
		m_timerReconnect.start(m_nReconnectTimer);
		m_webSocketState = reconnecting;
	}
}


bool WebSocketClientManager::bindLogData(const QObject* receiver, const char* method)
{
	return QObject::connect(WebSocketClientManager::instance(), SIGNAL(showMsg(const int,const QString,const QString)), receiver, method, Qt::QueuedConnection);
}

void WebSocketClientManager::onTextMessageReceived(const QString& val)
{
	//收到网页后端发来的内容，处理内容
	if (val.startsWith(tr("pong"))) {
		return;
	}
	else {
		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(val.toUtf8(), &error);
		if (QJsonParseError::NoError == error.error)
		{
			int index;
			QString qq;
			QString show;
			QVariantMap map = document.toVariant().toMap();
			if (map.contains("id"))
			{
				 index = map["id"].toInt();
			}
			if (map.contains("qq"))
			{
				qq = map["qq"].toString();
			}
			if (map.contains("show"))
			{
				show = map["show"].toString();
			}
			emit showMsg(index, qq, show);
		}
	}
}


void WebSocketClientManager::onSendHeartbeatText()
{
	// 加锁 保证websocket 只在同一时间发送一条数据
	m_mutex.lock();
	int nSendByte = m_websocket.sendTextMessage(m_strHeartbeatText);
	m_mutex.unlock();
	// 发送心跳失败
	if (nSendByte != m_strHeartbeatText.toLocal8Bit().length())
	{
		m_nHeartbeatFailedCount++;
		// 失败次数等于启动重连的次数 停止发送心跳 开启重连 更新状态
		if (m_nHeartbeatFailedCount == m_nReconnectHeartbatTimerCount)
		{
			m_timerHeartbeat.stop();
			m_timerReconnect.start(m_nReconnectTimer);
			m_webSocketState = reconnecting;
		}
	}
}


void WebSocketClientManager::onReconnect()
{
	emit showMsg(int(0), QString(""), QString::fromLocal8Bit("等待服务器连接"));
	// close websocket
	m_websocket.close();
	//如果重连次数已到 不再重连 处于断开状态
	if (m_nReconnectFailedCount == m_nReconnectHeartbatTimerCount)
	{
		m_webSocketState = disconnected;
		m_timerReconnect.stop();
	}
	else // 开始连接并计数
	{
		startConnect();
		//无限重连
		//m_nReconnectFailedCount++;
	}
}
