/*
 * ClipController.cpp
 *
 *  Created on: 2014-4-22
 *      Author: qGG
 */

#include "ClipController.h"

ClipController::ClipController(QObject* parent)
	:	QObject(parent)
	,	m_dataModel(new ClipDataModel(this))
	,	m_handledNode(new NodeDetail(this))
	,	m_clipboard(new bb::system::Clipboard(this))
{
	if (!QFile::exists(QDir::currentPath() + "/data/" + "superClipboard.xml"))
	{
		QFile::copy(QDir::currentPath() + "/app/native/assets/" + "superClipboard.xml",QDir::currentPath() + "/data/" + "superClipboard.xml");
	}
	m_dataModel->load(QDir::currentPath() + "/data/" + "superClipboard.xml");
}

ClipController::~ClipController() {
}

ClipDataModel* ClipController::dataModel() const
{
	return m_dataModel;
}

NodeDetail* ClipController::currNode() const
{
	return m_handledNode;
}

void ClipController::setHandleNode(const QVariantList& indexPath, const QString& mode)
{
	m_handledNode->setIndexPath(indexPath);
	m_handledNode->setMode(mode);
	m_handledNode->setAttribute((m_dataModel->data(indexPath)).toMap());
}

void ClipController::addNode()
{
	m_dataModel->appendItem(QVariant(m_handledNode->attribute()));
}

void ClipController::updateNode()
{
	m_dataModel->updateItem(m_handledNode->indexPath(), QVariant(m_handledNode->attribute()));
}

void ClipController::deleteNode(const QVariantList& indexPath)
{
	m_dataModel->removeItem(indexPath);
}

void ClipController::paste(QString content)
{
	m_clipboard->clear();
	m_clipboard->insert("text/plain", content.toLocal8Bit());
}

QString ClipController::copy()
{

	QByteArray data = m_clipboard->value("text/plain");
	const char* ch;
	ch = data.data();
	QString str = QString::fromLocal8Bit(ch);
	//QString str = QString(QLatin1String(ch));
	qDebug()<<str;
	if (!data.isEmpty()) {
	    return str;
	}
	return "";
}

void ClipController::saveClipData()
{
	m_dataModel->save(QDir::currentPath() + "/data/" + "superClipboard.xml");
}

void ClipController::saveClipData(const QString& filename)
{
	m_dataModel->save(filename);
}
