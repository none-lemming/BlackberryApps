import bb.cascades 1.2
import com.qgg.CircleButton 1.0

Page {
    id: askPage
    
    Container {
        
        CircleButton {
            height : 1280*2/3
            width : 768
            innerScaleH: 1
            innerScaleW: 1
            outerSourceU: "asset:///image/background.jpg"
            content: askText
        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            
            }
            ImageTextButton {
                imageSource: "asset:///icon/ic_previous.png"
                text: qsTr("Pomodoro") + Retranslate.onLocaleOrLanguageChanged
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                
                }
                onButtonClicked: {
                    console.debug("tomato");
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
                    rootPane.gotoStart();
                }
            }
        
        }
    }
    
    attachedObjects: [
        Label {
            id: askText
            text: qsTr("Yesterday\nYou said\nTomorrow.") + Retranslate.onLocaleOrLanguageChanged
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.5
            
            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.color: Color.White
            multiline: true
            textStyle.textAlign: TextAlign.Left
            textStyle.fontSize: FontSize.XLarge
            leftMargin: 30.0
            rightMargin: 30.0

        }
    ]
    
}
