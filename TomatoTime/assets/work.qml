import bb.cascades 1.2
import com.qgg.CircleTimer 1.0
import com.qgg.CircleColorButton 1.0
import com.qgg.RepeatedImage 1.0
import bb.device 1.2

NavigationPane {
    id: workPane
    
    property alias workTime : clock.duration
    property int restTime
    property int longRestTime
    property int longRestInterval : 4
    
    backButtonsVisible: false
    
    function reset()
    {
        clock.finished = false;
        clock.scaleX = 1;
        clock.scaleY = 1;
        clock.rotationZ =0;
        clock.opacity = 1;
        clock.reset();
        //clock.color = Color.Red;
        tomatoMsg.visible = false;
        askButton.visible = true;
    }
    
    function withZero(value)
    {
        return (value%60<10 ? "0" :"" )+ value%60;
    }
    
    Page {
        Container {
            ImageView {
                imageSource: "asset:///image/background.jpg"
            
            }
            layout: DockLayout {

            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            CircleTimer {
                id: clock
                content: clockLabel
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                width: _display.width*0.75
                height: _display.width*0.75
                duration: 5
                penWidth: 10
                color: Color.Red
                warpColor: Color.create(0.1,0.1,0.1,0.1)
                //warpWidth: 2
                innerScaleH: 1
                innerScaleW: 1
                onValueChanged: {
                    clockLabel.text = withZero(parseInt(value/60)) + ":" + withZero(parseInt(value%60));
                }
                property bool finished : false
                /*onStarted: {
                    _device.startVibration(1000);
                }*/
                onFinished: {
                    console.debug("finished");
                    tomatoMsg.visible = true;
                    tomatoMsg.count = tomatoMsg.count + 1;
                    //clock.color = Color.create(79/255, 105/255, 156/255, 1);
                    if (tomatoMsg.count%workPane.longRestInterval == 0)
                        clockLabel.text = qsTr("Longer Break") + Retranslate.onLocaleOrLanguageChanged;
                    else
                        clockLabel.text = qsTr("Tap to Break") + Retranslate.onLocaleOrLanguageChanged;
                    clockLabel.resetScale();
                    askButton.visible = false;
                    //finishMsg.visible =true;
                    finished = true;
                    finishAnimation.play();
                    if (_setting.map.VibrationNeeded)
                        _device.startVibration(_setting.map.VibrationTime);
                    if (_setting.map.LedNeeded)
                    {
                    	_device.startLed(LedColor.Blue);
                    }
                }
                onClicked: {
                    if (finished)
                    {
                        console.debug("clock clicked");
                        _device.cancelLed();
                        pageDefinition.source = "asset:///rest.qml"
                        var page = pageDefinition.createObject();
                        if (tomatoMsg.count%workPane.longRestInterval == 0)
                            page.restTime = workPane.longRestTime;
                        else
                            page.restTime = workPane.restTime;
                        workPane.push(page);
                        finishAnimation.stop();
                        //workPane.reset();
                    }
                }
                onTouch: {
                    if (finished)
                    	return ;
                    if (event.isDown())
                    {
                        clockLabel.scaleX = 1.5;
                        clockLabel.scaleY = 1.5;
                    }
                    else if (event.isUp() || event.isCancel())
                    {
                        clockLabel.resetScale();
                    }
                }
                animations: [
                	ParallelAnimation {
                        id: finishAnimation
                        animations: [
                            ScaleTransition {
                                fromX: 1.0
                                fromY: 1.0
                                toX: 2.0
                                toY: 2.0
                                duration: 3000

                            },
                            FadeTransition {
                                fromOpacity: 1.0
                                toOpacity: 0.0
                                duration: 3000
                            }
                        ]
                        onEnded: {
                            finishAnimation.play();
                        }
                    },
                    FadeTransition {
                        id: fadeIn
                        fromOpacity: 0.0
                        toOpacity: 1.0
                        onEnded: {
                            clock.start();
                            if (_setting.map.VibrationNeeded)
                                _device.startVibration(_setting.map.VibrationTime);
                        }
                        duration: 800
                    }                 
                ]
            }
            
            /*CircleColorButton {
                id: askButton
                width: 150
                height: 150
                innerScaleH: 1
                innerScaleW: 1
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center
                content: nextImage
                onClicked: {
                    clock.stop();
                    pageDefinition.source = "asset:///ask.qml"
                    var page = pageDefinition.createObject();
                    workPane.push(page);
                }
            }*/
            ImageButton {
                id: askButton
                defaultImageSource: "asset:///icon/ic_next.png"
                pressedImageSource: "asset:///icon/ic_next.png"
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    clock.stop();
                    pageDefinition.source = "asset:///ask.qml"
                    var page = pageDefinition.createObject();
                    workPane.push(page);
                }
            }
            
            RepeatedImage {
                id: tomatoMsg
                imageSource: "asset:///image/CircleTomato.png"
                imageHeight: 100
                imageWidth: 100
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center
                visible: false
                bottomMargin: 20.0

            }
        }
    }

    attachedObjects: [
        ComponentDefinition {
            id: pageDefinition
        },
        Label {
            id: clockLabel
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.fontSize: FontSize.XLarge
            textStyle.color: Color.White
            text: "00:00"
        }/*,
        ImageView {
            id: nextImage
            imageSource: "asset:///image/ic_next.png"
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }*/
    ]

    onCreationCompleted: {
        console.debug("work.qml");
        clock.start();
        if (_setting.map.VibrationNeeded)
            _device.startVibration(_setting.map.VibrationTime);
        //fadeIn.play();
    }

    onPopTransitionEnded: {
        clock.start();
        if (pageDefinition.source == "asset:///rest.qml" && _setting.map.VibrationNeeded)
        {
            _device.startVibration(_setting.map.VibrationTime);
        }
        page.destroy();
    }
    
    onPushTransitionEnded: {
        if (pageDefinition.source == "asset:///rest.qml")
        	workPane.reset();
    }
}
