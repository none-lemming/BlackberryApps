/*
 * PocketNao.h
 *
 *  Created on: 2014年8月29日
 *      Author: qGG
 */

#ifndef POCKETNAO_H_
#define POCKETNAO_H_

#include <QObject>
#include <bb/cascades/Image>
#include "Nao.h"

typedef QList< QPair<QString,QString> > QDict;

class PocketNao: public QObject
{
    Q_OBJECT
public:
    PocketNao(QObject* parent=0);
    Q_INVOKABLE QString token();
    Q_INVOKABLE void setToken(QString token);

public slots:
    void addUrl(QString url, QString title="", QString tags="");
    void retrieveItems(QDict params, QVariant addition);
    void retrieveAllItems(QVariant addition, int offset);
    void retrieveUnreadItems(QVariant addition, int offset);
    void retrieveFavotiteItems(QVariant addition, int offset);
    void retrieveArchiveItems(QVariant addition, int offset);
    void modifyItems(QVariant actions, QVariant addition=QVariant());

signals:
    void pocketAuthRequire();
    void addUrlOk(QVariant item);
    void addUrlError(QString error);
    void retrieveItemsOk(QVariantList items, QVariant addition);
    void retrieveItemsError(QString error, QVariant addition);
    void retrieveItemsComplete(QVariant addition);

private slots:
    void onAddUrl();
    void onRetrieveItems();
    void onModifyItems();

private:
    QString m_token;
};

#endif /* POCKETNAO_H_ */
