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
import com.qgg.CircleColorButton 1.0
import com.qgg.AppInvoker 1.0

NavigationPane {
    id: rootPane
    
    function gotoStart()
    {
        sheet.close();
    }
    
    function gotoWork()
    {
        tomatoDefinition.source = "work.qml";
        var newSheet = tomatoDefinition.createObject();
        newSheet.workTime = 10;//_setting.map.WorkTime;
        newSheet.restTime = _setting.map.RestTime;
        newSheet.longRestTime = _setting.map.LongRestTime;
        newSheet.longRestInterval = _setting.map.LongRestInterval;
        sheet.setContent(newSheet);
        sheet.open(newSheet);
    }
    
    function gotoSetting()
    {
        tomatoDefinition.source = "set.qml";
        var page = tomatoDefinition.createObject();
        rootPane.push(page);
    }
    
    function reset()
    {
        startButton.resetScale();
        startButton.resetRotationZ();
        startButton.resetOpacity();
        startButton.opacity = 0;
    }
    
    Menu.definition: MenuDefinition {
        settingsAction : SettingsActionItem{
        	onTriggered: {
            	gotoSetting();
            }
        }
        
        actions: [
            ActionItem {
                id: aboutAction
                title: qsTr("about") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///icon/ic_info.png"
                onTriggered: {
                    tomatoDefinition.source = "about.qml";
                    var page = tomatoDefinition.createObject();
                    rootPane.push(page);
                }
            },
            ActionItem {
                id: feedbackAction
                imageSource: "asset:///icon/ic_feedback.png"
                title: qsTr("feedback") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    appInvoker.feebbackByEmail();
                }
            },
            ActionItem {
                id: commentAction
                imageSource: "asset:///icon/ic_rating_good.png"
                title: qsTr("review") + Retranslate.onLocaleOrLanguageChanged
                onTriggered: {
                    appInvoker.reviewOnAppWorld();
                }
            }
        ]
    }
    Page {
        Container {
            layout: DockLayout {
            
            }
            
            ImageView {
                imageSource: "asset:///image/background.jpg"
            
            }		
            
            CircleColorButton {
                id: startButton
                width: _display.width*0.6
                height: _display.width*0.6
                colorD: Color.Green
                colorU: Color.DarkGreen
                colorO: Color.create(0,0.5,0,0.5)
                widthO: 20
                innerScaleH: 1
                innerScaleW: 1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                content: startLable
                onClicked: {
                    attractAnimation.stop();
                    startLable.resetScale();
                    startAnimation.play();
                }
                animations: [
                    ParallelAnimation {
                        id: startAnimation
                        animations: [
                            RotateTransition {
                                fromAngleZ: 0.0
                                toAngleZ: 720.0
                                duration: 1000
                                easingCurve: StockCurve.ExponentialOut   
                            },
                            ScaleTransition {
                                fromX: 1.0
                                fromY: 1.0
                                toX: 4.0
                                toY: 4.0
                                duration: 1000
                                easingCurve: StockCurve.ExponentialOut   
                            },
                            FadeTransition {
                                fromOpacity: 1.0
                                toOpacity: 0.2
                                duration: 1000
                                easingCurve: StockCurve.ExponentialOut   
                            }
                        ]
                        onEnded: {
                            gotoWork();
                        }
                    },
                    FadeTransition {
                        id: fadeIn
                        fromOpacity: 0.0
                        toOpacity: 1.0
                        onEnded: {
                            attractAnimation.play();
                        }
                        duration: 800
                    }
                ]
            }
        }
    }

    attachedObjects: [
        Sheet {
            id: sheet
            peekEnabled: false
            onOpened: {
                rootPane.reset();
            }
            onClosed: {
                fadeIn.play();
            }
        },
        ComponentDefinition {
            id: tomatoDefinition
        },
        Label {
            id: startLable
            text: qsTr("start !") + Retranslate.onLocaleOrLanguageChanged
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.fontSize: FontSize.XLarge
            animations: [
                SequentialAnimation {
                    id: attractAnimation
                    animations: [
                        ScaleTransition {
                            duration: 1500
                            easingCurve: StockCurve.ExponentialIn 
                            fromX: 1.0
                            toX: 1.5
                            fromY: 1.0
                            toY: 1.5
                        },
                        ScaleTransition {
                            duration: 1500
                            easingCurve: StockCurve.ExponentialOut 
                            fromX: 1.5
                            toX: 1.0
                            fromY: 1.5
                            toY: 1.0
                        }
                    ]
                    onEnded: {
                        attractAnimation.play();
                    }
                }
            ]
            textStyle.color: Color.White
        },
        AppInvoker {
            id: appInvoker
        }
    ]
    
    onCreationCompleted: {
        Application.mainWindow.screenIdleMode = _setting.map.ScreenUnlockNeeded ? 1 : 0;
        fadeIn.play();
    }
    
    onPopTransitionEnded: {
        page.destroy();
    }
    
}
