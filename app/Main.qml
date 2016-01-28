import QtQuick 2.3
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.2 as ListItem
import Ubuntu.Layouts 0.1
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
    //useDeprecatedToolbar: false

    width: units.gu(100)
    height: units.gu(76)

    Page {
        title: i18n.tr("Simple-IOT-App")

		Launcher {
			id:launcher
			onIotStatusChanged: {
				console.log("New message received:", newMsg)
				var len = output.length
				console.log("len = ", len)
				output.insert(len, "\n")
				output.insert(len+1, newMsg)
			}
		}

		Component.onCompleted: {
			launcher.init()
		}

		Layouts {
			id: layout
			anchors.fill: parent
			layouts: [
               ConditionalLayout {
				   name: "flow"
				   when: layouts.width > units.gu(60)

				   Flow {
					   anchors.fill: parent
					   flow: Flow.LeftToRight

					   ItemLayout {
						   item: "sidebar"
						   id: sidebar
						   anchors {
							   top: parent.top
							   bottom: parent.bottom
						   }
						   width: parent.width / 3
					   }

					   ItemLayout {
						   item: "colors"
						   anchors {
							   top: parent.top
							   bottom: parent.bottom
							   right: parent.right
							   left: sidebar.right
						   }
					   }
				   }
			   }
		   ]

		   Column {
			   id: sidebar
			   anchors {
				   left: parent.left
				   top: parent.top
				   right: parent.right
			   }
			   Layouts.item: "sidebar"

			   ListItem.Header {
				   text: "Smart Home Control Panel"
			   }

			   ListItem.Standard {
				   id: orangeBtn
				   text: "Ubuntu Orange"
				   selected: false
				   onClicked: selected = !selected
				   control: Switch {
					   checked: false 
					   onClicked: { 
						   hello.color = UbuntuColors.orange
						   console.log("Switch toggled:", checked)
						   launcher.switchLed(checked)
					   }
				   }
			   }

			   ListItem.Standard {
				   id: auberBtn
				   text: "Canonical Aubergine"
				   control: Button {
					   text: "Click me"
					   onClicked: {
						   hello.color = UbuntuColors.lightAubergine
					   }
				   }
			   }

			   ListItem.Standard {
				   id: grayBtn
				   text: "Warm Grey"
				   control: Button {
					   text: "Click me"
					   onClicked: {
						   hello.color = UbuntuColors.warmGrey
					   }
				   }
			   }
		   } // Column

		   Rectangle {
			   id: hello
			   Layouts.item: "colors"
			   color: UbuntuColors.warmGrey
			   anchors {
				   top: sidebar.bottom
				   bottom: parent.bottom
				   left: parent.left
				   right: parent.right
			   }

			   TextArea {
				   id: output
				   text: "Hello (ConditionalLayout) IoT World!"
				   color: "black"
				   readOnly: true
				   anchors {
					   top: hello.top
					   bottom: parent.bottom
					   left: parent.left
					   right: parent.right
				   }
			   }
		   }
		}//Layouts
    }//Page
}//Main view
