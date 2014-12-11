import bb.cascades 1.2
import Qt.QTimer 1.0
import bb.device 1.2

Container {
    property alias  restTime : clock.duration
    
    preferredHeight: _display.height
    preferredWidth: _display.width
    
    signal finished();
    
    function startPrompt()
    {
        promptAnimation.play();
    }
    
    function withZero(value)
    {
        return (value%60<10 ? "0" :"" )+ value%60;
    }
    
    layout: DockLayout {
    
    }
    ImageView {
        imageSource: _setting.getRestImage()
        scalingMethod: ScalingMethod.AspectFill
        preferredHeight: _display.height
        preferredWidth: _display.width

    }
    
    Container {
        verticalAlignment: VerticalAlignment.Bottom
        horizontalAlignment: HorizontalAlignment.Left
        Label {
            id: clockText
            verticalAlignment: VerticalAlignment.Bottom
            textStyle.color: Color.White
            textStyle.fontSize: FontSize.XLarge
            animations: [
                ParallelAnimation {
                    id: promptAnimation
                    animations: [
                        ScaleTransition {
                            fromX: 1.0
                            toX: 1.5
                            fromY: 1.0
                            toY: 1.2
                            duration: 1000
                        
                        },
                        FadeTransition {
                            fromOpacity: 1.0
                            toOpacity: 0.3
                            duration: 1000
                        
                        }
                    ]
                }
            ]
            leftMargin: 10.0
        }
        
        ProgressIndicator {
            id: progress
            fromValue: 0
            toValue: restTime
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            opacity: 0.1
            preferredWidth: _display.width
        }
    }
    attachedObjects: [
        QTimer {
            property int duration
            property int value : 0
            id: clock
            interval: 1000
            onTimeout :{
                clock.value++;
                var value = clock.value;
                clockText.text = withZero(parseInt(value/60)) + ":" + withZero(parseInt(value%60));
                progress.value = value;
                if (value >= clock.duration)
                {
                    clock.stop();
                    if (_setting.map.VibrationNeeded)
                        _device.startVibration(_setting.map.VibrationTime);
                    finished();
                }
            }
        }
    ]
    
    onCreationCompleted: {
        console.debug(restTime);
        clock.start();
        if (_setting.map.VibrationNeeded)
            _device.startVibration(_setting.map.VibrationTime);
    }
}
