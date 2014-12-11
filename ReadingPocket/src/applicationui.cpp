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
#include <bb/system/InvokeManager>
#include <bb/device/DisplayInfo>
#include "ReadNao.h"
#include "Setting.h"
#include "AppInvoker.h"

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
        QObject()
    ,   m_unreadModel(new GroupDataModel(QStringList()<<"time_added", this))
    ,   m_archiveModel(new GroupDataModel(QStringList()<<"time_added", this))
    ,   m_favoriteModel(new GroupDataModel(QStringList()<<"time_added", this))
    ,   m_allModel(new GroupDataModel(QStringList()<<"time_added", this))
{
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

    qml->setContextProperty("_app", this);
    qml->setContextProperty("_appInvoker", new AppInvoker(this));
    qml->setContextProperty("_pocketNao", &m_pocket);
    ReadNao *readNao = new ReadNao("e0acfc62fbbf72a78df021b69821850777d98d25", this);
    qml->setContextProperty("_readNao", readNao);
    qml->setContextProperty("_setting", new Setting(this));
    bb::device::DisplayInfo display;
    QDeclarativePropertyMap* displayProperties = new QDeclarativePropertyMap;
    displayProperties->insert("width", QVariant(display.pixelSize().width()));
    displayProperties->insert("height", QVariant(display.pixelSize().height()));
    qml->setContextProperty("_display", displayProperties);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);

    m_unreadModel->setGrouping(bb::cascades::ItemGrouping::None);
    m_unreadModel->setSortedAscending(false);
    m_archiveModel->setGrouping(bb::cascades::ItemGrouping::None);
    m_archiveModel->setSortedAscending(false);
    m_favoriteModel->setGrouping(bb::cascades::ItemGrouping::None);
    m_favoriteModel->setSortedAscending(false);
    m_allModel->setGrouping(bb::cascades::ItemGrouping::None);
    m_allModel->setSortedAscending(false);

    bb::system::InvokeManager *invokeManager = new bb::system::InvokeManager(this);
    qDebug()<<connect(invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
        this, SLOT(onInvoke(const bb::system::InvokeRequest&)));
    qDebug()<<connect(&m_pocket, SIGNAL(retrieveItemsOk(QVariantList, QVariant)),
            this, SLOT(onRetrieveItemsOk(QVariantList, QVariant)));
    qDebug()<<connect(&m_pocket, SIGNAL(retrieveItemsComplete(QVariant)),
                this, SLOT(onRetrieveItemsComplete(QVariant)));
    /*qDebug()<<connect(&m_pocket, SIGNAL(addUrlOk(QVariant)),
                this, SLOT(onAddUrlOk(QVariant)));*/
    qDebug()<<connect(&m_imageCache, SIGNAL(retrieveImageOk(bb::cascades::Image, QVariant)),
                this, SLOT(onRetrieveImageOk(bb::cascades::Image, QVariant)));


    m_pocket.setToken(Setting::get("token", "").toString());
    qDebug()<<"app ok";

    //readNao->parseUrl("http://www.baidu.com");
    //readNao->parseUrl("http://www.36kr.com/p/167962.html");
    //qDebug()<<m_imageCache.retrieve("http://farm4.staticflickr.com/3710/9312163125_81f1c1997b_o.jpg");
}

void ApplicationUI::onInvoke(const bb::system::InvokeRequest& invoke)
{
    qDebug()<<invoke.action();
    qDebug()<<invoke.metadata();
    qDebug()<<invoke.mimeType();
    qDebug()<<invoke.target();
    qDebug()<<invoke.uri();
    qDebug()<<invoke.data();
    if (invoke.mimeType() == "text/plain")
    {
        if (m_pocket.token().isEmpty())
            return;
        QByteArray buff = invoke.data();
        emit addUrlInvoke(QString::fromUtf8(buff.data(), buff.size()), "", "");
        /*return;
        QString pattern("(http|https)://[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?");
        QRegExp rx(pattern);
        QString str = QString(invoke.data());
        int pos = str.indexOf(rx);
        if (pos >= 0)
        {
            qDebug()<<rx.cap(0);
            m_pocket.addUrl(rx.cap(0));
        }
        else
            qDebug()<<"unValid";*/
    }
    else if (invoke.uri().toString() == "lemming:pocket/pocketAuthCallback")
    {
        qDebug()<<"pocketAuthCallback";
        emit pocketAuthCallback();
    }
    else
    {
        if (m_pocket.token().isEmpty())
            return;
        QVariantMap map = invoke.metadata();
        emit addUrlInvoke(invoke.uri().toString(), map["subject"].toString(), map["tags"].toString());
        //m_pocket.addUrl(invoke.uri().toString(), map["subject"].toString(), map["tags"].toString());
    }
}

