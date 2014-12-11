import bb.cascades 1.2
import com.qgg.CircleColorButton 1.0

Container {
    property alias imageSource : image.imageSource
    property alias text : label.text
    
    signal buttonClicked;
    
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    
    CircleColorButton {
        id: button
        width: _display.width*0.2
        height: _display.width*0.2
        colorU: Color.create(0.4,0.6,0.25,1)
        colorD: Color.create(0.4,0.7,0.25,1)
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        content: image
        onClicked: {
            console.debug("imageTextButton click");
            buttonClicked();
        }
    }
    
    Label {
        id: label
        verticalAlignment: VerticalAlignment.Top
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.fontSize: FontSize.XSmall
    }
    
    attachedObjects: [
        ImageView {
            id: image
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
    ]
}
