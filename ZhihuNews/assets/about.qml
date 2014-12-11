import bb.cascades 1.2

Page {
    signal close();
    titleBar: TitleBar {
        title: qsTr("About") + Retranslate.onLocaleOrLanguageChanged

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
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                ImageView {
                    imageSource: "asset:///icon/BigIcon.png"
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    scalingMethod: ScalingMethod.AspectFit                
                }
                Label {
                    text: qsTr("ZhihuNews") + Retranslate.onLocaleOrLanguageChanged
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.fontSize: FontSize.XXLarge
                }
                Label {
                    text: "App Version 1.0.0"
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.fontSize: FontSize.Small
                }
                
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    multiline: true
                    text: "本应用内所有内容权益都归知乎和文章作者所有。若有侵权行为，请及时告知，本人会及时下架应用。"
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
    }
    
    actions : [
        ActionItem {
            onTriggered: {
                _appInvoker.feebbackByEmail();
            }
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///icon/email.png"
            title: qsTr("Email")
        },
        ActionItem {
            onTriggered: {
                _appInvoker.openBrowser("http://weibo.com/u/5137043026");
            }
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///icon/weibo.png"
            title: qsTr("Weibo")
        }
    ]
}
