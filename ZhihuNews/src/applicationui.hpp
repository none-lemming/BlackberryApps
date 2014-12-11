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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Image>
#include <src/ZhihuNao.h>
#include <src/Setting.h>

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bb::cascades::GroupDataModel* newsModel READ newsModel CONSTANT);
public:
    ApplicationUI();
    virtual ~ApplicationUI() {}
    bb::cascades::GroupDataModel* newsModel();

public slots:
    QByteArray encodeQString(QString toEncode); //分享时转换
    void setNewsRead(QVariantList indexPath);   //标记已阅
    QString transformDay(QString date);         //转化日期
    void offlineDownload();                     //离线下载
    void cleanCache();                          //清除缓存

    void saveNewsList();                        //保存列表

    void onGetNewsListOk(QVariant list, QString date);
    void onGetListImgOk(QString imageUrl, QVariantList indexPath, QString id);
    void onItemAdded(QVariantList indexPath);

private slots:
    void onSystemLanguageChanged();

private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;

    bb::cascades::GroupDataModel* m_newsModel;
    Setting m_setting;
    ZhihuNao m_zhihu;
    bool m_toClean; //不保存list model
};

#endif /* ApplicationUI_HPP_ */
