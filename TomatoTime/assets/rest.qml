import bb.cascades 1.2
import Qt.QTimer 1.0
import bb.device 1.2

Page {
    id: restPage
    
    property alias restTime : restingPage.restTime

    Container {
        layout: AbsoluteLayout {

        }
        
        Resting {
            id: restingPage
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0.0
                positionY: 0.0
            }
            //duration : restPage.restTime
            
            animations: [
                ParallelAnimation {
                    id: flyOut
                    animations: [
                        FadeTransition {
                            fromOpacity: 1.0
                            toOpacity: 0.0
                            duration: 3000
                        },
                        TranslateTransition {
                            fromY: 0.0
                            toY: -_display.height
                            duration: 3000
                        }
                    ]
                },
                SequentialAnimation {
                    id: flashAnimation
                    animations: [
                        FadeTransition {
                            fromOpacity: 1.0
                            toOpacity: 0.5
                            duration: 2500
                            easingCurve: StockCurve.Linear    
                        },
                        FadeTransition {
                            fromOpacity: 0.5
                            toOpacity: 1.0
                            duration: 2500
                            easingCurve: StockCurve.Linear  
                        }
                    ]
                    onEnded: {
                        flashAnimation.play();
                    }
                }
            ]
            onFinished: {
                //flyOut.play();
                //flyIn.play();
                restingPage.layoutProperties.positionY = -_display.height/3;
                finishPage.layoutProperties.positionY = _display.height*2/3;
                //restingPage.startPrompt();
                flashAnimation.play();
                if (_setting.map.LedNeeded)
                {
                    _device.startLed(LedColor.Blue);
                }
            }
            
        }
        
        RestFinished {
            id: finishPage
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0.0
                positionY: 1280.0
            }
            animations: [
                ParallelAnimation {
                    id: flyIn
                    animations: [
                        /*FadeTransition {
                            fromOpacity: 0.0
                            toOpacity: 1.0
                            duration: 3000
                        },*/
                        TranslateTransition {
                            //fromY: 1280.0
                            toY: 0.0
                            duration: 3000
                        }
                    ]
                }
            ]
        }
    }    
}
