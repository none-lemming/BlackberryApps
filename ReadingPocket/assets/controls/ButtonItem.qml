import bb.cascades 1.2

Container {
    property string title
    property string description
    property string buttonText
    property bool active   : true 
    
    signal trigger();
    
    Container {                    
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        leftPadding: 30.0
        rightPadding: 30.0
        topPadding: 20.0
        bottomPadding: 20.0
        Container {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 3.0                        
            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Left
            Label {
                text: title
                textStyle.fontSize: FontSize.Large
            }
            Label {
                text: description
                textStyle.fontSize: FontSize.XSmall
                multiline: true
                visible: text.length!=0
            }
        }
        Button {
            enabled: active
            text: buttonText
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.5
            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right
            onClicked: {
                trigger();
            }
        }
    }
}
