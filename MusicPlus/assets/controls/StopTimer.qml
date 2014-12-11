import bb.cascades 1.2
import bb.system 1.2
import Qt.QTimer 1.0

Container {
    
    function open() {
        stopTimeSelector.title = qsTr("Sleep Time : ") + stopTimeSelector.getRelativeTime(stopTimeSelector.leftTime);
        stopTimeSelector.exec();
    }
    
    QTimer {
        id: clock
        interval: 1000 * 1
        onTimeout: {
            stopTimeSelector.leftTime = stopTimeSelector.leftTime - 1;
            console.debug("-------------left time:" + stopTimeSelector.leftTime);
            if (stopTimeSelector.leftTime == 0)
                _app.quit();
        }
    }
    
    onCreationCompleted: {
        stopTimeSelector.appendItem(qsTr("Not set"));
        stopTimeSelector.appendItem(qsTr("After 10 minutes"));
        stopTimeSelector.appendItem(qsTr("After 20 minutes"));
        stopTimeSelector.appendItem(qsTr("After 30 minutes"));
        stopTimeSelector.appendItem(qsTr("After 60 minutes"));
        stopTimeSelector.appendItem(qsTr("After 90 minutes"));
    }
    
    attachedObjects: [
        SystemListDialog {
            id: stopTimeSelector
            dismissOnSelection: true
            
            property int leftTime: -1
            onFinished: {
                if (value == SystemUiResult.ItemSelection) {
                    leftTime = getItemTime(stopTimeSelector.selectedIndices[0]);
                    clock.stop();
                    clock.start();
                }
            }
            
            function getSleepTime(pos) {
                var now = new Date();
                var h = now.getHours();
                var m = now.getMinutes();
                var lm = m + getItemTime(pos);
                var lh = lm > 60 ? h + 1 : h;
                lh = lh % 24;
                lm = m % 60;
                var lhStr = lh < 10 ? "0" + lh : "" + lh;
                var lmStr = lm < 10 ? "0" + lm : "" + lm;
                return lhStr + ":" + lmStr;
            }
            
            function getRelativeTime(time) {
                if (time < 0)
                    return qsTr("Not Set");
                else
                    return time + qsTr("min later");
            }
            
            function getItemTime(pos) {
                switch (pos) {
                    case 0:
                        return -1;
                    case 1:
                        return 10;
                    case 2:
                        return 20;
                    case 3:
                        return 30;
                    case 4:
                        return 60;
                    case 5:
                        return 90;
                    default:
                        return 90;
                }
            }
        }
    ]
}
