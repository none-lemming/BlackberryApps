import bb.cascades 1.2

Container {
    id: itemListContainer
    property variant dataModel
    property string listID
    property variant cardColor
    
    property int page_size : 15
    property int offset : 0
    property bool refresh : false
    property bool next : false
    property bool complete : false
    
    signal requireItems(variant addition, int offset);
    
    onRefreshChanged: {
        if (refresh)
        {
            offset = 0;
            complete = false;
            console.debug("goto refresh");
            getItems();
        }
    }
    
    onNextChanged: {
        if (next)
        {
            console.debug("goto next");
            getItems();
        }
    }
    
    function onGetItemsError(errorStr, addition) {
        if (addition.listID == listID)
        {
            refresh = false;
            next = false;
        }
    }
    
    function onGetItemsOk(items, addition) {
        console.debug("qml:onGetItemsOk");
        if (addition.listID == listID)
        {
            offset += page_size;
            refresh = false;
            next = false;
        }
    }
    
    function onGetItemsComplete(addition) {
        if (addition.listID == listID)
        {
            refresh = false;
            next = false;
            complete = true;
        }
    }
    
    function getItems() {
        var addition = new Object();
        addition.model = dataModel;
        addition.listID = listID;
        addition.refresh = refresh;
        requireItems(addition, offset);
    }
    
    Container {
        visible: refresh
        layout: DockLayout {
        
        }
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            ActivityIndicator {
                running: true
            }
            Label {
                text: qsTr("Loading")
                verticalAlignment: VerticalAlignment.Center
            }
        }
    }
    
    ListView {
        id: listView
        property variant backColor: cardColor
        dataModel: itemListContainer.dataModel

        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        leftPadding: 10
        rightPadding: 10
        topPadding: 15
        bottomPadding: 15

        function modifyItem(item_id, action) {
            var e = new Object();
            e.item_id = item_id;
            e.action = action;
            _pocketNao.modifyItems(e);
        }

        function openArticle(model, pos) {
            qmlRoot.openArticle(model, pos);
        }

        function encodeQString(text) {
            return _app.encodeQString(text);
        }

        listItemComponents: [
            ListItemComponent {
                type: "item"
                Container {
                    id: itemRoot
                    property variant back: ListItem.view.backColor
                    layout: AbsoluteLayout {
                    }
                    topMargin: 15
                    bottomMargin: 15

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

                    Container {
                        id: itemCard
                        background: itemRoot.back
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight

                        }

                        leftPadding: 20
                        rightPadding: 20
                        topPadding: 20
                        bottomPadding: 20
                        Container {
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 3.0
                            }
                            Label {
                                text: ListItemData.resolved_title
                                textStyle.fontSize: FontSize.Large
                                multiline: true
                                autoSize.maxLineCount: 2
                                textStyle.color: (ListItemData.status == 0) ? Color.Black : Color.create("#d2d2d2")

                            }
                            Label {
                                text: itemRoot.getHost(ListItemData.resolved_url)
                                verticalAlignment: VerticalAlignment.Bottom
                                textStyle.fontSize: FontSize.Small
                                textStyle.color: Color.create("#cccccc")
                            }
                        }

                        Container {
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 1.0
                            }
                            visible: ListItemData.hasOwnProperty("image")
                            horizontalAlignment: HorizontalAlignment.Right
                            leftMargin: 20.0
                            rightMargin: 20.0
                            preferredHeight: 150
                            ImageView {
                                image: ListItemData.hasOwnProperty("icon") ? ListItemData.icon : undefined
                                scalingMethod: ScalingMethod.AspectFit
                            }
                        }
                    }

                    ImageView {
                        imageSource: "asset:///icon/favorite.png"
                        visible: ListItemData.favorite == 1
                        preferredHeight: 50
                        preferredWidth: 50
                    }

                    contextActions: [
                        ActionSet {
                            title: ListItemData.resolved_title
                            subtitle: ListItemData.resolved_url
                            ActionItem {
                                title: (ListItemData.favorite == 1) ? qsTr("unFavorite") : qsTr("Favorite")
                                imageSource: (ListItemData.favorite == 1) ? "asset:///icon/ic_unfavorite.png" : "asset:///icon/ic_favorite.png"
                                onTriggered: {
                                    var item = itemRoot.ListItem.view.dataModel.data(itemRoot.ListItem.indexPath);
                                    if (ListItemData.favorite == 1) {
                                        itemRoot.ListItem.view.modifyItem(ListItemData.item_id, "unfavorite");
                                        item.favorite = 0;
                                    } else {
                                        itemRoot.ListItem.view.modifyItem(ListItemData.item_id, "favorite");
                                        item.favorite = 1;
                                    }
                                    itemRoot.ListItem.view.dataModel.updateItem(itemRoot.ListItem.indexPath, item);
                                }
                            }
                            ActionItem {
                                title: (ListItemData.status == 0) ? qsTr("Archive") : qsTr("unRead")
                                imageSource: (ListItemData.status == 0) ? "asset:///icon/ic_read.png" : "asset:///icon/ic_unread.png"
                                onTriggered: {
                                    var item = itemRoot.ListItem.view.dataModel.data(itemRoot.ListItem.indexPath);
                                    if (ListItemData.status == 0) {
                                        itemRoot.ListItem.view.modifyItem(ListItemData.item_id, "archive");
                                        item.status = 1;
                                    } else {
                                        itemRoot.ListItem.view.modifyItem(ListItemData.item_id, "readd");
                                        item.status = 0;
                                    }
                                    itemRoot.ListItem.view.dataModel.updateItem(itemRoot.ListItem.indexPath, item);
                                }
                            }
                            DeleteActionItem {
                                onTriggered: {
                                    itemRoot.ListItem.view.modifyItem(ListItemData.item_id, "delete");
                                    itemRoot.ListItem.view.dataModel.removeAt(itemRoot.ListItem.indexPath);
                                }
                            }
                            InvokeActionItem {
                                query {
                                    mimeType: "text/plain"
                                    invokeActionId: "bb.action.SHARE"
                                }
                                onTriggered: {
                                    var text = ListItemData.resolved_title + "\n" + ListItemData.resolved_url;
                                    data = itemRoot.ListItem.view.encodeQString(text);
                                    console.log(data);
                                }
                            }
                        }
                    ]

                    gestureHandlers: [
                        TapHandler {
                            onTapped: {
                                itemRoot.ListItem.view.openArticle(itemRoot.ListItem.view.dataModel, itemRoot.ListItem.indexPath);
                                console.debug("open article: " + ListItemData.given_title);
                            }
                        }
                    ]
                }
            }
        ]

        attachedObjects: [
            ListScrollStateHandler {
                id: listScrollState
                /*onAtBeginningChanged: {
                 * if (atBeginning && atEnd && !complete)
                 * next = true;
                 * if (atBeginning && !next && listView.isTouching)
                 * refresh = true;
                 * }
                 * onAtEndChanged: {
                 * if (atEnd && !refresh && !complete && listView.isTouching)
                 * next = true;
                 }*/
            }
        ]

        property bool refreshJudge: false
        property bool nextJudge: false
        property int downy: 0
        property int lasty: 0
        onTouch: {
            if (event.isDown()) 
            {
                if (listScrollState.atBeginning) 
                {
                    refreshJudge = true;
                    downy = event.windowY;
                    lasty = downy;
                }
                if (listScrollState.atEnd)
                {
                    nextJudge = true;
                    downy = event.windowY;
                    lasty = downy;
                }
            }
            else if (event.isUp() || event.isCancel())
            {
                if (refreshJudge && event.windowY - downy > 120 && !next) 
                {                    
                    refresh = true;
                } 
                else if (nextJudge && downy - event.windowY > 120 && ! refresh && ! complete && offset > 0) 
                {                    
                    next = true;
                }
                refreshJudge = false;
                nextJudge = false;
            }
        }

    }

    Container {
        visible: next
        layout: DockLayout {
        
        }
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            
            }
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            ActivityIndicator {
                running: true
            }
            Label {
                text: qsTr("Loading")
                verticalAlignment: VerticalAlignment.Center
            }
        }
    }

    onCreationCompleted: {
        _pocketNao.retrieveItemsError.connect(onGetItemsError);
        _pocketNao.retrieveItemsComplete.connect(onGetItemsComplete);
        _pocketNao.retrieveItemsOk.connect(onGetItemsOk);
        console.debug("list view ok");
    }
}
