/*
 * NoteBook.h
 *
 *  Created on: 2014-5-14
 *      Author: qGG
 */

#ifndef NOTEBOOK_H_
#define NOTEBOOK_H_

#include <bb/cascades/GroupDataModel>
#include <bb/pim/notebook/NotebookEntryId>
#include <bb/pim/notebook/NotebookService>

class NoteBook  : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bb::cascades::GroupDataModel *model READ model CONSTANT);

public:
	NoteBook(QObject *parent = 0);
	virtual ~NoteBook();

	Q_INVOKABLE void setTranslationX(const QVariantList& indexPath ,int translationX);

private:
	bb::cascades::GroupDataModel* model() const;
	void loadUnCompleted();
	static int calPriority(const QDateTime& date);

	static const QString m_idKey;
	static const QString m_priorityKey;
	static const QString m_statusKey;
	static const QString m_titleKey;
	static const QString m_descriptionKey;
	static const QString m_dueDateTimeKey;

	bb::pim::notebook::NotebookService* m_notebookService;
	bb::cascades::GroupDataModel* m_model;
};

#endif /* NOTEBOOK_H_ */
