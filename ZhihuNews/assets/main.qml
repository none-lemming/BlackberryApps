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

import bb.cascades 1.2
import bb.system 1.2

NavigationPane {
    id: qmlRoot
    
    property string theme;
    
    function openArticle(indexPath)
    {
        articleSlider.indexPath = indexPath;
        articleSheet.open();
    }
    
    function closeArticle()
    {
        articleSheet.close();
    }
    
    function promptMsg(msg) {
        toast.body = msg;
        toast.show();
    }

    Page {
        Container {
            background: Color.create("#f3f3f3")
            ItemList {
                dataModel: _app.newsModel
                cardColor: Color.create("#ffffff")
            }
        }
    }
    
    Menu.definition: MenuDefinition {
        settingsAction : SettingsActionItem{
            onTriggered: {
                pageDefinition.source = "set.qml";
                var page = pageDefinition.createObject();
                qmlRoot.push(page);
            }
        }
        actions: [
            ActionItem {
                id: aboutAction
                title: qsTr("About") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///icon/ic_info.png"
                onTriggered: {
                    pageDefinition.source = "about.qml";
                    var page = pageDefinition.createObject();
                    qmlRoot.push(page);
                }
            },
            ActionItem {
                id: feedbackAction
                imageSource: "asset:///icon/ic_feedback.png"
                title: qsTr("Feedback") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    _appInvoker.feebbackByEmail();
                }
            },
            ActionItem {
                id: commentAction
                imageSource: "asset:///icon/ic_rating_good.png"
                title: qsTr("Review") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    _appInvoker.openAppOnAppWorld("appworld://content/lemming-ZhihuNews");
                }
            }
        ]
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: pageDefinition
        },
        Sheet {
            id: articleSheet
            peekEnabled: false
            content: SlideView {
                id: articleSlider
                dataModel: _app.newsModel
            }
        },
        SystemToast {
            id: toast
        }
    ]

    onPopTransitionEnded: {
        page.destroy();
    }
    
    onCreationCompleted: {
        _zhihuNao.getNewsList(_zhihuNao.todayDate());
    }
}
