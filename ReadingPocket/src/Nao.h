/*
 * Nao.h
 *
 *  Created on: 2014年8月28日
 *      Author: qGG
 */

#ifndef NAO_H_
#define NAO_H_

#include <QObject>
#include "ReplyWrap.h"

class QNetworkReply;
typedef QList< QPair<QString,QString> > QDict;

class Nao : public QObject
{
    Q_OBJECT
public:
    Nao(QObject* parent=0);

    static QByteArray processParams(const QDict& params);

    static ReplyWrap* GET(QString url, const QDict& params=QDict());
    static ReplyWrap* POST(QString url, const QDict& params);
};

#endif /* NAO_H_ */
