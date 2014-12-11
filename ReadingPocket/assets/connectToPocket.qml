import bb.cascades 1.2
import com.qgg.AppInvoker 1.0
import com.qgg.PocketAuth 1.0

Page {
    signal pocketConnected(string token, string userName);
    
    titleBar: TitleBar {
        title: qsTr("Login")

    }
    
    Container {
        layout: DockLayout {

        }
        ImageView {
            imageSource: "asset:///image/back_login.png"
            scalingMethod: ScalingMethod.Fill
            preferredHeight: _display.height
            preferredWidth: _display.width
        }
        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight

                }
                bottomPadding: 150.0
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                ImageView {
                    imageSource: "asset:///icon/BigIcon.png"
                    preferredHeight: 150
                    preferredWidth: 150
                }
                Label {
                    text: qsTr("ReadingPocket")
                    verticalAlignment: VerticalAlignment.Center
                    textStyle.fontSize: FontSize.XLarge
                    textStyle.color: Color.White
                }
                
            }
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight

                }
                ActivityIndicator {
                    running: !connectButton.enabled

                }
                Label {
                    id: statusLabel
                    text: qsTr("Connect to your account")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.color: Color.create("#bbbbbb")
                }
            }
            
            Divider {
                
            }
            Button {
                id: connectButton
                text: qsTr("        Pocket")
                onClicked: {
                    enabled = false;
                    auth.status = 1;
                    auth.getCode();
                }
                imageSource: "asset:///icon/pocket.png"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
        }   
    }
    
    attachedObjects: [
        AppInvoker {
            id: invoker
        },
        PocketAuth {
            id: auth     
            property int status
            
            onStatusChanged: {
                switch (status)
                {
                    case 0:
                        statusLabel.text = qsTr("Connect to your account");
                        connectButton.enabled = true;
                        break;
                    case 1:
                        statusLabel.text = qsTr("linking...");
                        break;
                    case 2:
                        statusLabel.text = qsTr("please give authorization on the web page");
                        break;
                    case 3:
                        statusLabel.text = qsTr("getting token...");
                        break;
                    case 4:
                        statusLabel.text = qsTr("all done!");
                        break;
                    case 5:
                        statusLabel.text = qsTr("something error, please retry!");
                        connectButton.enabled = true;
                        break;
                }
            }
            
            onCodeGot: {
                status = 2;
                invoker.openBrowser(auth.getAuthUrl());
            }
            
            onTokenGot: {
                status = 4;
                _setting.set("token", token);
                _setting.set("userName", userName);
                _pocketNao.setToken(token);
                pocketConnected(token, userName);
            }
            
            onAuthError: {
                status = 5;
                console.debug(error);
            }
        }
    ]
    
    function onPocketAuthCallback() {
        auth.status = 3;
        auth.getToken();
    }
    
    onCreationCompleted: {
        _app.pocketAuthCallback.connect(onPocketAuthCallback);
        console.debug("auth");
        auth.status = 0;
    }
}
