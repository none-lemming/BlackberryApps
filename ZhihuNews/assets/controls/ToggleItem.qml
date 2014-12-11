import bb.cascades 1.2

Container {
    property string title
    property string description
    property bool on
    signal trigger(bool checked);
    
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
        horizontalAlignment: HorizontalAlignment.Center
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
    ToggleButton {
        checked: on
        onCheckedChanged: {
            trigger(checked);
        }
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        
        }
        horizontalAlignment: HorizontalAlignment.Right
        verticalAlignment: VerticalAlignment.Center

    }
}
