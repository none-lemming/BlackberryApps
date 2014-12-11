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
#include <bb/cascades/SceneCover>
#include <bb/cascades/LocaleHandler>


using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
	:	QObject(app)
	,	m_controller(new ClipController(this))
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

    qml->setContextProperty("_controller", m_controller);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);

    //create app cover
    QmlDocument *qmlCover = QmlDocument::create("asset:///cover.qml").parent(this);
	// Create the QML Container from using the QMLDocument.
	m_coverContainer = qmlCover->createRootObject<Container>();
	// Create a SceneCover and set the application cover
	SceneCover *sceneCover = SceneCover::create().content(m_coverContainer);
	Application::instance()->setCover(sceneCover);

	QObject::connect(app,SIGNAL(thumbnail()), this, SLOT(updateCover()));
	QObject::connect(app,SIGNAL(aboutToQuit()),this,SLOT(saveUserData()));
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("superClipboard_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::updateCover()
{
	qDebug()<<"ApplicationUI updateCover";
	QString str = m_controller->copy();
	m_coverContainer->setProperty("contentText", str);
	qDebug()<<str;

}

void ApplicationUI::saveUserData()
{
	qDebug()<<"save user data";
	m_controller->saveClipData();
	//有三秒时间做清理工作,需要调用quit()来退出
	//Application::instance()->quit();
}
