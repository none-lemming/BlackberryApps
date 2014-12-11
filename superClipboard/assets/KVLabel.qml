import bb.cascades 1.2

Container {
    property string key
    property string value
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight

    }
    Label {
        id: keyLabel
        text: key
        verticalAlignment: VerticalAlignment.Top
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0

        }
        multiline: true

    }
    Label {
        id: valueLabel
        text: value
        verticalAlignment: VerticalAlignment.Top
        layoutProperties: StackLayoutProperties {
            spaceQuota: 2.0

        }
        multiline: true
    }

}
