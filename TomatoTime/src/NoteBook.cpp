/*
 * NoteBook.cpp
 *
 *  Created on: 2014-5-14
 *      Author: qGG
 */

#include "NoteBook.h"

using namespace bb::cascades;
using namespace bb::pim::notebook;

const QString NoteBook::m_idKey = "id";
const QString NoteBook::m_priorityKey = "priority";
const QString NoteBook::m_statusKey = "status";
const QString NoteBook::m_titleKey = "title";
const QString NoteBook::m_descriptionKey = "description";
const QString NoteBook::m_dueDateTimeKey = "dueDateTime";


NoteBook::NoteBook(QObject* parent)
	:	QObject(parent)
	, 	m_notebookService(new NotebookService(this))
    , 	m_model(new GroupDataModel(QStringList()<<m_priorityKey<<m_titleKey ,this))
{
	m_model->setGrouping(ItemGrouping::None);
	loadUnCompleted();
}

NoteBook::~NoteBook()
{
}

GroupDataModel* NoteBook::model() const
{
	return m_model;
}

void NoteBook::loadUnCompleted()
{
	NotebookEntryFilter filter;
	filter.setStatus(NotebookEntryStatus::NotCompleted);
	filter.setDueDateTime(NotebookEntryDueDate::BySpecificDate, QDateTime(QDate::currentDate().addDays(1)));

	QList<NotebookEntry> entries = m_notebookService->notebookEntries(filter);
	QVariantMap map;
	for (int i=0; i<entries.size(); i++)
	{
		map[m_idKey] =  QVariant::fromValue(entries[i].id());
		map[m_priorityKey] = calPriority(entries[i].dueDateTime());
		map[m_statusKey] = entries[i].status();
		map[m_titleKey] = entries[i].title();
		map[m_descriptionKey] = entries[i].description().plainText();
		map[m_dueDateTimeKey] = entries[i].dueDateTime().date().toString();
		map["translationX"] = 0;
		//qDebug()<<map[m_statusKey];
		//qDebug()<<map;
		m_model->insert(map);
	}
}

int NoteBook::calPriority(const QDateTime& date)
{
	if (date.isNull() || !date.isValid())
		return 6;
	int diff = QDate::currentDate().daysTo(date.date());
	if (diff < 0)
		return 1;//过期
	if (diff == 0)
		return 2;//今天
	if (diff == 1)
		return 3;//明天
	if (diff <= 7)
		return 4;//本周
	if (diff <= 14)
		return 5;//下周
	return 6;//稍后
}

void NoteBook::setTranslationX(const QVariantList& indexPath ,int translationX)
{
	if (indexPath.isEmpty())
	{
		qDebug()<<"empty";
		return;
	}
	QVariantMap map = m_model->data(indexPath).toMap();
	map["translationX"] = translationX;
	m_model->updateItem(indexPath, map);
}
