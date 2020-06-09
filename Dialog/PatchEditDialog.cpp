#include <QMessageBox>
#include <QFileDialog>
#include "PatchEditDialog.h"
#include "ui_PatchEditDialog.h"
#include "ROMUtils.h"

static QStringList PatchTypeNameSet;

/// <summary>
/// Perform static initializtion of constant data structures for the dialog.
/// </summary>
void PatchEditDialog::StaticComboBoxesInitialization()
{
    PatchTypeNameSet << "Binary" << "Assembly" << "C";
}

/// <summary>
/// Construct an instance of the PatchEditDialog.
/// </summary>
/// <param name="parent">
/// The parent QWidget.
/// </param>
/// <param name="patchEntry">
/// The patch entry whose fields are used to initialize the dialog.
/// </param>
PatchEditDialog::PatchEditDialog(QWidget *parent, struct PatchEntryItem patchEntry) :
    QDialog(parent),
    ui(new Ui::PatchEditDialog)
{
    ui->setupUi(this);

    // Initialize the items in comboBox_PatchType
    ui->comboBox_PatchType->addItems(PatchTypeNameSet);

    // Set Validator for lineEdit_HookAddress
    QRegExp regExp1("[a-fA-F0-9]{6}");
    ui->lineEdit_HookAddress->setValidator(addressvalidator = new QRegExpValidator(regExp1, this));

    // Set Validator for lineEdit_HookText
    QRegExp regExp2("( *[a-fA-F0-9] *[a-fA-F0-9])*( *[pP])?( *[a-fA-F0-9] *[a-fA-F0-9])* *");
    ui->lineEdit_HookText->setValidator(addressvalidator = new QRegExpValidator(regExp2, this));

    // Initialize the components with the patch entry item
    InitializeComponents(patchEntry);
}

/// <summary>
/// Normalize the hook text to a standard format for the patch saving.
/// </summary>
/// <remarks>
/// Spaces and "P" identifier are removed, all hex digits are made uppercase.
/// </remarks>
/// <param name="str">
/// The string whose contents to normalize.
/// </param>
/// <returns>
/// The string in a format like: 8F69B144A08956BC
/// </returns>
static QString NormalizeHookText(QString str)
{
    str = str.replace(" ", ""); // remove whitespace
    str = str.toUpper();        // uppercase
    str = str.replace("P", ""); // placeholder bytes for patch address
    return str;
}

/// <summary>
/// Find the index of the patch address identifier in hook text.
/// </summary>
/// <param name="str">
/// The string whose contents to analyze.
/// </param>
/// <returns>
/// The index (in bytes, not characters) of the patch address identifier.
/// </returns>
static unsigned int GetPatchOffset(QString str)
{
    str = str.replace(" ", "").toUpper();
    int index = str.indexOf("P");
    return index > 0 ? index / 2 : -1;
}

/// <summary>
/// Format the normalized hook text in a nice, human-readable format.
/// </summary>
/// <param name="hookStr">
/// The normalized hook text.
/// </param>
/// <param name="patchOffset">
/// The offset of the patch address.
/// </param>
/// <returns>
/// The string in a format like: 8F 69 B1 44 A0 89 P 56 BC
/// </returns>
static QString FormatHookText(QString hookStr, int patchOffset)
{
    QString ret;
    for(int i = 0;; ++i)
    {
        if(i == patchOffset) ret += " P";
        if(i == hookStr.length() / 2) break;
        ret += QString(" %1").arg(hookStr.mid(i * 2, 2));
    }
    return ret.length() ? ret.mid(1) : "";
}

/// <summary>
/// Deconstruct the PatchEditDialog and clean up its instance objects on the heap.
/// </summary>
PatchEditDialog::~PatchEditDialog()
{
    delete addressvalidator;
    delete ui;
}

/// <summary>
/// Initialize the components of the PatchEditDialog.
/// </summary>
/// <param name="patchEntry">
/// The patch entry whose fields are used to initialize the dialog.
/// </param>
void PatchEditDialog::InitializeComponents(struct PatchEntryItem patchEntry)
{
    ui->lineEdit_FilePath->setText(patchEntry.FileName);
    ui->comboBox_PatchType->setCurrentIndex(patchEntry.PatchType);
    QString hookText = patchEntry.HookAddress ? QString::number(patchEntry.HookAddress, 16).toUpper() : "";
    ui->lineEdit_HookAddress->setText(hookText);
    ui->lineEdit_HookText->setText(FormatHookText(patchEntry.HookString, patchEntry.PatchOffsetInHookString));
}

/// <summary>
/// Create a patch entry struct based on the fields of the PatchEditDialog.
/// </summary>
/// <returns>
/// The patch entry.
/// </returns>
struct PatchEntryItem PatchEditDialog::CreatePatchEntry()
{
    return
    {
        ui->lineEdit_FilePath->text(),
        static_cast<enum PatchType>(ui->comboBox_PatchType->currentIndex()),
        static_cast<unsigned int>(ui->lineEdit_HookAddress->text().toInt(Q_NULLPTR, 16)),
        NormalizeHookText(ui->lineEdit_HookText->text()),
        GetPatchOffset(ui->lineEdit_HookText->text()),

        // The following fields are populated and used by the saving routine
        0,
        ""
    };
}

/// <summary>
/// This slot function will be triggered when clicking the "Browse" button.
/// </summary>
void PatchEditDialog::on_pushButton_Browse_clicked()
{
    // Promt the user for the patch file
    QString qFilePath = QFileDialog::getOpenFileName(
        this,
        tr("Open patch file"),
        QString(""),
        tr("C source files (*.c);;ARM assembly files (*.s);;Binary files (*.bin)")
    );
    if(qFilePath.contains(";"))
    {
        QMessageBox::information(this, "About", "The file path may not contain a semicolon. File path: \"" + qFilePath + "\"");
    }
    else if(!qFilePath.isEmpty())
    {
        ui->lineEdit_FilePath->setText(qFilePath);

        // Infer the type based on the extension
        if(qFilePath.endsWith(".c", Qt::CaseInsensitive))
        {
            ui->comboBox_PatchType->setCurrentIndex(PatchType::C);
        }
        else if(qFilePath.endsWith(".s", Qt::CaseInsensitive))
        {
            ui->comboBox_PatchType->setCurrentIndex(PatchType::Assembly);
        }
        else
        {
            ui->comboBox_PatchType->setCurrentIndex(PatchType::Binary);
        }
    }
}
