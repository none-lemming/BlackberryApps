/*
 * NodeDetail.cpp
 *
 *  Created on: 2014-4-22
 *      Author: qGG
 */

#include "NodeDetail.h"

NodeDetail::NodeDetail(QObject* parent)
	:	QObject(parent)
{
}

NodeDetail::~NodeDetail()
{
}

QVariantList NodeDetail::indexPath() const
{
	return m_indexPath;
}

void NodeDetail::setIndexPath(const QVariantList& indexPath)
{
	m_indexPath = indexPath;
}

QVariantMap NodeDetail::attribute() const
{
	return m_attribute;
}

void NodeDetail::setAttribute(const QVariantMap& attribute)
{
	m_attribute = attribute;
}

QString NodeDetail::mode() const
{
	return m_mode;
}

void NodeDetail::setMode(QString mode)
{
	m_mode = mode;
}

void NodeDetail::setAttribute(QString key, const QVariant& value)
{
	m_attribute[key] = value;
}
