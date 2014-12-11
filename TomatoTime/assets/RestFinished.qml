import bb.cascades 1.2

Container {
    preferredHeight: _display.height/3
    preferredWidth: _display.width
    layout: DockLayout {

    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        ImageTextButton {
            imageSource: "asset:///icon/ic_previous.png"
            text: qsTr("Pomodoro") + Retranslate.onLocaleOrLanguageChanged
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0

            }
            onButtonClicked: {
                console.debug("tomato");
                _device.cancelLed();
                workPane.pop();
            }

        }

        ImageTextButton {
            imageSource: "asset:///icon/ic_next.png"
            text: qsTr("Leave") + Retranslate.onLocaleOrLanguageChanged
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0

            }
            onButtonClicked: {
                console.debug("next");
                _device.cancelLed();
                rootPane.gotoStart();
            }
        }

    }
}
