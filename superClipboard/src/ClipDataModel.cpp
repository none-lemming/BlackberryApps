/*
 * ClipDataModel.cpp
 *
 *  Created on: 2014-4-22
 *      Author: qGG
 */

#include "ClipDataModel.h"
#include <bb/data/XmlDataAccess>
#include <QTextStream>
#include <QFile>

ClipDataModel::ClipDataModel(QObject* parent)
	: bb::cascades::DataModel(parent)
{
}

ClipDataModel::~ClipDataModel() {
}

QString ClipDataModel::source() {
	return m_source;
}

void ClipDataModel::setSource(QString source) {
	if (m_source == source)
		return;
	m_source = source;
	m_clipData.clear();
	this->load(source);
	emit sourceChanged();
}

void ClipDataModel::load(const QString& filename)
{
//	QFile file(filename);
//	file.open(QIODevice::ReadOnly);
//	QTextStream stream(&file);
//	qDebug()<<"ClipDataModel load";
//	qDebug()<<file.exists();
//	qDebug()<<file.size();
//	qDebug()<<stream.readAll();
	bb::data::XmlDataAccess xda;
	QVariantList list = xda.load(filename, "/superClipboard/item").value<QVariantList>();
	for (int i=0; i<list.size(); i++)
	{
		appendItem(list.at(i));
	}
	//bb::cascades::DataModel::IndexMapper mapper;
	//emit itemChanged(bb::cascades::DataModelChangeType::AddRemove,);

}

void ClipDataModel::clearAllItem()
{
	m_clipData.clear();
	emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

void ClipDataModel::save(const QString& filename)
{
	QVariantMap topLevelMap;
	topLevelMap[".root"] = QVariant("superClipboard");
	topLevelMap["item"] = QVariant(m_clipData);
	bb::data::XmlDataAccess xda;
	xda.save(QVariant(topLevelMap), filename);
}

//可以在QML中调用的函数
bool ClipDataModel::hasChildren(const QVariantList &indexPath) {
	//qDebug()<<"hasChildren";
	if (indexPath.size() == 0)
		return true;
	else
		return false;
}

int ClipDataModel::childCount(const QVariantList &indexPath) {
	//qDebug()<<"childCount";
	if (indexPath.size() == 0)
		return m_clipData.size();
	else
		return 0;
}

QString ClipDataModel::itemType(const QVariantList &indexPath) {
	//qDebug()<<"itemType";
	if (indexPath.size() == 1)
		return "ClipDataItem";
	return "";
}

QVariantList ClipDataModel::last()
{
	QVariantList indexPath;
	indexPath.append(m_clipData.size()-1);
	return indexPath;
}

//get
QVariant ClipDataModel::data(const QVariantList &indexPath) {
	//qDebug()<<"data";
	if (indexPath.size() == 1)
	{
		//qDebug()<<indexPath.at(0).toInt();
		return m_clipData.at(indexPath.at(0).toInt());
	}
	else
		return QVariant();
}

//delete
void ClipDataModel::removeItem(const QVariantList& indexPath) {
	if (indexPath.size() == 1)
	{
		m_clipData.removeAt(indexPath.at(0).toInt());
		emit itemRemoved(indexPath);
	}
}

//append
void ClipDataModel::appendItem(const QVariant& item) {
	qDebug()<<"appendItem";
	qDebug()<<item;
	m_clipData.append(item);
	QVariantList indexPath;
	indexPath.append(m_clipData.size()-1);
	qDebug()<<"indexPath"<<indexPath.at(0).toInt();
	emit itemAdded(indexPath);
	//TODO 似乎有问题,传递一个局部变量
}

//insert
void ClipDataModel::insertItem(const QVariantList& indexPath, const QVariant& item) {
	if (indexPath.size() == 1)
	{
		m_clipData.insert(indexPath.at(0).toInt(), item);
		emit itemAdded(indexPath);
	}
}

//update
void ClipDataModel::updateItem(const QVariantList& indexPath, const QVariant& item) {
	//qDebug()<<"updateItem";
	//qDebug()<<item;
	if (indexPath.size() == 1)
	{
		m_clipData.replace(indexPath.at(0).toInt(), item);
		//qDebug()<<"indexPath"<<indexPath.at(0).toInt();
		emit itemUpdated(indexPath);
	}
}
