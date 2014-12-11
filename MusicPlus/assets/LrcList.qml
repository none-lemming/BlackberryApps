import bb.cascades 1.2
import "controls"
Container {
    id: lrcContainer
    property int currPos : 0
    
    onCreationCompleted: {
        //歌词改变时
        _player.songChanged.connect(lrcContainer.resetLrcPos);
    }
    
    onCurrPosChanged: {
        console.debug("-------------onPosChanged");
        var indexPath = new Array();
        indexPath[0] = currPos;
        lightListItem(indexPath);
        scrollToMid(indexPath);
    }
    
    function lightListItem(indexPath)
    {
        listView.clearSelection();
        console.debug("clearSelection");
        listView.select(indexPath,true);
        console.debug("select");
    }
    
    function scrollToMid(indexPath)
    {
        indexPath[0] = indexPath[0] -4;
        listView.scrollToItem(indexPath);
    }
    
    function resetLrcPos()
    {
        if (currPos == 0)
        	currPosChanged();
        else
        	currPos = 0;
    }
    
    function calculateLrcPos(position)
    {
        var model = listView.dataModel;
        console.debug("--------------calculateLrcPos");
        for (var i=currPos+1; i<model.size(); i++)
        {
            if (model.value(i).offset > position)
            {
                currPos = i - 1;
            	return ;
            }
        }
        if (i==model.size() && model.value(i-1).offset > position)
        {
            currPos = i-1;
        }
    }
    
    signal hide();
    MotionHandler {
        layout: DockLayout {
            
        }
        ImageView {
            imageSource: _player.song.darkImagePath
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }
        
        ListView {
            id: listView
            dataModel: _player.song.lrc
            preferredWidth: _display.width
            
            listItemComponents : [
                ListItemComponent {
                    type: ""
                    Container {
                        topMargin: 30.0
                        bottomMargin: 30.0
                        preferredWidth: ListItem.view.preferredWidth
                        Label {
                            id: row
                            text: ListItemData.lrc
                            textStyle.color: Color.Gray
                            textStyle.fontSize: FontSize.Small
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                        property bool active : ListItem.selected
                        onActiveChanged: {
                            if (ListItem.selected)
                            {
                                row.textStyle.color = Color.White;
                                console.debug("--------------------selected");
                            }
                            else 
                            {
                                row.textStyle.color = Color.Gray;
                                console.debug("--------------------unselected");
                            }
                            console.debug(ListItem.indexPath);
                        }
                    }
                }
            ]
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            topPadding: 30.0
            bottomPadding: 30.0
        }
        
        onClick: {
            hide();
        }
    }
}
