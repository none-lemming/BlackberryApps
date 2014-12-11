/*
 * AppCover.cpp
 *
 *  Created on: 2014-4-26
 *      Author: qGG
 */

#include "AppCover.h"
#include <bb/cascades/QmlDocument>

AppCover::AppCover(QObject* parent)
	: bb::cascades::SceneCover(this)
{
	bb::cascades::QmlDocument *qml = bb::cascades::QmlDocument::create("asset:///cover.qml").parent(this);
	m_mainContainer = qml->createRootObject<bb::cascades::Container>();
	setContent(m_mainContainer);
	//setText("hhhhhh");
}

AppCover::~AppCover() {
}

void AppCover::updateCover()
{
	qDebug()<<"update cover";
	m_mainContainer->setProperty("content", "updated!");
}

void AppCover::setText(const QString& text)
{
	m_mainContainer->setProperty("contentText", text);
}
