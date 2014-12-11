/*
 * ClipController.h
 *
 *  Created on: 2014-4-22
 *      Author: qGG
 */

#ifndef CLIPCONTROLLER_H_
#define CLIPCONTROLLER_H_

#include <QObject>
#include <NodeDetail.h>
#include <ClipDataModel.h>
#include <bb/system/Clipboard>

class ClipController : public QObject
{
	Q_OBJECT

	Q_PROPERTY(ClipDataModel* dataModel READ dataModel CONSTANT);
	Q_PROPERTY(NodeDetail* currNode READ currNode CONSTANT);

public:
	ClipController(QObject* parent=0);
	virtual ~ClipController();

	Q_INVOKABLE void setHandleNode(const QVariantList& indexPath, const QString& mode);
	Q_INVOKABLE void addNode();
	Q_INVOKABLE void updateNode();
	Q_INVOKABLE void deleteNode(const QVariantList& indexPath);
	Q_INVOKABLE void paste(QString content);
	Q_INVOKABLE QString copy();
	Q_INVOKABLE void saveClipData(const QString& filename);
	void saveClipData();

	ClipDataModel* dataModel() const;
	NodeDetail* currNode() const;

private:
	ClipDataModel* m_dataModel;
	NodeDetail* m_handledNode;
	bb::system::Clipboard* m_clipboard;
};

#endif /* CLIPCONTROLLER_H_ */
