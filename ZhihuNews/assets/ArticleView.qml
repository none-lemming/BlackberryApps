import bb.cascades 1.2

Container {
    id: articleViewRoot
    property variant dataModel
    property variant indexPath
    
    property string  styleUrl
    property variant  title_color
    property variant  info_color
    property variant  background_color
    property int fontSize
    
    property int width : _display.width
    property int padding : 25     
    
    property bool loading : false
    
    onIndexPathChanged: {
        if (indexPath == undefined)
            return ;
        loading = true;
        cleanContent();
        initContent();        
        _zhihuNao.getNewsContent(dataModel.data(indexPath).id);
    }
    
    function onGetContentOk(data) {
        if (indexPath == undefined || data.id!=dataModel.data(indexPath).id)
            return ;
        _zhihuNao.getNewsImg(data.id, data.image);
        header.copyright = data.image_source;
        contentView.html = data.body;                
    }
    
    function onGetNewsImgOk(location, id) {
        if (indexPath == undefined || id!=dataModel.data(indexPath).id)
            return ;
        header.imageLocation = location;
    }
    
    function onGetContentError(msg) {
        loading = false;
    }
    
    function cleanContent() {
        contentView.html = "";
        header.title = "";
        header.copyright = "";
        header.imageLocation = "asset:///icon/article_back.png";
    }
    
    function initContent() {
        var item = dataModel.data(indexPath);
        header.title = item.title;
    }
    
    layout: DockLayout {
    
    }
    ScrollView {
        id: scrollView
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        scrollRole: ScrollRole.Main
        scrollViewProperties.overScrollEffectMode: OverScrollEffectMode.None
        Container {
            //background: Color.create("#343434")
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            ArticleHeader {
                id: header
            }
            Container {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                WebView {
                    id: contentView
                    settings.cookiesEnabled: false
                    settings.binaryFontDownloadingEnabled: false
                    settings.userStyleSheetLocation: "style/bright.css"//styleUrl
                    settings.defaultFontSize: fontSize
                    onNavigationRequested: {
                        //console.log("NavigationRequested: " + request.url + " navigationType=" + request.navigationType);
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
        
    }

    onCreationCompleted: {
        _zhihuNao.getNewsContentOk.connect(onGetContentOk);
        _zhihuNao.getNewsContentError.connect(onGetContentError);
        _zhihuNao.getNewsImgOk.connect(onGetNewsImgOk);
    }

}
