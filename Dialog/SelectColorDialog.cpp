#include "SelectColorDialog.h"
#include "ui_SelectColorDialog.h"

SelectColorDialog::SelectColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectColorDialog)
{
    ui->setupUi(this);
    pb = new QGraphicsScene();
    ui->graphicsView->SetFather(this);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

SelectColorDialog::~SelectColorDialog()
{
    delete pb;
    delete ui;
}

void SelectColorDialog::SetPalette(QVector<QRgb> palette)
{
    // draw palette Bar
    QPixmap PaletteBarpixmap(16 * 20, 40);
    PaletteBarpixmap.fill(Qt::transparent);
    QPainter PaletteBarPainter(&PaletteBarpixmap);
    for(int i = 0; i < qMin(palette.size(), 16); i++)
    {
        PaletteBarPainter.fillRect(20 * i, 0, 20, 40, palette[i]);
    }
    PalBar = pb->addPixmap(PaletteBarpixmap);
    PalBar->setZValue(0);
    PalBar->setVisible(true);

    // Add the highlighted tile rectangle in SelectionBox_Color
    QPixmap selectionPixmap3(20, 40);
    selectionPixmap3.fill(Qt::transparent);
    QPainter SelectionBoxRectPainter(&selectionPixmap3);
    SelectionBoxRectPainter.setPen(QPen(QBrush(Qt::blue), 2));
    SelectionBoxRectPainter.drawRect(2, 2, 18, 38);
    SelectionBox_Color = pb->addPixmap(selectionPixmap3);
    SelectionBox_Color->setZValue(1);
    SelectionBox_Color->setVisible(true);
    ui->graphicsView->setScene(pb);

    // Data saving
    temppal = palette;
}

void SelectColorDialog::SetColor(int colorId)
{
    SelectionBox_Color->setPos(20 * colorId, 0);
    QColor color = temppal[colorId];
    ui->label_ColorInfo->setText(QString("RGB888: (") +
                                 QString::number(color.red(), 10) + QString(", ") +
                                 QString::number(color.green(), 10) + QString(", ") +
                                 QString::number(color.blue(), 10) + QString(") RGB555: (") +
                                 QString::number(color.red() >> 3, 10) + QString(", ") +
                                 QString::number(color.green() >> 3, 10) + QString(", ") +
                                 QString::number(color.blue() >> 3, 10) + QString(")")); //temppal
}

int SelectColorDialog::GetSelectedColorId()
{
    return ui->graphicsView->GetSelectedColorId();
}
