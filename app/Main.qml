import QtQuick 2.0
import Ubuntu.Components 1.1
import SimpleC 1.0

/*!
    \brief MainView with Tabs element.
           First Tab has a single Label and
           second Tab has a single ToolbarAction.
*/

MainView {
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "simplec.woodrow-shen"

    /*
     This property enables the application to change orientation
     when the device is rotated. The default is false.
    */
    //automaticOrientation: true

    // Removes the old toolbar and enables new features of the new header.
    useDeprecatedToolbar: false

    width: units.gu(100)
    height: units.gu(76)

    Page {
        title: i18n.tr("Simple-IOT-App")

        Launcher {
            id:launcher
			onIotStatusChanged: {
				console.log("New message received:", newMsg)
				output.append(newMsg)
			}
        }

        //        MyType {
        //            id: myType

        //            Component.onCompleted: {
        //                myType.helloWorld = i18n.tr("Hello world..")
        //            }
        //        }

        Column {
            anchors.fill:parent
            spacing: units.gu(2)
            anchors.margins:units.gu(2)

			Label {
				id:label
				text: "default"
			}

            //spacing: units.gu(1)
            //            anchors {
            //                margins: units.gu(2)
            //                fill: parent
            //            }

            //            Label {
            //                id: label
            //                objectName: "label"

            //                text: myType.helloWorld
            //            }

            Row {
				spacing: units.gu(2)

                TextField{
                    id:command
                }

                Button {
                    //objectName: "button"
                    id:start
                    //width: parent.width

                    text: i18n.tr("Run!")

                    onClicked: {
                        label.text = launcher.launch(command.text)
                    }
                }

                Button {
                    //objectName: "button"
                    id:clear
                    //width: parent.width

                    text: i18n.tr("Clear!")

                    onClicked: {
                        output.text = ""
                    }
                }
            }

			TextEdit {
				id:output
				readOnly: true
				wrapMode: TextEdit.Wrap
            }
        }
    }
}

