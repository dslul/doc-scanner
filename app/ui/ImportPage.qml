import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Content 1.3

Page {
    id: picker

    signal cancel()
    signal imported(var fileUrl)

    header: PageHeader {
        title: i18n.tr("Choose from")
    }

    ContentPeerPicker {
        visible: parent.visible
        contentType: ContentType.Pictures
        handler: ContentHandler.Source

        onPeerSelected: {
            peer.selectionType = ContentTransfer.Multiple
            mainPage.activeTransfer = peer.request()
            pageStack.pop()
//            mainView.activeTransfer.stateChanged.connect(function() {
//                if (mainView.activeTransfer.state === ContentTransfer.Charged) {
//                    picker.imported(mainView.activeTransfer.items[0].url)
//                    mainView.activeTransfer = null
//                }
//            })
        }

    }


}
