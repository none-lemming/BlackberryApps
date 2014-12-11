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

public slots:
	void feebbackByEmail();
	void openAppOnAppWorld(QString url);
	void openBrowser(QString url);
	void shareText(QString text);

private slots:
	void onInvocationFinished();
	void onCardDone(const bb::system::CardDoneMessage& message);

private:
	bb::system::InvokeManager m_invokeManager;
};

#endif /* APPINVOKER_H_ */
