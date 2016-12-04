import QtQuick 2.4
import QtGraphicalEffects 1.0
import QtQml.Models 2.1
import Ubuntu.Components 1.3
import Ubuntu.Content 1.3
import Doc_Scanner 1.0
/*!
    \brief MainView with a Label and Button elements.
*/

MainView {
    id: mainView

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "doc-scanner.dslul"

    width: units.gu(100)
    height: units.gu(75)
    backgroundColor: "#f2f2f2"
    PageStack {
        id: pageStack
        Component.onCompleted: pageStack.push(mainPage)
    }

    ScanImage {
        id: scanImage
    }

    Page {
        id: mainPage

        property var activeTransfer: null
        property double gridmargin: units.gu(1)
        property double mingridwidth: units.gu(15)

        header: PageHeader {
            id: pageHeader
            title: i18n.tr("Document Scanner")
            StyleHints {
                foregroundColor: UbuntuColors.coolGrey
                //backgroundColor: UbuntuColors.porcelain
                dividerColor: UbuntuColors.lightGrey
            }
        }

        ContentPeer {
            id: exportPeer
            //appId: "com.ubuntu.docviewer_docviewer"
            contentType: ContentType.Documents
            handler: ContentHandler.Destination

            property Component picItem: ContentItem {}

            function save(filePath) {
                console.log(filePath)
                var transfer = exportPeer.request()
                transfer.items = [ picItem.createObject(mainView, { "url": filePath }) ]
                transfer.state = ContentTransfer.Charged
            }
        }

        ContentPeer {
            id: peer
            contentType: ContentType.Pictures
            handler: ContentHandler.Source
            selectionType: ContentTransfer.Multiple
        }

        ContentTransferHint {
            anchors.fill: mainPage
            activeTransfer: mainPage.activeTransfer
        }

        /* Watch mainPage.activeTransfer to find out when content is ready for our use. */
        Connections {
            target: mainPage.activeTransfer
            onStateChanged: {
                if (mainPage.activeTransfer.state === ContentTransfer.Charged)
                    for (var i=0; i<mainPage.activeTransfer.items.length; i++) {
                        var item = mainPage.activeTransfer.items[i]
                        imageModel.append({origimg: String(item.url), imgout: scanImage.elaborate(String(item.url))})
                    }
            }
        }



        DelegateModel {
            id: visualModel
            model: ListModel {
                id: imageModel
                //ListElement {imgout: "/home/phablet/.cache/doc-scanner.dslul/screenshot20161108_105748015.png_out.png"}
                //ListElement {imgout: "/home/phablet/.cache/doc-scanner.dslul/1479221316289.jpg_out.png"}
            }

            delegate: MouseArea {
                id: delegateRoot
                property int visualIndex: DelegateModel.itemsIndex
                width: gridview.cellWidth
                height: gridview.cellHeight
                drag.smoothed: true
                //drag.target: icon

                Item {
                    id: icon
                    width: gridview.cellWidth
                    height: gridview.cellHeight
                    anchors {
                        horizontalCenter: parent.horizontalCenter;
                        verticalCenter: parent.verticalCenter
                    }

                    Drag.active: delegateRoot.drag.active
                    Drag.source: delegateRoot
                    Drag.hotSpot.x: 36
                    Drag.hotSpot.y: 36

                    states: [
                        State {
                            when: icon.Drag.active
                            ParentChange {
                                target: icon
                                parent: mainPage
                            }

                            AnchorChanges {
                                target: icon;
                                anchors.horizontalCenter: undefined;
                                anchors.verticalCenter: undefined
                            }
                        }
                    ]

                    Image {
                        id: mainImg
                        anchors {
                            fill: parent
                            leftMargin: mainPage.gridmargin
                            rightMargin: mainPage.gridmargin
                            topMargin: 1.5*mainPage.gridmargin
                            bottomMargin: 1.5*mainPage.gridmargin
                        }
                        asynchronous: true
                        fillMode: Image.PreserveAspectFit
                        source: Qt.resolvedUrl(model.imgout)
                        // Prevent blurry SVGs
                        sourceSize.width: 2*mainPage.mingridwidth
                        sourceSize.height: 3*mainPage.mingridwidth

                        /* Overlay for when image is pressed */
                        Rectangle {
                            id: overlay
                            anchors.fill: parent
                            color: "#000"
                            border.color: UbuntuColors.orange
                            border.width: 0
                            opacity: delegateRoot.pressed ? 0.3 : 0

                            Behavior on opacity {
                                NumberAnimation {
                                    duration: UbuntuAnimation.SlowDuration
                                }
                            }
                        }
                    }

                    DropShadow {
                        anchors.fill: mainImg
                        horizontalOffset: 5
                        verticalOffset: 5
                        radius: 18
                        samples: 25
                        transparentBorder: true
                        color: "#80000000"
                        source: mainImg
                    }


                }

                DropArea {
                    anchors { fill: parent; margins: 15 }

                    onEntered: {
                        visualModel.items.move(drag.source.visualIndex, delegateRoot.visualIndex)
                        imageModel.move(drag.source.visualIndex, delegateRoot.visualIndex, 1)
                    }
                }

                onPressAndHold: {
                    overlay.border.width = 10
                    mouse.accepted = false
                    delegateRoot.drag.target = icon
                    deleteIcon.visible = true
                    btnAdd.visible = false
                    btnExport.visible = false
                }
                onReleased: {
                    if(icon.Drag.target === deleteDragTarget) {
                        imageModel.remove(model.index)
                    }
                    overlay.border.width = 0
                    delegateRoot.drag.target = undefined
                    deleteIcon.visible = false
                    btnAdd.visible = true
                    btnExport.visible = true
                }
            }
        }

        Item {
            id: topPanel
            anchors {
                left: parent.left
                right: parent.right
                top: pageHeader.bottom
                topMargin: units.gu(2)
            }
            //color: "#f2f2f2"
            height: units.gu(5)

            DropArea {
                id: deleteDragTarget
                anchors.fill: parent
                Icon {
                    id: deleteIcon
                    name: "delete"
                    visible: false
                    color: UbuntuColors.red
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    }
                states: [
                    State {
                        when: deleteDragTarget.containsDrag
                        PropertyChanges {
                            target: deleteIcon
                            color: UbuntuColors.coolGrey
                        }
                    }
                ]
            }

            Button {
                id: btnExport
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: units.gu(2)
                }

                text: i18n.tr("Export PDF")
                color: UbuntuColors.coolGrey

                onClicked: {
                    if(imageModel.count > 0) {
                        var imgarray = []
                        for(var i=0; i < imageModel.count; i++)
                            imgarray.push(imageModel.get(i).imgout)
                        var path = "/home/phablet/.cache/doc-scanner.dslul/" + new Date().valueOf() + ".pdf"
                        scanImage.exportPdf(path, imgarray)
                        exportPeer.save(path)
                    }

                }
            }

            Button {
                id: btnAdd
                anchors {
                    left: btnExport.right
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: units.gu(2)
                    rightMargin: units.gu(2)
                }

                text: i18n.tr("Add")
                color: UbuntuColors.orange

                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ImportPage.qml"))
                }
            }
        }

        GridView {
            id: gridview
            anchors {
                top: topPanel.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: units.gu(2)
                leftMargin: mainPage.gridmargin
                rightMargin: mainPage.gridmargin
            }
            height: mainView.height/2
            clip: true
            cellWidth: width / Math.floor(width/mainPage.mingridwidth)
            cellHeight: cellWidth*1.4

            displaced: Transition {
                NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
            }

            model: visualModel
        }

        Loader {
            id: emptyStateLoader
            anchors.fill: parent
            active: imageModel.count === 0
            source: Qt.resolvedUrl("EmptyDocument.qml")
        }


    }
}


