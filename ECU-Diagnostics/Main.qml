import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

ApplicationWindow {
    id: window
    visible: true
    width: 1120
    height: 740
    minimumWidth: 900
    minimumHeight: 620
    title: "CoruhECU Diagnostics v0.0.1"
    color: "#F4F6F9"

    property string activeCategory: "Hydraulic excavator"
    property string activeSubModel: "ZX-7"

    property string detectedStatus: ""
    property string detectedModel: ""
    property string detectedFaultCode: ""
    property string detectedDescription: ""
    property bool isEcuConnected: false

    Component {
        id: mainSelectionPage
        Item {
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.margins: 16
                    spacing: 2
                    Label { text: "Start MP Dr"; font.bold: true; font.pixelSize: 15; color: "#111827" }
                    Label { text: "Select a machine type to which you want to connect."; font.pixelSize: 12; color: "#6B7280" }
                }

                GridLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    Layout.bottomMargin: 10
                    columns: 4
                    rows: 3
                    columnSpacing: 12
                    rowSpacing: 12

                    component MachineCard : Rectangle {
                        property string catName: ""
                        property string catDesc: ""
                        property var models: []
                        property bool isSelected: activeCategory === catName

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: isSelected ? "#15803D" : "#FFFFFF"
                        border.color: isSelected ? "#166534" : (cardMouse.containsMouse ? "#15803D" : "#D1D5DB")
                        border.width: isSelected ? 2 : 1
                        radius: 8

                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: "#20000000"
                            shadowBlur: 0.3
                            shadowVerticalOffset: 3
                        }

                        MouseArea {
                            id: cardMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: { pop.x = parent.width + 6; pop.y = 0; pop.visible = !pop.visible }
                        }

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 3
                            Label { text: catName; font.bold: isSelected; font.pixelSize: 12; color: isSelected ? "#FFFFFF" : "#1F2937"; Layout.alignment: Qt.AlignHCenter }
                            Label { text: catDesc; font.pixelSize: 11; color: isSelected ? "#DCFCE7" : "#4B5563"; Layout.alignment: Qt.AlignHCenter }
                            Label { visible: isSelected && activeSubModel !== ""; text: activeSubModel; font.bold: true; font.pixelSize: 11; color: "#FFFFFF"; Layout.alignment: Qt.AlignHCenter }
                        }

                        Rectangle {
                            width: 16; height: 16; radius: 8
                            color: isSelected ? "#166534" : "#E5E7EB"
                            anchors.right: parent.right; anchors.rightMargin: 10; anchors.verticalCenter: parent.verticalCenter
                            Text { anchors.centerIn: parent; text: "▼"; font.pixelSize: 7; color: isSelected ? "#FFFFFF" : "#374151" }
                        }

                        Popup {
                            id: pop
                            width: 190; height: Math.min(models.length * 32 + 10, 180); padding: 4
                            background: Rectangle { color: "#FFFFFF"; border.color: "#15803D"; border.width: 1; radius: 8 }
                            contentItem: ListView {
                                clip: true; model: models
                                delegate: Rectangle {
                                    width: parent.width; height: 30
                                    color: rm.containsMouse ? "#DCFCE7" : "transparent"; radius: 6
                                    Text { anchors.left: parent.left; anchors.leftMargin: 10; anchors.verticalCenter: parent.verticalCenter; text: modelData; font.pixelSize: 11; color: rm.containsMouse ? "#15803D" : "#374151"; font.bold: rm.containsMouse }
                                    MouseArea { id: rm; anchors.fill: parent; hoverEnabled: true; onClicked: { activeCategory = catName; activeSubModel = modelData; pop.close(); } }
                                }
                            }
                        }
                    }

                    component MachineCardBottom : Rectangle {
                        property string catName: ""
                        property string catDesc: ""
                        property var models: []
                        property bool isSelected: activeCategory === catName

                        Layout.fillWidth: true; Layout.fillHeight: true
                        color: isSelected ? "#15803D" : "#FFFFFF"
                        border.color: isSelected ? "#166534" : (cardMouseBot.containsMouse ? "#15803D" : "#D1D5DB")
                        border.width: isSelected ? 2 : 1; radius: 8

                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: "#20000000"
                            shadowBlur: 0.3
                            shadowVerticalOffset: 3
                        }

                        MouseArea {
                            id: cardMouseBot
                            anchors.fill: parent; hoverEnabled: true
                            onClicked: { popBot.x = 0; popBot.y = parent.height + 6; popBot.visible = !popBot.visible }
                        }

                        ColumnLayout {
                            anchors.centerIn: parent; spacing: 3
                            Label { text: catName; font.bold: isSelected; font.pixelSize: 12; color: isSelected ? "#FFFFFF" : "#1F2937"; Layout.alignment: Qt.AlignHCenter }
                            Label { text: catDesc; font.pixelSize: 11; color: isSelected ? "#DCFCE7" : "#4B5563"; Layout.alignment: Qt.AlignHCenter }
                            Label { visible: isSelected && activeSubModel !== ""; text: activeSubModel; font.bold: true; font.pixelSize: 11; color: "#FFFFFF"; Layout.alignment: Qt.AlignHCenter }
                        }

                        Rectangle {
                            width: 16; height: 16; radius: 8; color: isSelected ? "#166534" : "#E5E7EB"
                            anchors.right: parent.right; anchors.rightMargin: 10; anchors.verticalCenter: parent.verticalCenter
                            Text { anchors.centerIn: parent; text: "▼"; font.pixelSize: 7; color: isSelected ? "#FFFFFF" : "#374151" }
                        }

                        Popup {
                            id: popBot
                            width: parent.width; height: Math.min(models.length * 30 + 8, 170); padding: 4
                            background: Rectangle { color: "#FFFFFF"; border.color: "#15803D"; border.width: 1; radius: 8 }
                            contentItem: ListView {
                                clip: true; model: models
                                delegate: Rectangle {
                                    width: parent.width; height: 28
                                    color: rmBot.containsMouse ? "#DCFCE7" : "transparent"; radius: 6
                                    Text { anchors.left: parent.left; anchors.leftMargin: 10; anchors.verticalCenter: parent.verticalCenter; text: modelData; font.pixelSize: 11; color: rmBot.containsMouse ? "#15803D" : "#374151"; font.bold: rmBot.containsMouse }
                                    MouseArea { id: rmBot; anchors.fill: parent; hoverEnabled: true; onClicked: { activeCategory = catName; activeSubModel = modelData; popBot.close(); } }
                                }
                            }
                        }
                    }

                    MachineCard { catName: "Mini excavator"; catDesc: "(under 6 ton)"; models: ["ZX-10U", "ZX-17U", "ZX-26U", "ZX-33U", "ZX-55U"] }
                    MachineCard { catName: "Mining excavator"; catDesc: "(over 100 ton)"; models: ["EX1200-7", "EX1900-7", "EX2600-7", "EX3600-7"] }
                    MachineCard { catName: "Dump Truck"; catDesc: ""; models: ["EH1100-5", "EH1700-3", "EH3500AC-3", "EH4000AC-3"] }
                    MachineCardBottom { catName: "Wood/soil recycling machine"; catDesc: ""; models: ["VR514", "SR2000G", "ZR9501"] }

                    MachineCard { catName: "Hydraulic excavator"; catDesc: "(6 ton - under 40 ton)"; models: ["ZX-7 Series", "ZX-6 Series", "ZX-5 Series", "ZX-3 Series", "EX-Series"] }
                    MachineCard { catName: "Wheel type hydraulic excavator"; catDesc: ""; models: ["ZXW-7", "ZXW-6 (MC)", "ZXW-6 (DCU)", "ZXW-5", "ZXW-3", "ZXW-1", "EXW-3 or earlier"] }
                    MachineCard { catName: "Crawler Carrier"; catDesc: ""; models: ["EG40R", "EG70R", "EG110R"] }
                    MachineCardBottom { catName: "Others"; catDesc: ""; models: ["Custom Protocol", "Generic OBD-CAN", "Legacy Tier-2"] }

                    MachineCard { catName: "Large excavator"; catDesc: "(40 ton - under 100 ton)"; models: ["ZX350LC-7", "ZX490LCH-7", "ZX690LCH-6", "ZX890LCH-6"] }
                    MachineCard { catName: "Wheel Loader"; catDesc: ""; models: ["ZW140-6", "ZW180-6", "ZW220-6", "ZW310-6", "ZW370-6"] }
                    MachineCard { catName: "Road construction machine"; catDesc: ""; models: ["RT200", "CR300", "PF1600"] }

                    Rectangle { 
                        Layout.fillWidth: true; Layout.fillHeight: true; color: "transparent"; border.color: "#D1D5DB"; border.width: 1; radius: 8 
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: "#10000000"
                            shadowBlur: 0.2
                            shadowVerticalOffset: 2
                        }
                    }
                }
            }
        }
    }

    Component {
        id: genericModulePage
        Item {
            property string moduleTitle: "Module"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 24; spacing: 16
                Label { text: parent.parent.moduleTitle + " Management Console"; font.bold: true; font.pixelSize: 20; color: "#15803D" }
                Rectangle {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    color: "#FFFFFF"; border.color: "#D1D5DB"; radius: 8
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: "#15000000"
                        shadowBlur: 0.3
                        shadowVerticalOffset: 3
                    }
                    ColumnLayout {
                        anchors.centerIn: parent; spacing: 10
                        Label { text: "Active ECU Session Ready"; font.bold: true; font.pixelSize: 16; color: "#111827"; Layout.alignment: Qt.AlignHCenter }
                        Label { text: "Selected Machine: " + activeCategory + " (" + activeSubModel + ")"; font.pixelSize: 13; color: "#4B5563"; Layout.alignment: Qt.AlignHCenter }
                        
                        Label { text: isEcuConnected ? "ECU Bağlantısı Kuruldu" : "Cihaz Bekleniyor..." ; font.bold: true; font.pixelSize: 16; color: isEcuConnected ? "#15803D" : "#DC2626"; Layout.alignment: Qt.AlignHCenter }
                        Label { text: "Algılanan Model: " + (detectedModel !== "" ? detectedModel : "Bilinmiyor"); font.pixelSize: 13; color: "#111827"; Layout.alignment: Qt.AlignHCenter }
                        Label { text: "Aktif Arıza Kodu: " + (detectedFaultCode !== "" ? detectedFaultCode : "Yok"); font.bold: true; font.pixelSize: 13; color: "#DC2626"; Layout.alignment: Qt.AlignHCenter }
                        Label { text: "Açıklama: " + (detectedDescription !== "" ? detectedDescription : "Tarama bekleniyor..."); font.pixelSize: 12; color: "#4B5563"; Layout.alignment: Qt.AlignHCenter }
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            color: "#FFFFFF"

            Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: "#E5E7EB" }

            layer.enabled: true
            layer.effect: MultiEffect {
                shadowEnabled: true
                shadowColor: "#15000000"
                shadowBlur: 0.4
                shadowVerticalOffset: 2
            }

            RowLayout {
                anchors.fill: parent; anchors.leftMargin: 12; anchors.rightMargin: 12; spacing: 4

                component ToolButton : Button {
                    property color textColor: "#4B5563"
                    implicitHeight: 34
                    background: Rectangle { color: parent.hovered ? "#DCFCE7" : "transparent"; radius: 6 }
                    contentItem: Text {
                        text: parent.text; color: parent.hovered ? "#15803D" : parent.textColor
                        font.pixelSize: 11; font.weight: Font.Medium
                        verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
                    }
                }

                ToolButton { text: "Main Menu"; onClicked: stackView.replace(mainSelectionPage) }
                ToolButton { text: "Data Download"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "Data Download"}) }
                ToolButton { text: "Troubleshooting"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "Troubleshooting (DTC)"}) }
                ToolButton { text: "Monitoring"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "Live Monitoring"}) }
                ToolButton { text: "Functions"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "ECU Functions & Calibrations"}) }

                Rectangle { Layout.preferredWidth: 1; Layout.preferredHeight: 20; color: "#E5E7EB" }

                ToolButton { text: "Capture"; onClicked: console.log("Snapshot captured.") }
                ToolButton { text: "Upload"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "Firmware Upload"}) }
                ToolButton { text: "MP Viewer"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "Log MP Viewer"}) }
                ToolButton { text: "Option"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "System Options"}) }
                ToolButton { text: "Help"; onClicked: stackView.replace(genericModulePage, {moduleTitle: "Help & Manual"}) }
                ToolButton { text: "End"; textColor: "#DC2626"; onClicked: Qt.quit() }

                Item { Layout.fillWidth: true }
            }
        }
    
        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: mainSelectionPage
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#FFFFFF"

            Rectangle { anchors.top: parent.top; width: parent.width; height: 1; color: "#E5E7EB" }

            layer.enabled: true
            layer.effect: MultiEffect {
                shadowEnabled: true
                shadowColor: "#15000000"
                shadowBlur: 0.4
                shadowVerticalOffset: -2
            }

            RowLayout {
                anchors.fill: parent; anchors.leftMargin: 16; anchors.rightMargin: 16

                RowLayout {
                    spacing: 12
                    Label { text: "Other operations"; font.bold: true; font.pixelSize: 12; color: "#111827" }
                    Label { text: "Confirmation of Connection Place   →   Browse save data   →   Play recorded file"; font.pixelSize: 11; color: "#6B7280" }
                }

                Item { Layout.fillWidth: true }

                Button {
                    text: "Connect(C)"
                    implicitWidth: 120; implicitHeight: 36

                    background: Rectangle {
                        color: parent.down ? "#166534" : (parent.hovered ? "#16a34a" : "#15803D")
                        radius: 8

                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: "#3015803D"
                            shadowBlur: 0.4
                            shadowVerticalOffset: 3
                        }
                    }

                    contentItem: Text {
                        text: parent.text; color: "#FFFFFF"; font.pixelSize: 12; font.bold: true
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        console.log("Bağlantı ve otomatik tarama başlatılıyor");
                        var result = dbManager.scanAndDetectECU("ECU_SIGNATURE_99"); 
                        detectedStatus = result.status;
                        detectedModel = result.ecuModel;
                        detectedFaultCode = result.faultCode;
                        detectedDescription = result.description;
                        isEcuConnected = (result.status === "Success");
                        console.log("Veriler arayuze aktarildi");
                    }
                }
            }
        }
    }
}