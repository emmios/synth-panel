//contents of Control.qml
import QtQuick 2.9
import QtGraphicalEffects 1.0

Rectangle {
    id: control
    width: 100
    height: 4
    radius: height
    color: "transparent"

    property alias bg: bg
    property string detail: "#7310A2"

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "#fff"
        radius: height
    }

    property int value: 0
    property int max: 0
    property int perValue: 0
    property int percentage: -1
    property int position: -1

    signal change

    function setPosition() {
        perValue = parseInt((position * max) / width)
        value = position
        controlBtn.x = position - (controlBtn.width / 2)
    }

    function setPercentage() {
        perValue = percentage
        value = parseInt((percentage * width) / max)
        controlBtn.x = value - (controlBtn.width / 2)
    }

    function update() {
        if (position != -1) {
            setPosition()
        }

        if (percentage != -1) {
            setPercentage()
        }
    }

    onPositionChanged: {
        update()
    }

    onPercentageChanged: {
        update()
    }

    MouseArea {
        id: mouseControl
        anchors.fill: parent
        property bool move: false

        Rectangle {
            id: controlBtn
            x: 0
            y: -(parent.height / 2) - 2
            width: parent.height * 2 + 4
            height: parent.height * 2 + 4
            radius: parent.height + 2
            color: detail
            z: 1
        }

        Rectangle {
            id: controlBar
            width: controlBtn.x + (controlBtn.width / 2)
            height: parent.height
            radius: parent.height
            color: detail
        }

        RectangularGlow {
            id: effect1
            anchors.fill: controlBtn
            glowRadius: 1
            spread: 0
            color: main.detailColor
            cornerRadius: controlBtn.radius + glowRadius
            opacity: 0.8
        }

        RectangularGlow {
            id: effect2
            anchors.fill: controlBar
            glowRadius: 2
            spread: 0
            color: main.detailColor
            cornerRadius: controlBtn.radius + glowRadius
            opacity: 0.8
        }

        onPressedChanged: {
            move = !move
        }

        onMouseXChanged: {
            if (move && 0 <= mouseX && parent.width >= mouseX) {

                control.perValue = parseInt((mouseX * max) / control.width)
                control.value = mouseX
                percentage = control.perValue
                position = mouseX
                control.change()
            }
        }

        onClicked: {
            control.perValue = parseInt((mouseX * max) / control.width)
            control.value = mouseX
            percentage = control.perValue
            position = mouseX
            control.change()
        }

        onWheel: {

            var scroll = wheel.angleDelta.y / max
            var per = parseInt((2 * control.width) / max)

            if (scroll > 0) {
                if (controlBtn.x < control.width - (controlBtn.width / 2)) {
                    percentage += per
                    control.perValue = parseInt((control.value * max) / control.width)
                }
            } else {
                if (controlBtn.x > -(controlBtn.width / 2)) {
                    percentage -= per
                    control.perValue = parseInt((control.value * max) / control.width)
                }
            }

            change()
        }
    }

    Component.onCompleted: {
        update()
        change()
    }
}
