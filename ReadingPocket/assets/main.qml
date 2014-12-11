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
import com.qgg.AppInvoker 1.0
import bb.system 1.2


TabbedPane {
    id: qmlRoot
    showTabsOnActionBar: false
    property bool autoArchive
    
    function openArticle(dataModel, indexPath)
    {
        articleSlider.dataModel = dataModel;
        articleSlider.indexPath = indexPath;
        articleSlider.checkBothSide();//更新两边内容
        articleSheet.open();
        if (autoArchive)
            articleSlider.setArchive();
    }
    
    function closeArticle()
    {
        articleSheet.close();
    }
    
    function setAutoArchive(auto) {
        autoArchive = auto;
        articleSlider.autoArchive = auto;
    }
    
    function promptMsg(msg) {
        toast.body = msg;
        toast.show();
    }
    
    Tab {
        title: qsTr("Unread") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "asset:///icon/ic_list_unread.png"
        Page {
            Container {
                background: Color.create("#f3f3f3")
                ItemList {
                    id: allList
                    dataModel: _app.unreadModel
                    listID: "tabAll"
                    cardColor: Color.create("#ffffff")
                    onRequireItems: {
                        _pocketNao.retrieveUnreadItems(addition, offset);
                    }
                }
            }
            actions: [
                ActionItem {
                    onTriggered: {
                        authSheet.openAddPage("http://", "", "");
                    }
                    ActionBar.placement: ActionBarPlacement.OnBar
                    imageSource: "asset:///icon/ic_add.png"
                    title: qsTr("Add")
                }
            ]
        }
    }
    
    Tab {
        title: qsTr("Archive") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "asset:///icon/ic_read.png"
        Page {
            Container {
                background: Color.create("#f3f3f3")
                ItemList {
                    dataModel: _app.archiveModel
                    listID: "tabAll"
                    cardColor: Color.create("#ffffff")
                    onRequireItems: {
                        _pocketNao.retrieveArchiveItems(addition, offset);
                    }
                }
            }
        }
    }
    
    Tab {
        title: qsTr("Favorite") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "asset:///icon/ic_favorite.png"
        Page {
            Container {
                background: Color.create("#f3f3f3")
                ItemList {
                    dataModel: _app.favoriteModel
                    listID: "tabAll"
                    cardColor: Color.create("#ffffff")
                    onRequireItems: {
                        _pocketNao.retrieveFavotiteItems(addition, offset);
                    }
                }
            }
        }
    }
    
    Tab {
        title: qsTr("All") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "asset:///icon/ic_list_all.png"
        Page {
            Container {
                background: Color.create("#f3f3f3")
                ItemList {
                    dataModel: _app.allModel
                    listID: "tabAll"
                    cardColor: Color.create("#ffffff")
                    onRequireItems: {
                        _pocketNao.retrieveAllItems(addition, offset);
                    }
                }
            }
        }
    }
    
    Menu.definition: MenuDefinition {
        settingsAction : SettingsActionItem{
            onTriggered: {
                authSheet.openPage("set.qml");
            }
        }
        actions: [
            ActionItem {
                id: aboutAction
                title: qsTr("About") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///icon/ic_info.png"
                onTriggered: {
                    authSheet.openPage("about.qml");
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
                    _appInvoker.openAppOnAppWorld("appworld://content/lemming-ReadingPocket");
                }
            }
        ]
    }
       
    attachedObjects: [
        Sheet {
            id: authSheet
            peekEnabled: false
            function gotoAuth() {
                sheetDefinition.source = "connectToPocket.qml";
                var newSheet = sheetDefinition.createObject();
                newSheet.pocketConnected.connect(authSheet.onPocketConnected);
                authSheet.setContent(newSheet);
                authSheet.open(newSheet);
            }
            function onPocketConnected(token, userName) {
                console.debug(token+"+"+userName);
                authSheet.close();
                allList.refresh = true;
            }
            
            function openAddPage(urlStr, title, tags) {
                sheetDefinition.source = "add.qml";
                var newSheet = sheetDefinition.createObject();
                newSheet.close.connect(authSheet.onClose);
                newSheet.addUrl.connect(authSheet.onAddUrl);
                newSheet.title = title;
                newSheet.urlStr = urlStr;
                newSheet.tags = tags;
                authSheet.setContent(newSheet);
                authSheet.open(newSheet);
            }
            function onAddUrl(urlStr, title, tags) {
                _pocketNao.addUrl(urlStr, title, tags);
                promptMsg(qsTr("Adding..."))
            }
            
            function openPage(source) {
                sheetDefinition.source = source;
                var newSheet = sheetDefinition.createObject();
                newSheet.close.connect(authSheet.onClose);
                authSheet.setContent(newSheet);
                authSheet.open(newSheet);
            }
            function onClose() {
                authSheet.close();
            }
        },
        Sheet {
            id: articleSheet
            peekEnabled: false
            content: SlideView {
                id: articleSlider
            }
        },
        ComponentDefinition {
            id: sheetDefinition
        },
        SystemToast {
            id: toast
        }
    ]
    
    function onAddUrlOk(item) {
        promptMsg(qsTr("Url Added"));
    }
    
    function onAddUrlError(error) {
        promptMsg(error);
    }
    
    function onRetrieveItemsError(error, addition) {
        promptMsg(error);
    }
    
    function onNetworkError(error) {
        promptMsg(error);
    }

    onCreationCompleted: {
        _pocketNao.pocketAuthRequire.connect(authSheet.gotoAuth);
        _pocketNao.addUrlOk.connect(onAddUrlOk);
        _pocketNao.addUrlError.connect(onAddUrlError);
        _pocketNao.retrieveItemsError.connect(onRetrieveItemsError);
        _readNao.networkError.connect(onNetworkError);
        _app.addUrlInvoke.connect(authSheet.openAddPage);
        console.debug("mian view ok");
    }
}
