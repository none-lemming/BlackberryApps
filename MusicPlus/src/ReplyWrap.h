/*
 * ReplyWrap.h
 *
 *  Created on: 2014-5-17
 *      Author: qGG
 */

#ifndef REPLYWRAP_H_
#define REPLYWRAP_H_

#include <QObject>
class QNetworkReply;

class ReplyWrap : public QObject
{
	Q_OBJECT
public:
	ReplyWrap(QObject* parent=0);

	void setReply(QNetworkReply* reply);
	QNetworkReply* reply();
	void setAddition(QVariant addition);
	QVariant addition();

signals:
	void finished();

protected:
	QNetworkReply* m_reply;
	QVariant m_addition;
};

#endif /* REPLYWRAP_H_ */
