#include "RoomConfigDialog.h"
#include "ui_RoomConfigDialog.h"

RoomConfigDialog::RoomConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomConfigDialog)
{
    ui->setupUi(this);

    QStringList TilesetNamesSet;
    TilesetNamesSet << "00  Debug room" << "01  Palm Tree Paradise" << "02  Caves" << "03  The Big Board" << "04  The Big Board" <<
                       "05  The Big Board (indoor)" << "06  Wildflower Fields" << "07  Toy Block Tower" << "08  Factory" << "09  Wildflower Underground" <<
                       "0A  Wildflower WaterPlace" << "0B  Underwater" << "0C  Toy Block Tower" << "0D  Toy Block Tower" << "0E  Toy Block Tower" <<
                       "0F  Doodle woods" << "10  Dominoes" << "11  Hall of Hieroglyphs" << "12  Haunte House" <<
                       "13  Crescent Moon Village outside" << "14  Drain" << "15  Arabian outside" << "16  Arabian inside" << "17  Arabian" <<
                       "18  Arabian" << "19  Arabian" << "1A  Dominoes (blue)" << "1B  Dominoes (purple)" << "1C  Dominoes (teal)" << "1D  Factory" <<
                       "1E  Factory" << "1F  Jungle" << "20  Factory" << "21  Toxic Landfill" << "22  Toxic Landfill" << "23  Pinball" <<
                       "24  Pinball" << "25  Pinball (with Gorilla)" << "26  Jungle" << "27  40 Below Fridge" << "28  Jungle" <<
                       "29  Jungle caves" << "2A  Hotel" << "2B  Hotel" << "2C  Hotel" << "2D  Hotel" << "2E  Hotel" << "2F  Hotel (outside)" <<
                       "30  Unused in-game (Haunted House)" << "31  Unused in-game (Haunted House)" << "32  Unused in-game (Cardboard)" <<
                       "33  Cardboard" << "34  Caves" << "35  Jungle" << "36  Caves" << "37  Lava level" << "38  Caves" << "39  Golden Passage" <<
                       "3A  Hotel" << "3B  Hotel" << "3C  Hotel" << "3D  Hotel" << "3E  40 Below Fridge" << "3F  Factory" << "40  Factory" <<
                       "41  Arabian" << "42  Boss room" << "43  Boss corridor" << "44  Boss room" << "45  Frozen lava level" << "46  Lava level" <<
                       "47  Hall of Hieroglyphs" << "48  Boss room" << "49  Boss room" << "4A  Boss corridor" << "4B  Boss corridor" <<
                       "4C  Boss corridor" << "4D  Boss corridor" << "4E  Boss corridor" << "4F  Boss room (Diva)" << "50  Hall of Hieroglyphs" <<
                       "51  Jungle" << "52  Wildflower" << "53  Crescent Moon Village" << "54  Crescent Moon Village" << "55  Crescent Moon Village" <<
                       "56  Toy Block Tower" << "57  Pinball" << "58  Bonus room" << "59  Bonus room" << "5A  Final level" << "5B  The Big Board end";
    ui->ComboBox_TilesetID->addItems(TilesetNamesSet);
    QStringList LayerPrioritySet;
    LayerPrioritySet << "layer 0 > layer 1 > layer 2 > Layer 3" << "layer 1 > layer 0 > layer 2 > Layer 3" << "layer 1 > layer 2 > layer 0 > Layer 3";
    ui->ComboBox_LayerPriority->addItems(LayerPrioritySet);
}

RoomConfigDialog::~RoomConfigDialog()
{
    delete ui;
}

void RoomConfigDialog::ShowTilesetDetails()
{
    // Set up tileset
    int _tilesetPtr = WL4Constants::TilesetDataTable + currentParams.CurrentTilesetIndex * 36;
    LevelComponents::Tileset *tmpTileset = new LevelComponents::Tileset(_tilesetPtr, currentParams.CurrentTilesetIndex);

    // Set up scene
    QGraphicsScene *tmpTile16MAPScene = new QGraphicsScene(0, 0, 8 * 16, (48 * 2) * 16);
    QPixmap layerPixmap(8 * 16, (48 * 2) * 16);
    layerPixmap.fill(Qt::transparent);

    // Draw the tiles to the QPixmap
    for(int i = 0; i < (48 * 2); ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            tmpTileset->GetMap16Data()[i * 8 + j]->DrawTile(&layerPixmap, j * 16, i * 16);
        }
    }
    tmpTile16MAPScene->addPixmap(layerPixmap);

    ui->graphicsView->repaint();
    ui->graphicsView->setScene(tmpTile16MAPScene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    delete tmpTileset; delete tmpTile16MAPScene; // TODO: Can I delete tmpTile16MAPScene also?
}

void RoomConfigDialog::ShowMappingType20LayerDetails(int _layerdataAddr, LevelComponents::Layer *_tmpLayer)
{
    // TODO: Put these 2 lines somewhere else
    if(_tmpLayer != nullptr) delete _tmpLayer;
    _tmpLayer = new LevelComponents::Layer(_layerdataAddr, LevelComponents::LayerTile8x8);

    int _tilesetPtr = WL4Constants::TilesetDataTable + currentParams.CurrentTilesetIndex * 36;
    LevelComponents::Tileset *tmpTileset = new LevelComponents::Tileset(_tilesetPtr, currentParams.CurrentTilesetIndex);
    QPixmap tmplayerpixmap = _tmpLayer->RenderLayer(tmpTileset);
    int sceneWidth = 64 * 16;
    int sceneHeight = 64 * 16;
    QGraphicsScene *scene = new QGraphicsScene(0, 0, sceneWidth, sceneHeight);
    scene->addPixmap(tmplayerpixmap);
    ui->graphicsView->repaint();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    delete tmpTileset; delete scene; // TODO: Can I delete scene also?
}
