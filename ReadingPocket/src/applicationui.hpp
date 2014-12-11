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
#include <bb/system/InvokeRequest>
#include <bb/cascades/GroupDataModel>
#include "PocketNao.h"
#include "ImageCache.h"

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
    Q_PROPERTY(bb::cascades::GroupDataModel* unreadModel READ unreadModel CONSTANT);
    Q_PROPERTY(bb::cascades::GroupDataModel* favoriteModel READ favoriteModel CONSTANT);
    Q_PROPERTY(bb::cascades::GroupDataModel* archiveModel READ archiveModel CONSTANT);
    Q_PROPERTY(bb::cascades::GroupDataModel* allModel READ allModel CONSTANT);
public:
    ApplicationUI();
    virtual ~ApplicationUI() {}

    bb::cascades::GroupDataModel* unreadModel();
    bb::cascades::GroupDataModel* favoriteModel();
    bb::cascades::GroupDataModel* archiveModel();
    bb::cascades::GroupDataModel* allModel();

public slots:
    QByteArray encodeQString(QString toEncode);

signals:
    void pocketAuthCallback();
    void addUrlInvoke(QString url, QString title, QString tags);
    void getItemsOk(QVariant model);

private slots:
    void onSystemLanguageChanged();
    void onInvoke(const bb::system::InvokeRequest& invoke);
    void onRetrieveItemsOk(QVariantList items, QVariant addition);
    void onRetrieveItemsComplete(QVariant addition);
    void onRetrieveImageOk(bb::cascades::Image image, QVariant addition);
    void onAddUrlOk(QVariant e);

private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    PocketNao m_pocket;
    bb::cascades::GroupDataModel* m_unreadModel;
    bb::cascades::GroupDataModel* m_archiveModel;
    bb::cascades::GroupDataModel* m_favoriteModel;
    bb::cascades::GroupDataModel* m_allModel;
    ImageCache m_imageCache;
};

#endif /* ApplicationUI_HPP_ */
