/*
 * ReplyWrap.cpp
 *
 *  Created on: 2014-5-17
 *      Author: qGG
 */

#include "ReplyWrap.h"
#include <bb/cascades/QmlDocument>

ReplyWrap::ReplyWrap(QObject* parent)
	: 	QObject(parent)
	,	m_reply(NULL)
	,	m_id(-1)
{

}

void ReplyWrap::setReply(QNetworkReply* reply)
{
	//转发reply的结束信号,当前者信号发射时,后者信号自动发射
	if (m_reply != NULL)
		disconnect(m_reply, SIGNAL(finished()), this, SIGNAL(finished()));
	m_reply = reply;
	bool ok = connect(m_reply, SIGNAL(finished()), this, SIGNAL(finished()));
	Q_ASSERT(ok);
}

QNetworkReply* ReplyWrap::ReplyWrap::reply()
{
	return m_reply;
}

void ReplyWrap::setId(int id)
{
	m_id = id;
}

int ReplyWrap::id()
{
	return m_id;
}
