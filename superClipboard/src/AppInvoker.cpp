/*
 * AppInvoker.cpp
 *
 *  Created on: 2014-5-3
 *      Author: qGG
 */

#include "AppInvoker.h"
#include <bb/data/JsonDataAccess>
#include <QUrl>

AppInvoker::AppInvoker(QObject* parent)
	:	QObject(parent)
{

}

AppInvoker::~AppInvoker()
{
}

void AppInvoker::feebbackByEmail()
{
	bb::system::InvokeRequest request;
	request.setTarget("sys.pim.uib.email.hybridcomposer");
	request.setAction("bb.action.OPEN, bb.action.SENDEMAIL");
	//request.setMimeType("message/rfc822");
	request.setUri("mailto:none.lemming@gmail.com?subject=superClipboard%20feedback%20V0.5.1&body=Thank%20you%20for%20feedback!");
	//qDebug()<<QUrl::frome

	/*QVariantMap map;
	map["to"] = "qggviii@163.com";
	map["subject"] = "feedback superClipboard v0.5 z10-2";
	map["body"] = "hei,man";
	map["cc"] = "";
	map["bcc"] = "";
	map["attachment"] = "";
	map["accountid"] = "";
	QByteArray data;
	//QString str;
	bb::data::JsonDataAccess jda;
	jda.saveToBuffer(QVariant(map),&data);
	//jda.saveToBuffer(QVariant(map),&str);
	qDebug()<<map;
	qDebug()<<data;
	//qDebug()<<str;
	request.setData(data);
	request.setMetadata(map);*/

	bb::system::InvokeTargetReply* reply = m_invokeManager.invoke(request);
	if (reply)
	{
		bool result = connect(reply, SIGNAL(finished()), this, SLOT(onInvocationFinished()));
		Q_ASSERT(result);
		result = connect(&m_invokeManager,SIGNAL(childCardDone(const bb::system::CardDoneMessage&)),
						this, SLOT(onCardDone(const bb::system::CardDoneMessage&)));
	}
	else
	{
		// error handling goes here
	}
}

void AppInvoker::reviewOnAppWorld()
{
	bb::system::InvokeRequest request;
	request.setTarget("sys.appworld");
	request.setAction("bb.action.OPEN");
	request.setUri("appworld://search/lemming");
	bb::system::InvokeTargetReply* reply = m_invokeManager.invoke(request);
	if (reply)
	{
		bool result = connect(reply, SIGNAL(finished()), this, SLOT(onInvocationFinished()));
		Q_ASSERT(result);
		result = connect(&m_invokeManager,SIGNAL(childCardDone(const bb::system::CardDoneMessage&)),
						this, SLOT(onCardDone(const bb::system::CardDoneMessage&)));
	}
	else
	{
		// error handling goes here
	}
}

void AppInvoker::onInvocationFinished()
{
	bb::system::InvokeTargetReply* reply = qobject_cast<bb::system::InvokeTargetReply*>(sender());
	if(reply->error()){
	// error handling goes here
	}
	reply->deleteLater();
	qDebug()<<"onInvocationFinished";
}

void AppInvoker::onCardDone(const bb::system::CardDoneMessage& message)
{
	qDebug()<<"onCardDone";
}
