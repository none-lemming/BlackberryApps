import bb.cascades 1.2

Page {
    id: root
    
    function getTime()
    {
        var d = new Date(_controller.currNode.attribute.editTime);
        console.debug(d.getFullYear() + "/" + (d.getMonth() + 1) + "/" + d.getDate() + " " + d.toLocaleTimeString());
        return d.getFullYear() + "/" + (d.getMonth() + 1) + "/" + d.getDate() + " " + d.toLocaleTimeString();
    }
    
    titleBar: TitleBar {
        title: qsTr("Details") + Retranslate.onLocaleOrLanguageChanged

    }
    
    Container {
        topPadding: 100.0
        leftPadding: 20.0
        rightMargin: 20.0
        topMargin: 50.0
        KVLabel {
            key: qsTr("content :") + Retranslate.onLocaleOrLanguageChanged
            value: _controller.currNode.attribute.content
            topMargin: 50.0
        }
        KVLabel {
            key: qsTr("last edit time :") + Retranslate.onLocaleOrLanguageChanged
            value: root.getTime()
            topMargin: 50.0
        }
        KVLabel {
            key: qsTr("count of copy :") + Retranslate.onLocaleOrLanguageChanged
            value: _controller.currNode.attribute.countOfCopy
            topMargin: 50.0
        }
        
    }
}
