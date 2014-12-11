import bb.cascades 1.2

Container {
    //将要处理的控件放入MotionHandler内
    //tonchevent传递方面有点问题  
    
    signal click();
    signal slideStarted(int x, int y);
    signal slideLeftRight(int offset);
    signal slideTopBottom(int offset);
    
    property int moveCount : 0
    property int judgeCount : 3
    property int downx : 0
    property int downy : 0
    property int slideType : 1 //0 none; 1 LeftRight; 2 TopBottom
    onTouch: {
        if (event.isMove())
        {
            //console.debug("move");
            moveCount++;
            if (moveCount > judgeCount)
            {
                if (slideType == 1)
                    slideLeftRight(event.windowX - downx);
                else if (slideType == 2)
                    slideTopBottom(event.windowY - downy);
            }
            else if (moveCount == judgeCount)
            {
                if (Math.abs(event.windowX - downx) > Math.abs(event.windowY - downy))
                    slideType = 1;
                else 
                    slideType = 2;
                slideStarted(downx, downy);
            }
        }
        else if (event.isDown())
        {
            //console.debug("down");
            downx = event.windowX;
            downy = event.windowY;
        }
        else if (event.isCancel() || event.isUp())
        // cancel 和  up 的区别在于,如果控件本身有某个方向的交互,比如listview的滚动,则为cancel
        {
            if (moveCount <= judgeCount)
                click();
            moveCount = 0;
            slideType = 0;
            /*if (event.isCancel())
            	console.debug("cancel");
            else 
            	console.debug("up");*/
        }
    }
}
