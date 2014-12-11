/*
 * PocketAuth.h
 *
 *  Created on: 2014年8月28日
 *      Author: qGG
 */

#ifndef POCKETAUTH_H_
#define POCKETAUTH_H_

#include <qobject.h>
class QNetworkReply;
typedef QList< QPair<QString,QString> > QDict;

class PocketAuth: public QObject
{
public:
    PocketAuth();
};

#endif /* POCKETAUTH_H_ */
