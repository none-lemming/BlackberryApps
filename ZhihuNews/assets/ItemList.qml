import bb.cascades 1.2

Container {
    id: itemListContainer
    property variant dataModel
    property variant cardColor
    
    property bool refresh : false
    property bool next : false
    
    signal requireItems(variant addition, int offset);
    
    onRefreshChanged: {
        if (refresh)
        {
            _zhihuNao.getNewsList();
        }
    }
    
    onNextChanged: {
        if (next)
        {
            var indexPath = dataModel.last();
            if (indexPath == undefined)
            {
                next = false;
                return ;
            }
            var lastDate = dataModel.data(indexPath).date;
            var beforeDate = _zhihuNao.dateToOffset(lastDate,-1);
            _zhihuNao.getNewsList(beforeDate);
        }
    }
    
    function setNoLoading() {
        refresh = false;
        next = false;
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
        leftPadding: 25
        rightPadding: 25
        topPadding: 20
        bottomPadding: 20

        function openArticle(indexPath) {
            qmlRoot.openArticle(indexPath);
        }
        
        function transformDay(str) {
            return _app.transformDay(str);
        }

        listItemComponents: [
            ListItemComponent {
                type: "header"
                Container {
                    id: headerContainer
                    background: Color.create("#ff0089b0")
                    leftPadding: 20.0
                    rightPadding: 20.0
                    topPadding: 10.0
                    bottomPadding: 10.0
                    function transformDay(str) {
                        return ListItem.view.transformDay(str);
                    }
                    Label {
                        text: headerContainer.transformDay(ListItemData)
                        textStyle.color: Color.White
                    }
                }
            },
            ListItemComponent {
                type: "item"
                Container {
                    id: itemRoot
                    property variant back: ListItem.view.backColor
                    layout: AbsoluteLayout {
                    }
                    topMargin: 20
                    bottomMargin: 20

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
                                spaceQuota: 1.0
                            }
                            visible: ListItemData.hasOwnProperty("images")
                            horizontalAlignment: HorizontalAlignment.Right
                            leftMargin: 20.0
                            rightMargin: 20.0
                            //preferredHeight: 150
                            ImageView {
                                imageSource: ListItemData.hasOwnProperty("imageLocation") ? ListItemData.imageLocation : ""
                                scalingMethod: ScalingMethod.AspectFill
                            }
                        }
                        
                        Container {
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 3.0
                            }
                            Label {
                                text: ListItemData.title
                                multiline: true
                                textStyle.color: ListItemData.hasOwnProperty("read") ? Color.create("#777777") : Color.Black 
                            }
                            Label {
                                visible: ListItemData.hasOwnProperty("theme_name") ? true : false 
                                text: ListItemData.hasOwnProperty("theme_name") ? ListItemData.theme_name : "" 
                                verticalAlignment: VerticalAlignment.Bottom
                                textStyle.fontSize: FontSize.Small
                                textStyle.color: Color.create("#b7b7b7")
                            }
                        }                        
                    }

                    gestureHandlers: [
                        TapHandler {
                            onTapped: {
                                itemRoot.ListItem.view.openArticle(itemRoot.ListItem.indexPath);
                                console.debug("open article: " + ListItemData.title);
                            }
                        }
                    ]
                }
            }
        ]

        attachedObjects: [
            ListScrollStateHandler {
                id: listScrollState
            }
        ]

        property bool refreshJudge: false
        property bool nextJudge: false
        property int downy: 0
        property int lasty: 0
        onTouch: {
            if (event.isDown()) {
                if (listScrollState.atBeginning) {
                    refreshJudge = true;
                    downy = event.windowY;
                    lasty = downy;
                }
                if (listScrollState.atEnd) {
                    nextJudge = true;
                    downy = event.windowY;
                    lasty = downy;
                }
            } else if (event.isUp() || event.isCancel()) {
                if (refreshJudge && event.windowY - downy > 120 && ! next) {                    
                    refresh = true;
                }
                else if (nextJudge && downy - event.windowY > 120 && ! refresh) 
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
        _zhihuNao.getNewsListOk.connect(setNoLoading);
        _zhihuNao.getNewsListError.connect(setNoLoading);
    }
}
