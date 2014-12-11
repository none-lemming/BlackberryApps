import bb.cascades 1.2

Container {
    layout: DockLayout {

    }
    property string title
    property string copyright
    property string imageLocation
    property int width : _display.width
    
    preferredWidth: width
    preferredHeight: width*13/20
    
    ImageView {
        preferredWidth: width
        preferredHeight: width*13/20
        imageSource: "asset:///icon/article_back.png"
    }
    
    ImageView {
        preferredWidth: width
        preferredHeight: width*13/20
        imageSource: imageLocation
        scalingMethod: ScalingMethod.AspectFill
        opacity: 0.7
        loadEffect: ImageViewLoadEffect.FadeZoom
    }
    
    Container {
        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Bottom
        leftPadding: 30.0
        bottomPadding: 30.0
        rightPadding: 30.0
        Label {
            text: title
            multiline: true
            textStyle.fontSize: FontSize.Large
            textStyle.color: Color.White

        }
        Label {
            text: copyright
            textStyle.fontSize: FontSize.XSmall
            textStyle.textAlign: TextAlign.Right
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.color: Color.White
        }
    }
}
