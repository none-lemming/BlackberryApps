import bb.cascades 1.2

Container {
    property variant dataModel
    property variant indexPath
    property string debugID
    property int width : _display.width
    property int padding : 25
    
    property string  styleUrl
    property variant  title_color
    property variant  info_color
    property variant  background_color
    property int fontSize
    
    
    property bool loading : false
    
    onIndexPathChanged: {
        resetContent();
        var addition = new Object();
        var e = dataModel.data(indexPath);
        addition.itemID = e.item_id;
        //console.debug("view : "+e.resolved_title);
        var data = _readNao.parseUrl(e.resolved_url, addition);
        if (data != undefined)
            setContent(data);
        else 
            loading = true;
    }
    
    function onParseUrlOk(data, addition) {
        if (indexPath == undefined)
            return ;
        if (addition.itemID == dataModel.data(indexPath).item_id)
        {
            console.debug(debugID);
            loading = false;
            setContent(data);
        }
    }
    
    function setContent(data) {
        console.debug("---------setContent");
        contentView.html = data.content//.replace(/\\/g, "");
        if (data.author != undefined)
            fromLabel.text = fromLabel.text + " | " + data.author;
    }
    
    function resetContent() {
        contentView.html = "";
        var item = dataModel.data(indexPath);
        titleLabel.text = item.resolved_title;       
        fromLabel.text = getHost(item.resolved_url);
    }
    
    function getHost(urlStr) {
        if (urlStr==undefined)
            return "";
        var reg = "^https?://([^:/?#]*)";
        var group = urlStr.match(reg);
        if (group != null && group.length == 2)
            return group[1];
        else
            return urlStr;
    }
    
    ScrollView {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        scrollRole: ScrollRole.Main
        Container {
            topPadding: padding
            leftPadding: padding
            rightPadding: padding
            Container {
                Label {
                    id: titleLabel
                    multiline: true
                    textStyle.fontSize: FontSize.XLarge
                    textStyle.color: title_color
                }
                Divider {
                    id: divider
                }
                Label {
                    id: fromLabel
                    textStyle.fontSize: FontSize.XXSmall
                    textStyle.color: info_color
                }
            }
            WebView {
                id: contentView
                settings.cookiesEnabled: false
                settings.binaryFontDownloadingEnabled: false

                preferredWidth: width - padding * 2
                topMargin: padding * 1.5
                settings.background: background_color
                //settings.background: Color.Blue
                settings.userStyleSheetLocation: styleUrl
                settings.defaultFontSize: fontSize
                onNavigationRequested: {
                    console.log("NavigationRequested: " + request.url + " navigationType=" + request.navigationType);
                    //return ;
                    if (request.navigationType == 5) {
                        request.action = WebNavigationRequestAction.Accept;
                    } else {
                        request.action = WebNavigationRequestAction.Ignore;
                        if (request.navigationType == 0)
                            _appInvoker.openBrowser(request.url);
                    }
                }
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }
    }

    onCreationCompleted: {
        _readNao.parseUrlOk.connect(onParseUrlOk);
        console.debug("article view ok");
    }
}
