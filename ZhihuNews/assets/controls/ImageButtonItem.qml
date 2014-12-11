import bb.cascades 1.2

Container {
    property string imageUrl
    property string title
    property string buttonText
    signal trigger();
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    leftPadding: 30.0
    rightPadding: 30.0
    topPadding: 20.0
    bottomPadding: 20.0
    ImageView {
        imageSource: imageUrl
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.5
        }
        scalingMethod: ScalingMethod.AspectFit
        verticalAlignment: VerticalAlignment.Center

    }
    Label {
        text: title
        layoutProperties: StackLayoutProperties {
            spaceQuota: 3
        }
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
    }
    Button {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.5
        }
        text: buttonText
        verticalAlignment: VerticalAlignment.Center
        onClicked: {
            trigger();
        }
        horizontalAlignment: HorizontalAlignment.Right
    }
}
