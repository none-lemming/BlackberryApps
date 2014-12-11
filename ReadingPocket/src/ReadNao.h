/*
 * ReadNao.h
 *
 *  Created on: 2014年8月31日
 *      Author: qGG
 */

#ifndef READNAO_H_
#define READNAO_H_

#include <bb/cascades/Image>
#include "Nao.h"

typedef QList< QPair<QString,QString> > QDict;

class ReadNao: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString token READ token WRITE setToken);
public:
    ReadNao(QString token, QObject* parent=0);
    QString token();
    void setToken(QString token);

public slots:
    QVariant parseUrl(QString url, QVariant addition=QVariant());

signals:
    void parseUrlOk(QVariant data, QVariant addition);
    void networkError(QString error);

private slots:
    void onParseUrl();

private:
    QString m_token;
    QVariantMap m_cache;
};

#endif /* READNAO_H_ */
