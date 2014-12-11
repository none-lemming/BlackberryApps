/*
 * ClipDataModel.h
 *
 *  Created on: 2014-4-22
 *      Author: qGG
 */

#ifndef CLIPDATAMODEL_H_
#define CLIPDATAMODEL_H_

#include <QObject>
#include <bb/cascades/DataModel>

class ClipDataModel : public bb::cascades::DataModel
{
	Q_OBJECT

	Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged);

public:

	ClipDataModel(QObject* parent=0);
	virtual ~ClipDataModel();

	Q_INVOKABLE int childCount(const QVariantList& indexPath);
	Q_INVOKABLE QVariant data(const QVariantList& indexPath);
	Q_INVOKABLE bool hasChildren(const QVariantList& indexPath);
	Q_INVOKABLE QString itemType(const QVariantList& indexPath);
	Q_INVOKABLE QVariantList last();
	Q_INVOKABLE void removeItem(const QVariantList& indexPath);
	Q_INVOKABLE void appendItem(const QVariant& item);
	Q_INVOKABLE void insertItem(const QVariantList& indexPath, const QVariant& item);
	Q_INVOKABLE void updateItem(const QVariantList& indexPath, const QVariant& item);
	Q_INVOKABLE void clearAllItem();
	Q_INVOKABLE void load(const QString& filename);
	Q_INVOKABLE void save(const QString& filename);

	signals:
	void sourceChanged();

	private:
	QString source();
	void setSource(QString source);

	QString m_source;
	QVariantList m_clipData;
};

#endif /* CLIPDATAMODEL_H_ */
