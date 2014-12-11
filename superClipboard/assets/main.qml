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

import bb.cascades 1.2
import bb.cascades.pickers 1.0
import bb.system 1.2
import com.QGG.AppInvoker 1.0

NavigationPane {
    id: navigationPane
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                id: aboutAction
                title: qsTr("about")
                imageSource: "asset:///icon/ic_info.png"
                onTriggered: {
                    handlePageDefinition.source = "aboutPage.qml";
                    var page = handlePageDefinition.createObject();
                    navigationPane.push(page);
                }
            },
            ActionItem {
                id: feedbackAction
                imageSource: "asset:///icon/ic_feedback.png"
                title: qsTr("feedback")
                onTriggered: {
                    appInvoker.feebbackByEmail();
                }
            },
            ActionItem {
                id: commentAction
                imageSource: "asset:///icon/ic_rating_good.png"
                title: qsTr("review");
                onTriggered: {
                    appInvoker.reviewOnAppWorld();
                }
            }
        ]
    }
    
    Page {
        titleBar: TitleBar {
            title: qsTr("Clipboard") + Retranslate.onLocaleOrLanguageChanged
        }

        Container {
            topPadding: 50.0
            leftPadding: 10.0
            rightPadding: 10.0
            ListView {
                id: mainList
                dataModel: _controller.dataModel
                
                function pushHandlePage(indexPath,mode,source)
                {
                    _controller.setHandleNode(indexPath,mode);
                    handlePageDefinition.source = source;
                    var page = handlePageDefinition.createObject();
                    navigationPane.push(page);
                }
                
                function deleteItem(indexPath)
                {
                    _controller.deleteNode(indexPath);
                }
                
                function pasteToClipboard(indexPath)
                {
                    var data = mainList.dataModel.data(indexPath);
                    _controller.paste(data.content);
                    toast.body = qsTr("copied: ") + Retranslate.onLocaleOrLanguageChanged + (data.content.length<=15 ? data.content : (data.content.substr(0,15) + "..."));
                    toast.show();
                    data.countOfCopy++;
                    mainList.dataModel.updateItem(indexPath, data);
                }
                
                listItemComponents: [
                    ListItemComponent {
                        type: "ClipDataItem"
                        StandardListItem {
                            id: mainListItem
                            description: ListItemData.content.replace(/[\r\n]+/g,' ').trim()
                            contextActions: [
                                ActionSet {
                                    DeleteActionItem {
                                        id: deleteAction
                                        onTriggered: {
                                            mainListItem.ListItem.view.deleteItem(mainListItem.ListItem.indexPath);
                                        }
                                        title: qsTr("delete") + Retranslate.onLocaleOrLanguageChanged
                                    }
                                    ActionItem {
                                        id: editAction
                                        onTriggered: {
                                            mainListItem.ListItem.view.pushHandlePage(mainListItem.ListItem.indexPath,qsTr("Edit") + Retranslate.onLocaleOrLanguageChanged,"EditPage.qml");
                                        }
                                        title: qsTr("edit") + Retranslate.onLocaleOrLanguageChanged
                                        imageSource: "asset:///icon/ic_compose.png"
                                    }
                                    ActionItem {
                                        id: detailAction
                                        onTriggered: {
                                            mainListItem.ListItem.view.pushHandlePage(mainListItem.ListItem.indexPath,qsTr("Details") + Retranslate.onLocaleOrLanguageChanged,"DetailPage.qml");
                                        }
                                        title: qsTr("details") + Retranslate.onLocaleOrLanguageChanged
                                        imageSource: "asset:///icon/ic_document_info.png"

                                    }
                                }
                            ]
                            gestureHandlers: [
                                DoubleTapHandler {
                                    onDoubleTapped: {
                                        console.debug("double tap");
                                        var mainList = mainListItem.ListItem.view;
                                        mainList.clearSelection();
                                        mainList.toggleSelection(mainListItem.ListItem.indexPath);
                                        mainListItem.ListItem.view.pushHandlePage(mainListItem.ListItem.indexPath,qsTr("Edit") + Retranslate.onLocaleOrLanguageChanged,"EditPage.qml");
                                    }
                                },
                                TapHandler {
                                    onTapped: {
                                        console.debug("single tap");
                                        var mainList = mainListItem.ListItem.view;
                                        if (mainList.multiSelectHandler.active != true)
                                        {
                                            mainList.clearSelection();
                                            mainList.toggleSelection(mainListItem.ListItem.indexPath);
                                            mainList.pasteToClipboard(mainListItem.ListItem.indexPath);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                ]
                accessibility.labelledBy: [ mainList ]
                multiSelectAction: MultiSelectActionItem {}
                multiSelectHandler{
                    actions: [
                        DeleteActionItem {
                            onTriggered: {
                                //mainList.multiSelectHandler.
                               var arr = mainList.selectionList();
                               console.debug(arr);
                               //记得从后往前删除
                               for (var i=arr.length-1; i>=0; i--)
                               		mainList.deleteItem(arr[i]);
                            }
                        }
                    ]
                    onActiveChanged: {
                        if (active == true) {
                            console.log("Multiple selection is activated");
                        }
                        else {
                            console.log("Multiple selection is deactivated");
                        }
                    }
                    
                    
                }
                
                onSelectionChanged: {
                    if (selectionList().length > 1) {
                        multiSelectHandler.status = selectionList().length +
                        qsTr(" items selected");
                    } else if (selectionList().length == 1) {
                        multiSelectHandler.status = qsTr("1 item selected");
                    } else {
                        multiSelectHandler.status = qsTr("None selected");
                    }
                }
               /* onTriggered: {
                    mainList.clearSelection();
                    toggleSelection(indexPath);
                    var data = mainList.dataModel.data(indexPath);
                    _controller.paste(data.content);
                    toast.body = qsTr("copied: ") + (data.content.length<=10 ? data.content : (data.content.substr(0,10) + "..."));
                    toast.show();
                    data.countOfCopy++;
                    mainList.dataModel.updateItem(indexPath, data);
                }*/
            }
        }

        actions: [
            ActionItem {
                id: addAction
                title: qsTr("create") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    _controller.setHandleNode(null,qsTr("Create") + Retranslate.onLocaleOrLanguageChanged);
                    handlePageDefinition.source = "EditPage.qml";
                    var page = handlePageDefinition.createObject();
                    navigationPane.push(page);
                }
                imageSource: "asset:///icon/ic_edit.png"
            },
            ActionItem {
                id: pasteAction
                title: qsTr("add") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    _controller.currNode.setAttribute("content", _controller.copy());
                    _controller.currNode.setAttribute("editTime", new Date().toUTCString());
                    _controller.currNode.setAttribute("countOfCopy", 0);
                    _controller.addNode();
                    mainList.clearSelection();
                    mainList.toggleSelection(mainList.dataModel.last());
                    mainList.scrollToItem(mainList.dataModel.last(),ScrollAnimation.Default);
                }
                imageSource: "asset:///icon/ic_add.png"
            },
            ActionItem {
                id: importAction
                title: qsTr("import from XML") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    filePicker.open();
                }
                attachedObjects: FilePicker {
                    id: filePicker
                    title: qsTr("select a XML file") + Retranslate.onLocaleOrLanguageChanged
                    onFileSelected: {
                        console.debug(selectedFiles);
                        return ;
                        _controller.dataModel.load(selectedFiles);
                        mainList.scrollToItem(mainList.dataModel.last(),ScrollAnimation.Default);
                    }
                    type: FileType.Other
                    defaultType: FileType.Other
                    viewMode: FilePickerViewMode.ListView
                    mode: FilePickerMode.SaverMultiple
                }
                imageSource: "asset:///icon/ic_import.png"
            },
            ActionItem {
                id: exportAction
                title: qsTr("export to XML") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    fileSaver.open();
                }
                attachedObjects: FilePicker {
                    id: fileSaver
                    title: qsTr("save the XML file to") + Retranslate.onLocaleOrLanguageChanged
                    mode: FilePickerMode.Saver
                    onFileSelected: {
                        var filename = selectedFiles[0];
                        if (filename.substr(filename.lastIndexOf('.'),3).toLowerCase() != "xml")
                            filename += ".xml";
                        console.debug(filename);
                        _controller.dataModel.save(filename);
                        toast.body = qsTr("exported to: ") + Retranslate.onLocaleOrLanguageChanged + filename;
                        toast.show();
                    }
                }
                imageSource: "asset:///icon/ic_export.png"
            },
            ActionItem {
                id: clearAction
                title: qsTr("clear all") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    warningDialog.show();
                }
                attachedObjects: SystemDialog {
                    id: warningDialog
                    title: qsTr("friendly warning") + Retranslate.onLocaleOrLanguageChanged
                    body: qsTr("this action will clear all of your data, OK to continue?") + Retranslate.onLocaleOrLanguageChanged
                    onFinished: {
                        if (warningDialog.result == SystemUiResult.ConfirmButtonSelection)
                            _controller.dataModel.clearAllItem();
                    }
                }
                imageSource: "asset:///icon/ic_clear_list.png"
            }
        ]
    }

    attachedObjects: [
        ComponentDefinition {
            id: handlePageDefinition
        },
        SystemToast {
            id: toast
            position: SystemUiPosition.BottomCenter
        },
        AppInvoker {
            id: appInvoker
        }
    ]

    onPopTransitionEnded: {
        //mainList.scrollToItem(_controller.currNode.indexPath,ScrollAnimation.Default);
        if (page.needScroll)
        {
            console.debug("needScroll");
            mainList.clearSelection();
            mainList.toggleSelection(mainList.dataModel.last());
            mainList.scrollToItem(mainList.dataModel.last(),ScrollAnimation.Default);
        }
        page.destroy();
    }
    onCreationCompleted: {
        //.dataModel.source = "ClipboardData.json";
        console.debug("main page pushed");
    }
}
