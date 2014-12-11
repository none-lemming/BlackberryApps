import bb.cascades 1.2
import bb.cascades.pickers 1.0
import bb.system 1.2
import Qt.QTimer 1.0

Page {
    Container {
        layout: DockLayout {
            
        }
        bottomPadding: 50.0
        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            Container {
                id: buttons
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                Button {
                    id: importButton
                    text: qsTr("Import Songs")
                    onClicked: {
                        promptDialog.show();
                    }
                }
                Button {
                    id: clearButton
                    text: qsTr("Clear Data")
                    onClicked: {
                        warningDialog.show();
                    }
                }
            }
        }
        
        Label {
            id: importingLabel
            text: qsTr("Tip : \r\nImport songs will take some time\r\nPlease wait a moment")
            multiline: true
            textStyle.fontStyle: FontStyle.Italic
            textStyle.fontSize: FontSize.Small
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        ActivityIndicator {
            id: indicator
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredWidth: 100
            onRunningChanged: {
                console.debug("onRunningChanged:" + running);
            }
        }
    }
    
    attachedObjects: [
        FilePicker {
            id: filePicker
            type: FileType.Music
            onFileSelected: {
                var filename = selectedFiles[0];
                var dirPath = filename.substring(0,filename.lastIndexOf('/'));
                
                var num = _player.importSongs(dirPath);
                
                prompt(num + qsTr(" songs had been imported"));
            }
        },
        SystemDialog {
	        id: warningDialog
	        title: qsTr("Friendly Warning") + Retranslate.onLocaleOrLanguageChanged
            body: qsTr("This action will clear data in app's database. Your songs, album covers and lyrics won't be clear.\r\nOK to continue?") + Retranslate.onLocaleOrLanguageChanged
            onFinished: {
		        if (warningDialog.result == SystemUiResult.ConfirmButtonSelection)
		        {
		            _player.clearData();
                    prompt(qsTr("Data has been cleaned"));
		        	console.debug("clear");
		        }
            }
        },
        SystemDialog {
            id: promptDialog
            title: qsTr("Friendly Warning") + Retranslate.onLocaleOrLanguageChanged
            body: qsTr("Select one song from your local file system, then all songs at the same folder will be imported to the application.\r\nOnly support mp3 file now!") + Retranslate.onLocaleOrLanguageChanged
            onFinished: {
                if (promptDialog.result == SystemUiResult.ConfirmButtonSelection)
                {    
                    filePicker.open();
                    console.debug("import");
                }
            }
        },
        SystemToast {
            id: toast
        }
    ]
    
    function prompt(str) {
        toast.body = str;
        toast.show();
    }
}
