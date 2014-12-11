/*
 * AppInvoker.cpp
 *
 *  Created on: 2014-5-3
 *      Author: qGG
 */

#include "AppInvoker.h"
#include <bb/data/JsonDataAccess>
#include <QUrl>
#include <bb/device/HardwareInfo>
#include <bb/platform/PlatformInfo>
#include <bb/ApplicationInfo>

AppInvoker::AppInvoker(QObject* parent)
	:	QObject(parent)
{
	/*bb::device::HardwareInfo hardInfo;
	qDebug()<<hardInfo.modelName();
	qDebug()<<hardInfo.modelNumber();

	bb::platform::PlatformInfo platInfo;
	qDebug()<<platInfo.osVersion();

	bb::ApplicationInfo appInfo;
	qDebug()<<appInfo.version();
	qDebug()<<appInfo.title();

	qDebug()<<QString(QUrl("hello world ~!@#$%^&*()_+").toEncoded());*/
}

AppInvoker::~AppInvoker()
{
}

void AppInvoker::feebbackByEmail()
{
	bb::device::HardwareInfo hardInfo;
	bb::platform::PlatformInfo platInfo;
	bb::ApplicationInfo appInfo;

	bb::system::InvokeRequest request;
	request.setTarget("sys.pim.uib.email.hybridcomposer");
	request.setAction("bb.action.OPEN, bb.action.SENDEMAIL");
	QString str("mailto:%1?subject=%2&body=%3");
	str = str.arg("none.lemming@gmail.com").arg(tr("feedback :")+appInfo.title()+" V" +appInfo.version())
			.arg(hardInfo.modelName()+", "+hardInfo.modelNumber()+", "+platInfo.osVersion());
	request.setUri(str);
	qDebug()<<str;


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
	request.setUri("appworld://content/lemming-PomodoroTime");
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
