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

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include "applicationui.hpp"

#include <Qt/qdeclarativedebug.h>
#include <NodeDetail.h>
#include <ClipDataModel.h>
#include <ClipController.h>
#include <AppInvoker.h>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
	qmlRegisterType<NodeDetail>("com.QGG.NodeDetail", 1, 0, "NodeDetail");
	qmlRegisterType<ClipDataModel>("com.QGG.ClipDataModel", 1, 0, "ClipDataModel");
	qmlRegisterType<ClipController>("com.QGG.ClipController", 1, 0, "ClipController");
	qmlRegisterType<AppInvoker>("com.QGG.AppInvoker", 1, 0, "AppInvoker");

	Application app(argc, argv);

    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    new ApplicationUI(&app);
    // Enter the application main event loop.
    return Application::exec();
}
