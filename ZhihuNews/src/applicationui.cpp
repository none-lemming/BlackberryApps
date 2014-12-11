/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/data/JsonDataAccess>
#include <bb/device/DisplayInfo>
#include <src/ZhihuNao.h>
#include <src/AppInvoker.h>

using namespace bb::cascades;
using namespace bb::data;

ApplicationUI::ApplicationUI() :
        QObject()
    ,   m_newsModel(new GroupDataModel(QStringList()<<"date"<<"order", this))
    ,   m_toClean(false)
{
    m_newsModel->setGrouping(ItemGrouping::ByFullValue);
    m_newsModel->setSortedAscending(false);

    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    //m_zhihu.cleanFolder("./data");
    m_zhihu.initCacheFolder();
    qml->setContextProperty("_zhihuNao", &m_zhihu);
    qml->setContextProperty("_app", this);
    qml->setContextProperty("_appInvoker", new AppInvoker(this));
    qml->setContextProperty("_setting", &m_setting);

    bb::device::DisplayInfo display;
    QDeclarativePropertyMap* displayProperties = new QDeclarativePropertyMap;
    displayProperties->insert("width", QVariant(display.pixelSize().width()));
    displayProperties->insert("height", QVariant(display.pixelSize().height()));
    qml->setContextProperty("_display", displayProperties);

    qDebug()<<connect(&m_zhihu, SIGNAL(getNewsListOk(QVariant, QString)),
            this, SLOT(onGetNewsListOk(QVariant, QString)));
    qDebug()<<connect(&m_zhihu, SIGNAL(getListImgOk(QString, QVariantList, QString)),
                this, SLOT(onGetListImgOk(QString, QVariantList, QString)));
    qDebug()<<connect(Application::instance(), SIGNAL(aboutToQuit()), this, SLOT(saveNewsList()));
    qDebug()<<connect(m_newsModel, SIGNAL(itemAdded(QVariantList)), this, SLOT(onItemAdded(QVariantList)));

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);
}

void ApplicationUI::saveNewsList()
{
    if (m_toClean)
    {
        ZhihuNao::cleanFolder("./data");
        return;
    }
    JsonDataAccess jda;
    int dateCount = m_newsModel->childCount(QVariantList());
    //遍历model,保存列表
    for (int i=0; i<dateCount; i++)
    {
        QVariantMap item = m_newsModel->data(QVariantList()<<i<<0).toMap();
        QString date = item["date"].toString();
        QVariantMap json;
        json["date"] = date;

        QVariantList itemList;
        int itemCount = m_newsModel->childCount(QVariantList()<<i);
        for (int j=0; j<itemCount; j++)
        {
            itemList.append(m_newsModel->data(QVariantList()<<i<<j));
        }
        json["stories"] = itemList;
        jda.save(json, ZhihuNao::ListCachePre+date);
        qDebug()<<date<<" "<<itemList.size();
    }
}

void ApplicationUI::onGetNewsListOk(QVariant list, QString date)
{
    QVariantList newsList = list.toList();

    //type=1的消息没有body,未做处理,移除
    for (int i=0; i<newsList.size(); i++)
    {
        if (newsList.at(i).toMap()["type"].toInt() != 0)
        {
            newsList.removeAt(i);
            i--;
        }
    }

    int dateCount = m_newsModel->childCount(QVariantList());
    //处理日期相同,数量不同的情况
    for (int i=0; i<dateCount; i++)
    {
        QVariantMap item = m_newsModel->data(QVariantList()<<i<<0).toMap();
        if (item["date"].toString() == date)
        {
            int jsonSize = newsList.size();
            int modelSize = m_newsModel->childCount(QVariantList()<<i);
            if (jsonSize > modelSize)
            {
                qDebug()<<"same date, diff count";
                QVariantList idlist;
                int firstOrder = item["order"].toInt()+jsonSize-modelSize;
                for (int j=0; j<jsonSize; j++)
                {
                    QVariantMap jsonItem = newsList.at(j).toMap();
                    QString jsonId = jsonItem["id"].toString();
                    bool had = false;
                    for (int k=0; k<modelSize; k++)
                    {
                        QVariantMap modelItem = m_newsModel->data(QVariantList()<<i<<k).toMap();
                        if (modelItem["id"].toString() == jsonId)
                        {
                            had = true;
                            break;
                        }
                    }
                    if (!had)   //新增消息,添加
                    {
                        jsonItem["date"] = date;
                        jsonItem["order"] = firstOrder;
                        firstOrder--;
                        modelSize++;//插入, 数量变多
                        m_newsModel->insert(jsonItem);
                        idlist.append(jsonItem["id"].toString());
                    }
                }
                if (m_setting.autoOffline())
                    m_zhihu.offlineDownloadNews(idlist);
            }
            else    //新增的日期,数量相同,不做处理
                qDebug()<<"same date, same count";
            return;
        }
    }

    //没有找到相同的日期,添加到model, 并离线下载
    QVariantList idlist;
    int size = newsList.size();
    for (int i=0; i<size; i++)
    {
        //添加date属性,order属性
        QVariantMap item = newsList.at(i).toMap();
        item["date"] = date;
        item["order"] = size-i;
        m_newsModel->insert(item);
        idlist.append(item["id"].toString());
    }
    if (m_setting.autoOffline())
            m_zhihu.offlineDownloadNews(idlist);
}

void ApplicationUI::onItemAdded(QVariantList indexPath)
{
    QVariantMap item = m_newsModel->data(indexPath).toMap();
    //qDebug()<<indexPath<<"__"<<item["title"].toString();
    if (!item.contains("imageLocation") && item.contains("images"))
        m_zhihu.getListImg(item["id"].toString(), item["images"].toList().at(0).toString(), indexPath);
}

void ApplicationUI::onGetListImgOk(QString imageUrl, QVariantList indexPath, QString id)
{
    QVariantMap map = m_newsModel->data(indexPath).toMap();
    if (map["id"].toString() == id)
    {
        map["imageLocation"] = imageUrl;
        m_newsModel->updateItem(indexPath, map);
    }
    else
        qDebug()<<"id error";
}

void ApplicationUI::offlineDownload()
{
    QVariantList idlist;
    for (QVariantList indexPath = m_newsModel->first(); !indexPath.isEmpty(); indexPath = m_newsModel->after(indexPath))
    {
        idlist.append(m_newsModel->data(indexPath).toMap()["id"].toString());
    }
    qDebug()<<idlist.size();
    m_zhihu.offlineDownloadNews(idlist);
}

QString ApplicationUI::transformDay(QString date)
{
    QString retStr;
    QDate inputDate = QDate::fromString(date, "yyyyMMdd");
    QDate today = QDate::currentDate();
    int offset = inputDate.daysTo(today);
    if (offset == 0)
        retStr = tr("Today");
    else if (offset == 1)
        retStr = tr("Yesterday");
    else
        retStr = inputDate.toString("yyyy.M.d");
    return retStr;
}

void ApplicationUI::cleanCache()
{
    m_toClean = true;
}
void ApplicationUI::setNewsRead(QVariantList indexPath)
{
    QVariantMap map = m_newsModel->data(indexPath).toMap();
    map["read"] = 1;
    m_newsModel->updateItem(indexPath, map);
}

QByteArray ApplicationUI::encodeQString(QString toEncode)
{
    return toEncode.toUtf8();
}

GroupDataModel* ApplicationUI::newsModel()
{
    return m_newsModel;
}
void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("ZhihuNews_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