void ApplicationUI::onRetrieveItemsOk(QVariantList items, QVariant addition)
{
    GroupDataModel* model = static_cast<GroupDataModel*>(addition.toMap()["model"].value<QObject*>());
    if (addition.toMap()["refresh"].toBool())
        model->clear();
    model->insertList(items);
    for (int i=0; i<items.size(); i++)
    {
        QVariantMap e = items.at(i).toMap();
        if (e.contains("image"))
        {
            QVariantMap addition;
            addition["model"] = QVariant::fromValue((QObject*)model);
            QVariantList indexPath = model->find(e);
            addition["indexPath"] = indexPath;
            addition["item_id"] = e["item_id"];
            QVariant image = m_imageCache.retrieve(e["image"].toMap()["src"].toString(), addition);
            if (!image.isNull())
            {
                e["icon"] = image;
                model->updateItem(indexPath, e);
            }
        }
    }
}

void ApplicationUI::onRetrieveItemsComplete(QVariant addition)
{
    GroupDataModel* model = static_cast<GroupDataModel*>(addition.toMap()["model"].value<QObject*>());
    if (addition.toMap()["refresh"].toBool())
        model->clear();
}

void ApplicationUI::onAddUrlOk(QVariant e)
{
    QVariantMap item = e.toMap();
    m_unreadModel->insert(item);
    m_allModel->insert(item);
    if (item.contains("image"))
    {
        QVariantMap addition;
        addition["item_id"] = item["item_id"];
        addition["model"] = QVariant::fromValue((QObject*)m_unreadModel);
        QVariantList indexPath = m_unreadModel->find(item);
        addition["indexPath"] = indexPath;
        QVariant image = m_imageCache.retrieve(item["image"].toMap()["src"].toString(), addition);
        if (!image.isNull())
        {
            item["icon"] = image;
            m_unreadModel->updateItem(indexPath, item);
        }

        addition["model"] = QVariant::fromValue((QObject*)m_allModel);
        indexPath = m_allModel->find(item);
        addition["indexPath"] = indexPath;
        if (!image.isNull())
        {
            item["icon"] = image;
            m_allModel->updateItem(indexPath, item);
        }
    }
}

void ApplicationUI::onRetrieveImageOk(Image image, QVariant addition)
{
    //qDebug()<<"onDownloadImageOk";
    //qDebug()<<m_imageCache.retrieve("http://farm4.staticflickr.com/3710/9312163125_81f1c1997b_o.jpg");
    //return;

    QVariantMap map = addition.toMap();
    //qDebug()<<map;
    GroupDataModel* model = static_cast<GroupDataModel*>(map["model"].value<QObject*>());
    QVariantList indexPath = map["indexPath"].toList();
    QString item_id = map["item_id"].toString();
    QVariantMap item = model->data(indexPath).toMap();
    if (item["item_id"].toString() == item_id)
    {
        //qDebug()<<image;
        item["icon"] = QVariant::fromValue(image);
        model->updateItem(indexPath, item);
    }
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("ReadingPocket_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

GroupDataModel* ApplicationUI::allModel()
{
    return m_allModel;
}

GroupDataModel* ApplicationUI::unreadModel()
{
    return m_unreadModel;
}

GroupDataModel* ApplicationUI::favoriteModel()
{
    return m_favoriteModel;
}

GroupDataModel* ApplicationUI::archiveModel()
{
    return m_archiveModel;
}


QByteArray ApplicationUI::encodeQString(QString toEncode)
{
    return toEncode.toUtf8();
}
