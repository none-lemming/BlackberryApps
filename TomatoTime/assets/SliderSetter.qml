import bb.cascades 1.2

Container {
    property alias title : titleLable.text
    property alias fromValue : slider.fromValue
    property alias toValue : slider.toValue
    property alias value : slider.value
    property string unit
    
    signal silderValueChanged(int value);
    
    Label {
        id: titleLable
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight

        }
        leftPadding: 20.0
        rightPadding: 20.0
        Slider {
            id: slider
            layoutProperties: StackLayoutProperties {
                spaceQuota: 4.0

            }
            onValueChanged: {
                silderValueChanged(Math.round(value));
            }
        }
        Label {
            id: valueLabel
            text: Math.round(slider.value) + " " + unit
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0

            }
            textStyle.textAlign: TextAlign.Right
        }
    }
}
