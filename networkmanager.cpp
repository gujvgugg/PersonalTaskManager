#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &NetworkManager::onReplyFinished);
}

void NetworkManager::checkConnection()
{

    // jsonplaceholder is a good free fake API
    QUrl url("https://jsonplaceholder.typicode.com/todos/1");
    QNetworkRequest request(url);
    m_manager->get(request);
}

void NetworkManager::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
        // Just extract some info to prove it works
        if (response.contains("userId")) {
             emit connectionStatus(tr("网络连接正常: 已连接到云端服务器"));
        } else {
             emit connectionStatus(tr("网络连接正常: 但收到意外响应"));
        }
    } else {
        emit connectionStatus(tr("网络连接失败: ") + reply->errorString());
    }
    reply->deleteLater();
}



