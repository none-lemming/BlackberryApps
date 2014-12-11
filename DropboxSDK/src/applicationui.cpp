/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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
#include "DropboxClient.h"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
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

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);

    QFile* file = new QFile(QDir::currentPath() + "/shared/books/test.txt",this);
    file->open(QIODevice::ReadOnly);
    DropboxClient* box = new DropboxClient("s8d7Z4mEKlkAAAAAAAAABYyn48NJx-NzmPnT9_yaQmEgzrvBaTo-Xq_0Og6FZ0_O", "dropbox", "cn", this);
    qDebug()<<connect(box, SIGNAL(accountInfoFinished(const QVariantMap&)), this, SLOT(onAccountInfoFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(getFileFinished(const QByteArray&, const QVariantMap&, int)),
    		this, SLOT(onGetFileFinished(const QByteArray&, const QVariantMap&, int)));
    qDebug()<<connect(box, SIGNAL(putFileFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(createCopyRefFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(addCopyRefFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(fileCopyFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(fileCreateFolderFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(fileDeleteFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(fileMoveFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    qDebug()<<connect(box, SIGNAL(thumbnailFinished(const QByteArray&, const QVariantMap&, int)),
        		this, SLOT(onGetFileFinished(const QByteArray&, const QVariantMap&, int)));
    qDebug()<<connect(box, SIGNAL(mediaFinished(const QVariantMap&)), this, SLOT(onPutFileFinished(const QVariantMap&)));
    box->media("/test.flv");
    //box->thumbnail("/test.png");
    //box->fileMove("/dir/testDir", "mydir");
    //box->fileDelete("/dir/dir");
    //box->fileCreateFolder("/dir/dir");
    //box->fileCopy("/dir/test.txt", "/test.pdf");
    //box->addCopyRef("JHsCrWR3amtjeTI4bnV4eA", "/dir/test.txt");
    //box->putFile("/test.txt", file);
    //box->createCopyRef("/Dropbox.pdf");
    //box->accountInfo();
    //box->getFile("/Tomato.txt", 10);
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("DropboxSDK_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::onAccountInfoFinished(const QVariantMap& info)
{
	qDebug()<<info;
}

void ApplicationUI::onGetFileFinished(const QByteArray& buff, const QVariantMap& metadata, int id)
{
	qDebug()<<metadata;
	qDebug()<<id;
	QFile f(QDir::currentPath() + "/shared/books/image.JPEG");
	f.open(QIODevice::WriteOnly);
	f.write(buff);
}

void ApplicationUI::onPutFileFinished(const QVariantMap& metadata)
{
	qDebug()<<metadata;
}
