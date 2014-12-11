import bb.cascades 1.2

Page {
    titleBar: TitleBar {
        title: qsTr("About") + Retranslate.onLocaleOrLanguageChanged

    }
    Container {
        bottomPadding: 50.0
        ImageView {
            imageSource: "asset:///icon/BigIcon.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            scalingMethod: ScalingMethod.AspectFit
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2.0
            }

        }
        Label {
            text: qsTr("MusicPlus") + Retranslate.onLocaleOrLanguageChanged
            horizontalAlignment: HorizontalAlignment.Center
            layoutProperties: StackLayoutProperties {

            }
            textStyle.fontSize: FontSize.XXLarge
        }
        Label {
            text: "App Version 0.7.0"
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.fontSize: FontSize.Small
        }
        
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            topPadding: 50.0
            bottomPadding: 50.0
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                
                }
                horizontalAlignment: HorizontalAlignment.Left
                ImageView {
                    imageSource: "asset:///icon/weibo.png"
                
                }
                Label {
                    text: "@lemmingNONE"
                
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                
                }
                horizontalAlignment: HorizontalAlignment.Left
                topMargin: 10.0
                ImageView {
                    imageSource: "asset:///icon/email.png"
                }
                Label {
                    text: "none.lemming@Gmail.com"
                
                }
            }
        }
        
        Label {
            text: "Copyright © 2014 lemming. All Rights Reserved. "
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.fontSize: FontSize.XSmall
            bottomMargin: 20.0
        }

    }
}
