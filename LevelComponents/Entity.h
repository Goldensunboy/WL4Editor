#ifndef ENTITY_H
#define ENTITY_H

#include <QVector>

#include "ROMUtils.h"
#include "LevelComponents/EntitySet.h"

namespace LevelComponents
{
    struct EntityTile
    {
        int deltaX;
        int deltaY;
        Tile8x8 *objTile;
    };

    struct OAMTile
    {
        int Xoff;
        int Yoff;
        int OAMwidth;
        int OAMheight;
        bool xFlip;
        bool yFlip;
        QVector<EntityTile*> tile8x8;

        QImage Render();

        // Deconstructor for the OAMTile struct
        ~OAMTile()
        {
            foreach(EntityTile *t, tile8x8)
            {
                delete t->objTile;
                delete t;
            }
        }
    };

    class Entity
    {
    private:
        bool xFlip = false;
        bool yFlip = false;
        int EntityID = 0;
        int EntityGlobalID = 0;
        int OAMDataTablePtr = 0;
        int EntityDeltaX = 0, EntityDeltaY = 0;
        int Priority = 0;
        int PaletteOffset = 0;
        bool SemiTransparent = false;
        bool UnusedEntity = false;
        QVector<OAMTile*> OAMTiles;
        EntitySet *currentEntityset;

    public:
        Entity(int entityID, int entityGlobalId, EntitySet *_currentEntityset);
        ~Entity();
        QImage Render();
        int GetPriority() { return Priority; }
        EntityPositionalOffset GetLevelComponents() { return currentEntityset->GetEntityPositionalOffset(EntityGlobalID); }
        int GetEntityID() { return EntityID; }
        int GetEntityGlobalID() { return EntityGlobalID;}

    private:
        void OAMtoTiles(unsigned short *singleOAM);
        void ExtractSpritesTiles(int spritesFrameDataPtr, int frame);
    };
}

#endif // ENTITY_H
