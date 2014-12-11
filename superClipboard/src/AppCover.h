/*
 * AppCover.h
 *
 *  Created on: 2014-4-26
 *      Author: qGG
 */

#ifndef APPCOVER_H_
#define APPCOVER_H_

#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>

class AppCover : public bb::cascades::SceneCover
{
	Q_OBJECT
public:
	AppCover(QObject* parent=0);
	virtual ~AppCover();

	Q_SLOT void updateCover();
	void setText(const QString& text);

private:
	bb::cascades::Container* m_mainContainer;
};

#endif /* APPCOVER_H_ */
