import bb.cascades 1.2
import com.qgg.NoteBook 1.0

Page {
    id: rootPage    
    
    Container {

        Container {
            preferredHeight: 100
            background: Color.Black
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Fill
            layout: DockLayout {

            }
            Label {
                text: "just go !"
                textStyle.color: Color.White
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center

            }
        }

        Container {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            ListView {
                id: taskList
                dataModel: notebook.model

                property variant selectedIndex: null
                property double touchDownX
                property double offset: 0
                property bool overdue: true
                onTouch: {
                    if (overdue) {
                        touchDownX = event.localX;
                        overdue = ! overdue;
                        return;
                    }
                    if (event.isDown()) {
                        console.debug("down: " + touchDownX);
                    } else if (event.isMove()) {
                        notebook.setTranslationX(selectedIndex, event.localX - touchDownX);
                        console.debug("move: " + event.localX);
                    } else {
                        notebook.setTranslationX(selectedIndex, 0);
                        overdue = true;
                        selectedIndex = null;
                        console.debug("up,cancel");
                    }
                }

                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            id: itemContainer
                            preferredHeight: 150
                            verticalAlignment: VerticalAlignment.Fill
                            horizontalAlignment: HorizontalAlignment.Fill
                            layout: AbsoluteLayout {

                            }
                            onTouch: {
                                if (event.isDown())
                                    ListItem.view.selectedIndex = ListItem.indexPath;
                            }

                            Container {
                                id: underLayer
                                verticalAlignment: VerticalAlignment.Center
                                preferredHeight: 150
                                layoutProperties: AbsoluteLayoutProperties {
                                    positionX: 0.0

                                }
                                layout: DockLayout {

                                }
                                Label {
                                    verticalAlignment: VerticalAlignment.Center
                                    text: ListItemData.description
                                }
                            }

                            Container {
                                id: topLayer
                                translationX: ListItemData.translationX
                                opacity: 1 - Math.abs(translationX) / 768
                                background: itemContainer.getPriorityColorU(ListItemData.priority)
                                layoutProperties: AbsoluteLayoutProperties {
                                    positionX: 0.0

                                }
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Fill
                                preferredHeight: 150
                                layout: DockLayout {

                                }

                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    
                                    }
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    ImageView {
                                        id: checkeImage
                                        property bool checked: false
                                        imageSource: checked ? "asset:///icon/ic_checked.png" : "asset:///icon/ic_unchecked.png"
                                        scalingMethod: ScalingMethod.AspectFit
                                        //verticalAlignment: VerticalAlignment.Center
                                        preferredHeight: 150/3*2
                                        preferredWidth: 150/3*2
                                        onTouch: {
                                            if (event.isUp()) {
                                                checked = ! checked;
                                            }
                                        }
                                    }
                                    
                                    Label {
                                        text: ListItemData.title
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        textStyle.color: Color.White
                                        textStyle.fontSize: FontSize.XLarge
                                        layoutProperties: StackLayoutProperties {
                                            spaceQuota: 3.0
                                        
                                        }
                                    }
                                    
                                    ImageButton {
                                        preferredHeight: 150/2
                                        preferredWidth: 150/2
                                        defaultImageSource: "asset:///icon/ic_go.png"
                                        pressedImageSource: "asset:///icon/ic_go.png"
                                        horizontalAlignment: HorizontalAlignment.Right
                                    }
                                }                                
                            }

                            function getPriorityColorU(priority) {
                                switch (priority) {
                                    case 1:
                                        return Color.create("#ef532d");
                                    case 2:
                                        return Color.create("#253557");
                                    case 3:
                                        return Color.create("#6fbcc4");
                                    case 4:
                                        return Color.DarkBlue
                                    case 5:
                                        return Color.Blue
                                    case 6:
                                        return Color.Green
                                    default:
                                        return Color.Black
                                }
                            }

                        }
                    }
                ]
            }
        }
    }

    attachedObjects: [
        NoteBook {
            id: notebook
        }
    ]
}
