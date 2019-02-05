import QtQuick 2.9
import QtQuick.Window 2.2
import QtWebView 1.1

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Mobile")
    WebView {
            id: webView
            anchors.fill: parent
            url: path
            onLoadingChanged: {
                if (loadRequest.errorString)
                    console.error(loadRequest.errorString);
            }
    }
}
