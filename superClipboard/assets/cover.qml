import bb.cascades 1.0

Container {
    property alias contentText : body.text
    layout: DockLayout {
    
    }
    background: Color.DarkCyan
    leftMargin: 10.0
    rightMargin: 10.0
    topMargin: 10.0
    bottomMargin: 10.0
    Label {
        id: body
        textStyle.color: Color.White
        textStyle.fontSize: FontSize.Large
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        multiline: true
        textFormat: TextFormat.Auto
    }
}