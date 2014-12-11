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
import bb.multimedia 1.0
import bb.system 1.2
import Qt.QTimer 1.0
import com.qgg.AppInvoker 1.0
import "controls"

NavigationPane {
    property int width: _display.width
    property int height: _display.height
    property bool playing: false
    property bool show: false
    
    id: navigationPane
    
    function playSong() {
        if (mediaPlayer.play() == MediaError.None)
            playing = true;
    }
    
    function pauseSong() {
        if (mediaPlayer.pause() == MediaError.None)
        	playing = false;
    }
    
    function nextSong() {
        var indexPath = _player.nextSong();
        setSong(indexPath);
        listView.scrollToItem(indexPath);
    }
    
    function setSong(indexPath) {
        listView.clearSelection();
        listView.toggleSelection(indexPath);
        _player.setSong(indexPath);
    }
    
    function hideList() {
        show = false;
        listContainer.translationY = -height/3*2;
    }
    
    function showList() {
        show = true;
        listContainer.translationY = 0;
    }

    Menu.definition: MenuDefinition {
        settingsAction : SettingsActionItem{
            onTriggered: {
                pageDefinition.source = "set.qml";
                var page = pageDefinition.createObject();
                navigationPane.push(page);
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
                    navigationPane.push(page);
                }
            },
            ActionItem {
                id: feedbackAction
                imageSource: "asset:///icon/ic_feedback.png"
                title: qsTr("Feedback") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    appInvoker.feebbackByEmail();
                }
            },
            ActionItem {
                id: commentAction
                imageSource: "asset:///icon/ic_rating_good.png"
                title: qsTr("Review") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    appInvoker.reviewOnAppWorld();
                }
            }
        ]
    }

    Page {
        Container {
            layout: AbsoluteLayout {

            }
            
            Container {
                preferredHeight: height
                preferredWidth: width
                bottomMargin: 50.0
                Container {
                    id: top
                    layout: DockLayout {

                    }
                    ImageView {
                        preferredWidth: width
                        preferredHeight: width
                        imageSource: _player.song.imagePath
                        onTouch: {
                            if (event.isUp())
                            {
                            	lrcList.visible = true;
                            }
                        }
                    }
                    LrcList {
                        id: lrcList
                        visible: false
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        leftPadding: 20.0
                        rightPadding: 20.0
                        topPadding: 30.0
                        bottomPadding: 30.0
                        preferredHeight: width
                        onHide: {
                            visible = ! visible;
                        }
                    }
                }
                
                Container {
                    id: bottom
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    bottomPadding: 100.0
                    
                    Container {
                        id: littleButtons
                        layout: StackLayout {
                            orientation: LayoutOrientation.RightToLeft
                        }
                        horizontalAlignment: HorizontalAlignment.Fill
                        rightPadding: 10.0
                        leftPadding: 10.0
                        topPadding: 10.0
                        bottomPadding: 50.0
                        ImageButton {
                            defaultImageSource: "asset:///icon/ic_mode_" + _player.playMode +".png"
                            pressedImageSource: "asset:///icon/ic_mode_" + _player.playMode +".png"
                            onClicked: {
                                _player.playMode = (_player.playMode + 1) % 3
                            }
                        }
                        ImageButton {
                            defaultImageSource: stopTimeSelector.active ? "asset:///icon/ic_timer_active.png" : "asset:///icon/ic_timer.png"
                            pressedImageSource: stopTimeSelector.active ? "asset:///icon/ic_timer_active.png" : "asset:///icon/ic_timer.png"
                            onClicked: {
                                stopTimeSelector.open();
                            }
                        }     
                    }
                    
                    Container {
                        id: songInfo
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            text: _player.song.name
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                        }
                        Label {
                            text: _player.song.singer
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                    
                    Container {
                        id: buttonList  
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        preferredWidth: width
                        preferredHeight: 164
                        topPadding: 100.0
                        Container {
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 1.0
                            
                            }
                            ImageButton {                            
                                id: listButton
                                defaultImageSource: "asset:///icon/ic_list.png"
                                pressedImageSource: "asset:///icon/ic_list.png"
                                
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            if (show)
                                                hideList();
                                            else 
                                                showList();
                                        }
                                    },
                                    LongPressHandler {
                                        onLongPressed: {
                                            console.debug("LongPressHandler");
                                            console.debug(_player.song.indexPath);
                                            if (!show)
                                            	showList();
                                            listView.scrollToItem(_player.song.indexPath);
                                        }
                                    }
                                ]
                                horizontalAlignment: HorizontalAlignment.Center
                            
                            }
                        }
                        
                        Container {
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 1.0
                            
                            }
                            ImageButton {
                                id: playButton
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1.0
                                
                                }
                                defaultImageSource: playing ? "asset:///icon/ic_pause.png" : "asset:///icon/ic_play.png"
                                pressedImageSource: playing ? "asset:///icon/ic_pause.png" : "asset:///icon/ic_play.png"
                            
                                onClicked: {
                                    if (playing)
                                        pauseSong();
                                    else 
                                        playSong();
                                }
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                        }
                        
                        Container {
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 1.0
                            
                            }
                            ImageButton {
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1.0
                                
                                }
                                id: nextButton
                                defaultImageSource: "asset:///icon/ic_next.png"
                                pressedImageSource: "asset:///icon/ic_next.png"
                                gestureHandlers: [
                                    TapHandler {
                                        onTapped: {
                                            nextSong();
                                            if (playing)
                                            	playSong();
                                        }
                                    },
                                    LongPressHandler {
                                        onLongPressed: {
                                            mediaPlayer.seekTime(mediaPlayer.position + 60000);
                                        }
                                    }
                                ]
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                        }
                    }       
                }                  
            }
            
            Container {
                id: listContainer
                preferredHeight: height/3*2
                preferredWidth: width-100
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 100/2
                    positionY: 0
                }
                                
                background: Color.White
                ListView {
                    id: listView
                    dataModel: _player.model
                    
                    onTriggered: {
                        setSong(indexPath);
                        playSong();
                        
                    }
                    
                    listItemComponents : [
                        ListItemComponent {
                            type: "item"
                            StandardListItem {
                                title: ListItemData.music_name
                                //description: ListItemData.singer_name + " / " + ListItemData.file_name
                                description: ListItemData.album_name
                            }
                        }
                    ]
                }
            }
            
        }
    }

    attachedObjects: [
        // Definition of the second Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: pageDefinition
        },
        AppInvoker {
            id: appInvoker
        },
        MediaPlayer {
            id: mediaPlayer
            sourceUrl: _player.song.filePath
            onPositionChanged: {
                lrcList.calculateLrcPos(position);
            }
            onPlaybackCompleted: {
                if (_player.playMode != 2)
                	nextSong();
                playSong();
            }
        },
        QTimer {
            id: clock
            interval: 1000*60
            onTimeout :{
                stopTimeSelector.leftTime = stopTimeSelector.leftTime - 1;
                console.debug("-------------left time:" + stopTimeSelector.leftTime);
                if (stopTimeSelector.leftTime == 0)
                	_app.quit();
            }
        },
        SystemListDialog {
            id: stopTimeSelector
            dismissOnSelection: true
            
            property int leftTime : -1
            property bool active : false
            onFinished: {
                if (value == SystemUiResult.ItemSelection) {
                    leftTime = getItemTime(stopTimeSelector.selectedIndices[0]);
                    clock.stop();
                    active = false;
                    if (leftTime > 0)
                    {
                    	clock.start();
                    	active = true;
                    }
                }
            }

            function open() {
                title = qsTr("Sleep Time : ") + getRelativeTime(leftTime);
                exec();
            }
            
            function getSleepTime(pos) {
                var now = new Date();
                var h = now.getHours();
                var m = now.getMinutes();
                var lm = m + getItemTime(pos);
                var lh = lm>60 ? h+1 : h;
                lh = lh % 24;
                lm = m % 60;
                var lhStr = lh<10 ? "0"+lh : ""+lh;
                var lmStr = lm<10 ? "0"+lm : ""+lm; 
                return lhStr+":"+lmStr;              
            }
            
            function getRelativeTime(time) {
                if (time < 0)
                    return qsTr("Not Set");
                else 
                    return time + qsTr("min later");
            }
            
            function getItemTime(pos) {
                switch (pos)
                {
                    case 0:
                        return -1;
                    case 1:
                        return 10;
                    case 2:
                        return 20;
                    case 3:
                        return 30;
                    case 4:
                        return 60;
                    case 5:
                        return 90;
                    default :
                        return 90;
                }
            }
        }
    ]

    onCreationCompleted: {
        listContainer.translationY = - height / 3 * 2;
        setSong(_player.song.indexPath);
        stopTimeSelector.appendItem(qsTr("Not set"));
        stopTimeSelector.appendItem(qsTr("After 10 minutes"));
        stopTimeSelector.appendItem(qsTr("After 20 minutes"));
        stopTimeSelector.appendItem(qsTr("After 30 minutes"));
        stopTimeSelector.appendItem(qsTr("After 60 minutes"));
        stopTimeSelector.appendItem(qsTr("After 90 minutes"));
    }

    onPopTransitionEnded: {
        // Destroy the popped Page once the back transition has ended.
        page.destroy();
    }
}
