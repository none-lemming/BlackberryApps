import bb.cascades 1.2

Page {
    property string title;
    property string urlStr;
    property string tags;
    
    signal close();
    signal addUrl(string title, string urlStr, string tags);
    
    function extractUrl(urlStr) {
        if (urlStr==undefined ||urlStr.length==0)
            return "";
        var reg = /(http|https):\/\/[\w\-_]+(\.[\w\-_]+)+([\w\-\.,@?^=%&amp;:\/~\+#]*[\w\-\@?^=%&amp;\/~\+#])?/;
        var group = urlStr.match(reg);
        if (group!=undefined && group.length>0)
            return group[0];
        return "";
    }
    
    titleBar: TitleBar {
        title: qsTr("Add URL")

    }
    Container {
        leftPadding: 25.0
        rightPadding: 25.0
        topPadding: 25.0
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                TextField {
                    id: titleField
                    text: title
                    hintText: qsTr("title")

                }
                TextField {
                    id: urlField
                    text: urlStr
                    hintText: qsTr("URL")

                }
                TextField {
                    id: tagsField
                    text: tags
                    hintText: qsTr("tags, separate by comma")
                }
            }
        }
    }
    
    actions : [
        ActionItem {
            onTriggered: {
                close();
            }
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///icon/ic_cancel.png"
            title: qsTr("Cancel")
        },
        ActionItem {
            onTriggered: {
                //_appInvoker.openBrowser("http://getpocket.com");
                var str = extractUrl(urlField.text);
                if (str.length>0)
                {
                    addUrl(str, titleField.text, tagsField.text);
                    close();
                }
                else 
                {
                    qmlRoot.promptMsg(qsTr("Invalid URL"));
                }
            }
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///icon/ic_read.png"
            title: qsTr("OK")
        }
    ]
}
