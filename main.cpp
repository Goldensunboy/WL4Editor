#include "Compress.h"
#include "Dialog/DoorConfigDialog.h"
#include "Dialog/PatchEditDialog.h"
#include "Dialog/RoomConfigDialog.h"
#include "DockWidget/CameraControlDockWidget.h"
#include "LevelComponents/Level.h"
#include "ROMUtils.h"
#include "SettingsUtils.h"
#include "WL4Application.h"
#include "WL4EditorWindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <vector>

/// <summary>
/// Load a ROM file into the data array in ROMUtils.cpp.
/// </summary>
/// <param name="filePath">
/// The path to the file that will be read.
/// </param>
bool LoadROMFile(const QString filePath)
{
    // Read ROM file into current file array
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);

    // To check OPEN file
    int length;
    if (!file.isOpen() || (length = (int) file.size()) <= 0xB0)
    {
        file.close();
        return false;
    }

    // Read data
    auto ROMAddr = std::vector<unsigned char> ROMAddr(0x1000000); // set it to be 16 MB to let the data can be write freely
    file.read((char *) ROMAddr, length);
    file.close();

    // To check ROM correct
    if (strncmp((const char *) (ROMAddr + 0xA0), "WARIOLANDE", 10) != 0)
    { // if loaded a wrong ROM
        delete[] ROMAddr;
        return false;
    }

    ROMUtils::CurrentFileSize = length;
    ROMUtils::ROMFilePath     = filePath;
    ROMUtils::CurrentFile     = static_cast<unsigned char *>(ROMAddr);
    return true;
}

/// <summary>
/// Perform all static class initializations.
/// </summary>
static void StaticInitialization_BeforeROMLoading()
{
    RoomConfigDialog::StaticComboBoxesInitialization();
    DoorConfigDialog::StaticInitialization();
    CameraControlDockWidget::StaticInitialization();
    PatchEditDialog::StaticComboBoxesInitialization();
}

/// <summary>
/// Perform static initializations, and then create the main window for the application.
/// </summary>
/// <param name="argc">
/// Number of command line arguments.
/// </param>
/// <param name="argv">
/// Array of command line arguments.
/// </param>
int main(int argc, char *argv[])
{
    StaticInitialization_BeforeROMLoading();

    QApplication application(argc, argv);
    SettingsUtils::InitProgramSetupPath(application);
    QApplication::setWindowIcon(QIcon("./images/icon.ico"));
    WL4EditorWindow window;
    window.show();

    // Quickly test or debug by automatically loading the ROM without UI
    //-------------------------------------------------------------------
    QString filePath = R"(C:\Users\Andrew\Desktop\WL4.gba)";
    QFile testFile(filePath);
    if (testFile.exists())
    {
        window.LoadROMDataFromFile(filePath);
    }
    //-------------------------------------------------------------------

    return QApplication::exec();
}
