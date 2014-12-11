/*
 * AppInvoker.h
 *
 *  Created on: 2014-5-3
 *      Author: qGG
 */

#ifndef APPINVOKER_H_
#define APPINVOKER_H_

#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>

class AppInvoker : public QObject
{
	Q_OBJECT
public:
	AppInvoker(QObject* parent=0);
	virtual ~AppInvoker();

	Q_INVOKABLE void feebbackByEmail();
	Q_INVOKABLE void reviewOnAppWorld();

private slots:
	void onInvocationFinished();
	void onCardDone(const bb::system::CardDoneMessage& message);

private:
	bb::system::InvokeManager m_invokeManager;
};

#endif /* APPINVOKER_H_ */
