/*
 * NodeDetail.h
 *
 *  Created on: 2014-4-22
 *      Author: qGG
 */

#ifndef NODEDETAIL_H_
#define NODEDETAIL_H_

#include <QObject>

class NodeDetail : public QObject
{
	Q_OBJECT

	//从map中读取键值属性
	Q_PROPERTY(QVariantMap attribute READ attribute CONSTANT);
	Q_PROPERTY(QVariantList indexPath READ indexPath CONSTANT);
	Q_PROPERTY(QString mode READ mode CONSTANT);
	//Q_ENUMS(NodeMode);

public:
	/*enum NodeMode
	{
		None,Detail,Edit,Add
	};*/
	NodeDetail(QObject* parent=0);
	virtual ~NodeDetail();

	Q_INVOKABLE void setAttribute(QString key, const QVariant& value);

	QVariantMap attribute() const;
	void setAttribute(const QVariantMap& attribute);
	QVariantList indexPath() const;
	void setIndexPath(const QVariantList& indexPath);
	QString mode() const;
	void setMode(QString mode);

private:
	QVariantMap m_attribute;
	QVariantList m_indexPath;
	QString m_mode;
};

#endif /* NODEDETAIL_H_ */
