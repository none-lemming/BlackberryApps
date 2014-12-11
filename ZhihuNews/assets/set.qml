import bb.cascades 1.2
import bb.cascades.pickers 1.0
import bb.system 1.2
import "controls"

Page {
    signal close();
    
    titleBar: TitleBar {
        title: qsTr("Settings") + Retranslate.onLocaleOrLanguageChanged

    }
    
    Container {
        leftPadding: 30.0
        rightPadding: 30.0
        //topPadding: 30.0
        bottomPadding: 30.0
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                Divider {
                    
                }
                
                Header {
                    title: qsTr("My Apps")
                }
                ImageButtonItem {
                    title: qsTr("PomodoroTime")
                    imageUrl: "asset:///icon/PomodoroTime.png"
                    buttonText: qsTr("Download")
                    onTrigger: {
                        _appInvoker.openAppOnAppWorld("appworld://content/lemming-PomodoroTime");
                    }
                }
                ImageButtonItem {
                    title: qsTr("SuperClipboard")
                    imageUrl: "asset:///icon/SuperClipboard.png"
                    buttonText: qsTr("Download")
                    onTrigger: {
                        _appInvoker.openAppOnAppWorld("appworld://content/lemming-superClipboard");
                    }
                }
                Divider {
                    
                }
  /*              Header {
                    title: "Theme"

                }
                Container {
                    leftPadding: 30.0
                    rightPadding: 30.0
                    topPadding: 20.0
                    bottomPadding: 20.0
                    DropDown {
                        id: themeSelector
                        title: qsTr("Theme")
                        Option {
                            text: qsTr("Bright")
                            value: "bright"
                        }
                        Option {
                            text: qsTr("Dark")
                            value: "dark"
                        }
                        Option {
                            text: qsTr("Sepia")
                            value: "custom"
                        }
                        onSelectedValueChanged: {
                            console.debug("new value:" + selectedValue);
                        }
                        onSelectedIndexChanged: {
                            
                            console.log("SelectedIndex was changed to " + themeSelector.options[selectedIndex].value);
                        }
                    }
                    
                }
                Container {
                    leftPadding: 30.0
                    rightPadding: 30.0
                    topPadding: 20.0
                    bottomPadding: 20.0
                    DropDown {
                        title: qsTr("Font Size")
                        Option {
                            text: qsTr("Big")
                            value: 30
                        }
                        Option {
                            text: qsTr("Middle")
                            value: 30
                        }
                        Option {
                            text: qsTr("Small")
                            value: 30
                        }                    
                    }
                }  */ 
                                         
            }
        }
    }

    actions: [      
        ActionItem {
            id: commentAction
            imageSource: "asset:///icon/ic_rating_good.png"
            title: qsTr("Review") + Retranslate.onLocaleOrLanguageChanged
            onTriggered: {
                _appInvoker.openAppOnAppWorld("appworld://content/lemming-ZhihuNews");
            }
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        ActionItem {
            id: feedbackAction
            imageSource: "asset:///icon/ic_feedback.png"
            title: qsTr("Feedback") + Retranslate.onLocaleOrLanguageChanged
            onTriggered: {
                _appInvoker.feebbackByEmail();
            }
            ActionBar.placement: ActionBarPlacement.OnBar
        }
    ]
    
}
