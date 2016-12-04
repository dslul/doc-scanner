import QtQuick 2.4
import Ubuntu.Components 1.3

import Ubuntu.Content 1.3

Page {
    id: exportPicker

    header: PageHeader {
        title: i18n.tr("Open with")
    }

    ContentPeerPicker {
        visible: parent.visible
        contentType: ContentType.Documents
        handler: ContentHandler.Destination

        onPeerSelected: {
            exportPeer.selectionType = ContentTransfer.Multiple
            mainPage.activeTransfer = exportPeer.request()
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
