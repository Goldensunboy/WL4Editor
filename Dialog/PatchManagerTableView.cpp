#include "PatchManagerTableView.h"
#include <cassert>

/// <summary>
/// Construct an instance of the PatchManagerTableView.
/// </summary>
/// <param name="parent">
/// The parent QWidget.
/// </param>
PatchManagerTableView::PatchManagerTableView(QWidget *param) : QTableView(param),
    entryTableModel(this)
{
    // Configure the table
    setModel(&entryTableModel);
    setSelectionBehavior(SelectionBehavior::SelectRows);

    // Populate the table
    QVector<struct PatchEntryItem> patches = GetPatchesFromROM();
    foreach(struct PatchEntryItem patch, patches)
    {
        entryTableModel.AddEntry(patch);
    }

    // TEST
    struct PatchEntryItem TEST1 { QString("foo.c"), PatchType::C, 0x1111AF, 0x800000 };
    struct PatchEntryItem TEST2 { QString("bar.c"), PatchType::C, 0x2222AF, 0x800001 };
    struct PatchEntryItem TEST3 { QString("baz.asm"), PatchType::Assembly, 0x3333AF, 0x800002 };
    struct PatchEntryItem TEST4 { QString("file.bin"), PatchType::Binary, 0x4444AF, 0x800003 };
    entryTableModel.AddEntry(TEST1);
    entryTableModel.AddEntry(TEST2);
    entryTableModel.AddEntry(TEST3);
    entryTableModel.AddEntry(TEST4);

    UpdateTableView();
}

/// <summary>
/// Deconstruct the PatchManagerTableView and clean up its instance objects on the heap.
/// </summary>
PatchManagerTableView::~PatchManagerTableView()
{
    // TODO
}

/// <summary>
/// Update the underlying table based on the patch entry contents
/// </summary>
void PatchManagerTableView::UpdateTableView()
{
    entryTableModel.clear();
    entryTableModel.setHorizontalHeaderLabels(QStringList() << "File" << "Type" << "Hook Address" << "Patch Address");
    int row = 0;
    foreach(const struct PatchEntryItem patchEntry, entryTableModel.entries)
    {
        entryTableModel.setItem(row, 0, new QStandardItem(patchEntry.fileName));
        const char *typeStrings[3] =
        {
            "Binary",
            "Assembly",
            "C"
        };
        assert(patchEntry.patchType < sizeof(typeStrings) / sizeof(typeStrings[0]) /* Patch entry type out of range */);
        entryTableModel.setItem(row, 1, new QStandardItem(QString(typeStrings[patchEntry.patchType])));
        entryTableModel.setItem(row, 2, new QStandardItem(!patchEntry.hookAddress ?
            "none" : "0x" + QString::number(patchEntry.hookAddress, 16).toUpper()));
        entryTableModel.setItem(row++, 3, new QStandardItem("0x" + QString::number(patchEntry.patchAddress, 16).toUpper()));
    }
}

//---------------------------------------------------------------------------------------------------------------------------
// PatchEntryTableModel functions
//---------------------------------------------------------------------------------------------------------------------------

PatchEntryTableModel::PatchEntryTableModel(QWidget *_parent) : QStandardItemModel(_parent), parent(_parent)
{
    // Configure the model
    setColumnCount(3);
}
