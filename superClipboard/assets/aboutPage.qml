import bb.cascades 1.2

Page {
    titleBar: TitleBar {
        title: qsTr("about")

    }
    Container {
        bottomPadding: 100.0
        ImageView {
            imageSource: "asset:///icon/Icon.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            scalingMethod: ScalingMethod.AspectFit
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }

        }
        Label {
            text: qsTr("superClipboard")
            horizontalAlignment: HorizontalAlignment.Center
            layoutProperties: StackLayoutProperties {

            }
        }
        Label {
            text: "App Version 0.5.0"
            horizontalAlignment: HorizontalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0

            }
        }
        Label {
            text: "Copyright © 2014 lemming. All Rights Reserved. "
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.fontSize: FontSize.XSmall
        }

    }
}
