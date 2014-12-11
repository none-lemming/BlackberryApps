import bb.cascades 1.2
import bb.cascades.pickers 1.0
import bb.system 1.2

Page {
    
    titleBar: TitleBar {
        title: qsTr("Settings") + Retranslate.onLocaleOrLanguageChanged

    }
    Container {
        leftPadding: 25.0
        rightPadding: 25.0
        topPadding: 30.0
        SliderSetter {
            title: qsTr("Pomodoro Duration") + Retranslate.onLocaleOrLanguageChanged
            fromValue: 10
            toValue: 60
            value: _setting.map.WorkTime / 60
            unit: qsTr("min") + Retranslate.onLocaleOrLanguageChanged
            onSilderValueChanged: {
                console.debug("workTime" + value);
                _setting.set("WorkTime", value*60);
            }
        }
        
        SliderSetter {
            title: qsTr("Short Break Duration") + Retranslate.onLocaleOrLanguageChanged
            fromValue: 3
            toValue: 20
            value: _setting.map.RestTime / 60
            unit: qsTr("min") + Retranslate.onLocaleOrLanguageChanged
            onSilderValueChanged: {
                console.debug("restTime" + value);
                _setting.set("RestTime", value*60);
            }
        }
        
        SliderSetter {
            title: qsTr("Longer Break Duration") + Retranslate.onLocaleOrLanguageChanged
            fromValue: 5
            toValue: 40
            value: _setting.map.LongRestTime / 60
            unit: qsTr("min");
            onSilderValueChanged: {
                console.debug("LongRestTime" + value);
                _setting.set("LongRestTime", value*60);
            }
        }
        
        Divider {
            topMargin: 30.0
            bottomMargin: 30.0

        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }
            topMargin: 10.0
            Label {
                text: qsTr("Vibration Reminder") + Retranslate.onLocaleOrLanguageChanged
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2.0

                }

            }
            ToggleButton {
                checked: _setting.map.VibrationNeeded
                onCheckedChanged: {
                    _setting.set("VibrationNeeded",checked);
                }
                horizontalAlignment: HorizontalAlignment.Right
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0

                }

            }
        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            
            }
            topMargin: 10.0
            Label {
                text: qsTr("Led Reminder") + Retranslate.onLocaleOrLanguageChanged
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2.0

                }

            }
            ToggleButton {
                checked: _setting.map.LedNeeded
                onCheckedChanged: {
                    _setting.set("LedNeeded",checked);
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0

                }
                horizontalAlignment: HorizontalAlignment.Right

            }
        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            
            }
            topMargin: 10.0
            Label {
                text: qsTr("Keep Screen On") + Retranslate.onLocaleOrLanguageChanged
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2.0

                }

            }
            ToggleButton {
                checked: _setting.map.ScreenUnlockNeeded
                onCheckedChanged: {
                    _setting.set("ScreenUnlockNeeded",checked);
                    Application.mainWindow.screenIdleMode = checked ? 1 : 0;
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0

                }
                horizontalAlignment: HorizontalAlignment.Right

            }
        }
        
        Divider {
            topMargin: 30.0
            bottomMargin: 30.0

        }
        Label {
            text: qsTr("Image Directory (show on resting)") + Retranslate.onLocaleOrLanguageChanged

        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }
            topMargin: 10.0
            TextField {
                text: _setting.map.RestImageDir
                enabled: false
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2.0

                }
                hintText: qsTr("select a folder") + Retranslate.onLocaleOrLanguageChanged
            }
            
            Button {
                text: qsTr("Change") + Retranslate.onLocaleOrLanguageChanged
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0

                }
                horizontalAlignment: HorizontalAlignment.Right
                onClicked: {
                    filePicker.open();
                }
                attachedObjects: [
                    FilePicker {
                        id: filePicker
                        title: qsTr("select a folder") + Retranslate.onLocaleOrLanguageChanged
                        onFileSelected: {
                            console.debug(selectedFiles[0]);
                            _setting.set("RestImageDir", selectedFiles[0]);
                            toast.body = qsTr("Path Changed") + Retranslate.onLocaleOrLanguageChanged;
                        }
                        viewMode: FilePickerViewMode.ListView
                        mode: FilePickerMode.SaverMultiple
                    }
                ]
            }
        }
        
       Divider {
            topMargin: 30.0

        }
        
    }
    
    
    
    attachedObjects: [
        SystemToast {
            id: toast
        }
    ]
}
