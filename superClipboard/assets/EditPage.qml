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

Page {
    id: root
    
    property bool needScroll: false
    
    titleBar: TitleBar {
        title: _controller.currNode.mode + Retranslate.onLocaleOrLanguageChanged
    }
    
    Container {
        topPadding: 50.0
        leftPadding: 10.0
        rightPadding: 10.0
        bottomPadding: 50.0
        Label {
            id: countLabel
            text: qsTr("Text count : ") + Retranslate.onLocaleOrLanguageChanged + contentArea.text.length
            horizontalAlignment: HorizontalAlignment.Right

        }
        TextArea {
            id: contentArea
            text: _controller.currNode.mode == qsTr("Edit") + Retranslate.onLocaleOrLanguageChanged ? _controller.currNode.attribute.content : ""
            inputMode: TextAreaInputMode.Text
            preferredHeight: 500
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            onTextChanging: {
                countLabel.text = qsTr("Text count : ") + contentArea.text.length;
            }

        }
    }

    actions: [
        ActionItem {
            id: saveAction
            title: qsTr("save") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                console.debug("save...");
                _controller.currNode.setAttribute("content", contentArea.text);
                _controller.currNode.setAttribute("editTime", new Date().toUTCString()); //format("yyyy/MM/dd hh:mm:ss"));
                var str = new Date().toUTCString();
                console.debug(str);
                var d = new Date(str);
                console.debug(d.toJSON());
                console.debug(d.toLocaleDateString() + d.toLocaleTimeString());
                console.debug(d.toLocaleString());
                if (_controller.currNode.mode == qsTr("Edit") + Retranslate.onLocaleOrLanguageChanged)
                    _controller.updateNode();
                else {
                    _controller.currNode.setAttribute("countOfCopy", 0);
                    _controller.addNode();
                    root.needScroll = true;
                }
                navigationPane.pop();
                console.debug("save...");
            }
            imageSource: "asset:///icon/ic_done.png"

        }
    ]
    
    onCreationCompleted: {
        console.debug("edit page created");
    }
}