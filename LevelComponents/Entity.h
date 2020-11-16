﻿#ifndef ENTITY_H
#define ENTITY_H

#include "Tile.h"

#include <QVector>

namespace LevelComponents
{
    struct EntityPositionalOffset
    {
        int XOffset;
        int YOffset;
    };

    struct EntityTile
    {
        int deltaX;
        int deltaY;
        Tile8x8 *objTile;

        // Deconstructor for the EntityTile struct
        ~EntityTile() { delete objTile; }
    };

    struct OAMTile
    {
        int Xoff;
        int Yoff;
        int OAMwidth;
        int OAMheight;
        bool xFlip;
        bool yFlip;
        QVector<EntityTile *> tile8x8;

        QImage Render();

        // Deconstructor for the OAMTile struct
        ~OAMTile()
        {
            foreach (EntityTile *t, tile8x8)
            {
                delete t;
            }
        }
    };

    class Entity
    {
    public:
        Entity(int entityGlobalId, int basicElementPalettePtr = 0);
        Entity(const Entity &entity); // Copy constructor
        ~Entity();
        QImage Render();
        QImage GetTileMap(const int palNum);
        static EntityPositionalOffset GetEntityPositionalOffset(int entityglobalId);
        int GetEntityGlobalID() { return EntityGlobalID; }
        int GetXOffset() { return xOffset; }
        int GetYOffset() { return yOffset; }
        int GetPalNum() const { return EntityPaletteNum; }
        QVector<QRgb> GetPalette(const int palId) const;
        QVector<Tile8x8*> GetSpriteTiles(QVector<QRgb> *newPal) const;
        int GetTilesNum() { return tile8x8data.size(); }

    private:
        QVector<QRgb> palettes[16]; //i don't want to do some memory management here, so i just set it to be 16
        QVector<Tile8x8*> tile8x8data;
        int xOffset = 0, yOffset = 0;
        int EntityGlobalID = 0;
        int EntityPaletteNum = 0;
        bool UnusedEntity = false;
        QVector<OAMTile *> OAMTiles;

        void LoadSubPalettes(int paletteNum, int paletteSetPtr, int startPaletteId = 0);
        void LoadSpritesTiles(int tileaddress, int datalength);
        void OAMtoTiles(unsigned short *singleOAM);
        void ExtractSpritesTiles();

        // clang-format off
        static constexpr const unsigned char EntitySampleOamNumArray[129] =
        {
            5, 5, 5, 5, 4, 4, 3, 4, 4, 0, 0, 0, 0, 0, 0, 0,
            4, 3, 3, 3, 1, 2, 13, 5, 8, 4, 4, 4, 2, 2, 1, 6,
            1, 2, 1, 1, 3, 3, 4, 4, 5, 1, 2, 2, 13, 1, 3, 3,
            1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 2, 3, 1, 10,
            5, 3, 3, 1, 0, 1, 1, 2, 6, 4, 5, 5, 3, 2, 4, 7,
            7, 42, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 2, 5, 1, 4,
            5, 4, 8, 2, 2, 2, 2, 2, 2, 1, 6, 3, 5, 7, 8, 3,
            3, 3, 5, 2, 1, 2, 1, 5, 4, 5, 4, 6, 7, 4, 1, 1,
            9  // 0x80
        };

        static constexpr const unsigned short EntitiesOamSampleSets[129][0x2A * 3] =
        {
            {0xE3, 0x11FD, 0x14, 0xE0, 0x41F0, 0xC, 0xE0, 0x5000, 0xC, 0xF0, 0x41F0, 0xA, 0xF0, 0x5000, 0xA}, //0x00
            {0xE5, 0x1FD, 0x34, 0xE0, 0x41F0, 0xC, 0xE0, 0x5000, 0xC, 0xF0, 0x41F0, 0xA, 0xF0, 0x5000, 0xA},
            {0xE5, 0x11FB, 0x34, 0xE0, 0x41F0, 0xC, 0xE0, 0x5000, 0xC, 0xF0, 0x41F0, 0xA, 0xF0, 0x5000, 0xA},
            {0xE3, 0x1FB, 0x14, 0xE0, 0x41F0, 0xC, 0xE0, 0x5000, 0xC, 0xF0, 0x41F0, 0xA, 0xF0, 0x5000, 0xA},
            {0xE0, 0x41F0, 0x2, 0xE0, 0x5000, 0x2, 0xF0, 0x41F0, 0, 0xF0, 0x5000, 0},
            {0xF0, 0x41F0, 0, 0xF0, 0x5000, 0, 0xE0, 0x41F0, 0x15, 0xE0, 0x5000, 0x15},
            {0xEE, 0xB, 0x205A, 0x40E8, 0x81F0, 0x20AA, 0x40F8, 0x1F8, 0x20A8},
            {0x40D1, 0x81F4, 0x20B9, 0x40F0, 0x81EC, 0x20E3, 0x40DF, 0x81EC, 0x209C, 0x40EF, 0x41EC, 0x20DC},
            {0xFA, 0x1F3, 0x417B, 0x40FE, 0x1F0, 0x40A6, 0x4000, 0x1F9, 0x40C0, 0x40F8, 0x1F9, 0x40A4},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {0xE9, 0x41F3, 0x3106, 0x80E9, 0x3, 0x3108, 0xF0, 0x41F4, 0x3109, 0xF8, 0x4, 0x312B}, //0x10
            {0x40F3, 0x41EF, 0x53, 0x40F8, 0x1F6, 0, 0xEC, 0x41F6, 0x40},
            {0xE0, 0xB1F4, 0x101C, 0xEB, 0x41F6, 0x1040, 0x40F8, 0x1F8, 0x1026},
            {0xE0, 0xB1F4, 0x201C, 0xEB, 0x41F6, 0x2040, 0x40F8, 0x1F8, 0x2026},
            {0xE0, 0xC1E0, 0},
            {0xF0, 0x41F6, 0, 0x80F0, 0x6, 0x2},
            {0xED, 0x11FC, 0, 0x40F5, 0x11F4, 0x21, 0xED, 0x11F4, 0x2, 0x80F4, 0x11FA, 0xE, 0xFC, 0x11F2, 0x2F, 0xF7, 0x11FE, 0xF, 0x80EE, 0x1003, 0xB, 0x40EE, 0x11F7, 0x9, 0xF8, 0x41FC, 0x11, 0x7, 0x1FE, 0x15, 0x4003, 0x11FD, 0x29, 0x80EE, 0x9, 0x19, 0xEE, 0x11, 0x1A},
            {0xF8, 0x1FE, 0xB, 0x40EE, 0x1F7, 0x2D, 0x80EA, 0x1000, 0, 0x80EA, 0x1F8, 0, 0xF4, 0x1F9, 0xB},
            {0x80F1, 0x1F8, 0x10EA, 0x80F1, 0x1000, 0x10EA, 0xE8, 0xC1E0, 0x1041, 0x80E8, 0x4020, 0x1049, 0x8, 0x20, 0x10C9, 0x40D8, 0x81E8, 0x1002, 0xD8, 0x4008, 0x1006, 0x80F0, 0x1D8, 0x1060},
            {0xE6, 0x1FC, 0x36, 0x40F6, 0x1FA, 0x11, 0x40EE, 0x81F9, 0, 0xF8, 0x1F9, 0x10},
            {0xEB, 0x41F0, 0x1E, 0xEB, 0x5000, 0x1E, 0x40FB, 0x1F0, 0x1A, 0x40FB, 0x1000, 0x1A},
            {0xFA, 0x1FA, 0x17, 0xFA, 0x3, 0x17, 0xF0, 0x41EE, 0, 0x40EE, 0x81F7, 0x40},
            {0x40EC, 0x81F1, 0x15, 0x40F8, 0x1F9, 0},
            {0x40EC, 0x81F1, 0x1015, 0x40F8, 0x1F9, 0x1000},
            {0xEC, 0x1FC, 0x10},
            {0x40EA, 0x91EA, 0x40, 0x40FA, 0x51EA, 0x46, 0x40F8, 0x11F8, 0x4, 0xE4, 0x11F8, 0x38, 0x40DB, 0x81F0, 0x1C, 0x40EB, 0x41F0, 0x5C},
            {0x40F0, 0x1F8, 0}, //0x20
            {0x40F3, 0x81F5, 0, 0x4003, 0x41ED, 0x3},
            {0xFC, 0x1FC, 0x39},
            {0x40F8, 0x81F0, 0x12},
            {0xF6, 0x41F4, 0xE, 0x80F6, 0x4, 0x10, 0x4005, 0x1F8, 0xB},
            {0xF6, 0x41F4, 0xE, 0x80F6, 0x4, 0x10, 0x4005, 0x1F8, 0xB},
            {0x40FF, 0x1E8, 0x78, 0xF7, 0x41F8, 0x1A, 0x40FF, 0x10, 0x7A, 0x80F7, 0x8, 0x1B},
            {0x80EF, 0x5, 0xE, 0x40F8, 0x1F8, 0x17, 0xEB, 0x41F8, 0x2, 0xE5, 0x41EC, 0x6},
            {0xF5, 0x1FC, 0x206D, 0x80FA, 0x1FC, 0x200C, 0x80F0, 0x81F8, 0x2, 0xF0, 0x81F0, 0x13, 0x4010, 0x1F8, 0x17},
            {0xF0, 0x81F0, 0x10},
            {0x40F8, 0x41F1, 0xE, 0x40E9, 0x81F2, 0},
            {0x40F8, 0x41F1, 0x100E, 0x40E9, 0x81F2, 0x1000},
            {0x40F0, 0x8010, 0x12D, 0xF0, 0x41FE, 0x129, 0xC8, 0x81E4, 0, 0x80C8, 0x8004, 0x4, 0x80C8, 0x4014, 0x6, 0x40E8, 0x81E4, 0x80, 0xE8, 0x4004, 0x84, 0x80E8, 0x14, 0x86, 0x40F8, 0x41E4, 0xC0, 0x40F8, 0x4, 0xC4, 0xF8, 0x14, 0xC6, 0xF0, 0x41E6, 0xE9, 0x80F0, 0x1F6, 0xEB},
            {0, 0, 0x5F},
            {0xE0, 0x31DD, 0x20A3, 0x40DC, 0x11E0, 0x2083, 0xD8, 0x51F0, 0x2080},
            {0xE0, 0x201B, 0x20A3, 0x40DC, 0x10, 0x2083, 0xD8, 0x4000, 0x2080},
            {0x40E8, 0xC1E0, 0x1000}, //0x30
            {0x40E8, 0xD1E0, 0x1000},
            {0x40E8, 0xC1E0, 0x1000, 0x80F0, 0x8020, 0x1000},
            {0x40E8, 0xD1E0, 0x1000, 0x80F0, 0x91D0, 0x1000},
            {0x40E8, 0xC1E0, 0x1000, 0x80E0, 0x8020, 0x1000},
            {0x40E8, 0xD1E0, 0x1000, 0x80E0, 0x91D0, 0x1000},
            {0xEC, 0x24, 0x10A3, 0, 0x401C, 0x1080, 0x80F0, 0x20, 0x1082, 0x40E8, 0xC1E0, 0x1000},
            {0xEC, 0x11D4, 0x10A3, 0, 0x51D4, 0x1080, 0x80F0, 0x11D8, 0x1082, 0x40E8, 0xD1E0, 0x1000},
            {0xEC, 0x2024, 0x10A3, 0xD0, 0x601C, 0x1080, 0x80E0, 0x2020, 0x1082, 0x40E8, 0xC1E0, 0x1000},
            {0xEC, 0x31D4, 0x10A3, 0xD0, 0x71D4, 0x1080, 0x80E0, 0x31D8, 0x1082, 0x40E8, 0xD1E0, 0x1000},
            {0xF8, 0x28, 0xB1, 0x40F8, 0x1, 0x8D, 0xF2, 0x41F3, 0x89, 0x40F8, 0x4008, 0xAD},
            {0x40E0, 0x81F0, 0, 0x40F0, 0x81F0, 0x4},
            {0x40F0, 0x81F0, 0, 0x4000, 0x1F8, 0x4},
            {0x80F4, 0x3, 0xF, 0xF4, 0x41F8, 0xC, 0x80F4, 0x1F5, 0xE},
            {0xE2, 0x81F0, 0xC},
            {0x40D0, 0x81E7, 0x46, 0xD0, 0x4007, 0x9E, 0xDE, 0x81E7, 0xC, 0x80DE, 0x8007, 0x10, 0xEC, 0x41E0, 0x1B, 0xF4, 0x41E8, 0x3C, 0xEC, 0x5010, 0x1B, 0xF4, 0x5008, 0x3C, 0xFC, 0x41F5, 0x12, 0xFC, 0x5, 0x14},
            {0xEA, 0x41F7, 0x8, 0x40FA, 0x1F7, 0x48, 0xEC, 0x5, 0x60, 0xEC, 0x11F5, 0x60, 0x4000, 0x1F8, 0x3E}, //0x40
            {0x40F1, 0x81F0, 0x52, 0x80F0, 0x11FB, 0x1F, 0x40E0, 0x81F0, 0x11},
            {0xF2, 0x41F2, 0xC, 0x40F8, 0x1FA, 0x1A, 0xF8, 0xA, 0x1C},
            {0xE0, 0x81F0, 0},
            {},
            {0xE0, 0x81F0, 0x4},
            {0xE0, 0x81F0, 0x8},
            {0x80D8, 0x81F0, 0x1012, 0x80D8, 0x9000, 0x1012},
            {0x80EB, 0x5, 0x15, 0xF6, 0x1F6, 0x52, 0x80D6, 0x81EF, 0, 0x80DE, 0x11EB, 0x21, 0xF8, 0x1, 0x53, 0xDC, 0x91EE, 0x2},
            {0xE4, 0x41F2, 0x4B, 0xDD, 0x91EA, 0, 0x40F5, 0x1EE, 0xB, 0xF9, 0x2, 0x2C},
            {0xF3, 0x41F2, 0, 0x4003, 0x1F2, 0x40, 0x80F5, 0, 0x6, 0x80FD, 0x8, 0x7, 0x3, 0x2, 0x42},
            {0xF3, 0x41F2, 0, 0x4003, 0x1F2, 0x40, 0x80F5, 0, 0x6, 0x80FD, 0x8, 0x7, 0x3, 0x2, 0x42},
            {0x40F9, 0x1FC, 0x66, 0x40EE, 0x81F0, 0, 0xE8, 0x1F7, 0x8},
            {0x40F8, 0x81F4, 0, 0x40F0, 0x41F4, 0xE},
            {0xE8, 0x41F2, 0, 0x80E8, 0x2, 0x2, 0x40F8, 0x1F2, 0x40, 0xF8, 0x2, 0x42},
            {0x80EE, 0x8006, 0x1C, 0x80EC, 0x4010, 0x1E, 0x80F6, 0x51E8, 0x1E, 0xE0, 0x81E8, 0, 0x80E0, 0x8008, 0x4, 0, 0x81E8, 0x6, 0x8000, 0x8008, 0xA},
            {0x80EE, 0x8006, 0x1C, 0x80EC, 0x4010, 0x1E, 0x80F6, 0x51E8, 0x1E, 0xE0, 0x81E8, 0, 0x80E0, 0x8008, 0x4, 0, 0x81E8, 0x6, 0x8000, 0x8008, 0xA}, //0x50
            {0x40FE, 0x81EA, 0xB6, 0xE, 0x41E2, 0xF5, 0x6, 0x1E2, 0xD5, 0x12, 0x4000, 0x5E, 0x4022, 0, 0x9E, 0xE, 0x4001, 0x1AE, 0x800E, 0x11, 0x1B0, 0x401E, 0x1, 0x1EE, 0x1E, 0x11, 0x1F0, 0x11, 0x400E, 0x7C, 0xD4, 0x400F, 0x1BD, 0x80D4, 0x1F, 0x1BF, 0x40E4, 0x17, 0x1FE, 0xD7, 0x81E0, 0xA1, 0x80D7, 0x8000, 0xA5, 0x80D7, 0x4010, 0xA7, 0x40F7, 0x41E0, 0x121, 0x80DF, 0x4018, 0xC8, 0x80E7, 0x1D8, 0xE0, 0x40F7, 0x4000, 0x125, 0xEF, 0x81E8, 0x141, 0x80EF, 0x8008, 0x145, 0x400F, 0x41E8, 0x1C1, 0x400F, 0x8, 0x1C5, 0x80EF, 0x41E0, 0x140, 0x8, 0x81F0, 0x194, 0x8008, 0x8010, 0x198, 0x8007, 0x1E8, 0x193, 0xF8, 0x4010, 0x157, 0x8, 0x41DB, 0x7A, 0x3, 0x41DD, 0x1AB, 0x8003, 0x1ED, 0x1AD, 0x4013, 0x1DD, 0x1EB, 0x13, 0x1ED, 0x1ED, 0, 0x402F, 0x72, 0x4010, 0x8020, 0x60, 0x4008, 0x1F, 0x95, 0x8001, 0x2F, 0x77, 0xF1, 0x4031, 0x78, 0xD3, 0x41D9, 0x1BA, 0x80D3, 0x1E9, 0x1BC, 0xE3, 0x1D9, 0x1FA}, // flowerpot pal -3
            {0xF0, 0x81F0, 0x10},
            {0, 0xC000, 0x8},
            {0, 0xC000, 0},
            {0xE0, 0xC1E0, 0},
            {0xE0, 0xC1E0, 0},
            {0xE8, 0x81F0, 0x1C},
            {0x40E8, 0x1F0, 0x1040, 0x40E8, 0x1000, 0x1040, 0xF0, 0x81F0, 0x100A},
            {0xF0, 0xA1F0, 0x100A, 0x4010, 0x21F0, 0x1040, 0x4010, 0x3000, 0x1040},
            {0xF0, 0x81F0, 0x1012, 0x8000, 0x1E8, 0x1047, 0x80F0, 0x21E8, 0x1047},
            {0xF0, 0x91F0, 0x1012, 0x80F0, 0x3010, 0x1044, 0x8000, 0x1010, 0x1044},
            {0xF8, 0x41F8, 0x1008, 0xF0, 0x81F0, 0x100E},
            {0xF5, 0x1FC, 0x6D, 0x80FA, 0x1FC, 0xC, 0x80F0, 0x81F8, 0x2, 0xF0, 0x81F0, 0x13, 0x4010, 0x1F8, 0x17},
            {0x40F3, 0, 0x1D},
            {0, 0xA1E0, 0, 0xE0, 0x81E0, 0, 0, 0xB000, 0, 0xE0, 0x9000, 0},
            {0xF1, 0x1F7, 0xB0, 0xE0, 0x41F3, 0x40, 0x80E0, 0x3, 0x42, 0xF0, 0x41F3, 0x4D, 0x80F0, 0x3, 0x4F}, //0x60
            {0xE0, 0x41F3, 0x2004, 0x80E0, 0x3, 0x2006, 0xF0, 0x41F3, 0x2017, 0x80F0, 0x3, 0x2019},
            {0xC0, 0x81E0, 0, 0xE0, 0x81E0, 0x4, 0, 0x81E0, 0x4, 0x20, 0x81E0, 0x4, 0xE0, 0x9000, 0x4, 0, 0x9000, 0x4, 0x20, 0x9000, 0x4, 0xC0, 0x9000, 0x8},
            {0x80C0, 0xC000, 0x4, 0x80C0, 0xD1E0, 0x4},
            {0x80C0, 0xC000, 0x4, 0x80C0, 0xD1E0, 0x4},
            {0x80C0, 0xC000, 0x4, 0x80C0, 0xD1E0, 0x4},
            {0x80C0, 0xC000, 0x4, 0x80C0, 0xD1E0, 0x4},
            {0x80C0, 0xC000, 0x4, 0x80C0, 0xD1E0, 0x4},
            {0x80C0, 0xC000, 0x4, 0x80C0, 0xD1E0, 0x4},
            {0xE0, 0xC1E0, 0x3000},
            {0x80E2, 0x8000, 0x12, 0x80E2, 0x91F0, 0x12, 0x40E0, 0x1FA, 0x80, 0x4000, 0x81F4, 0, 0xE7, 0x8008, 0xA, 0xF9, 0x81E1, 0x1C},
            {0x40F8, 0x51F0, 0x67, 0xEF, 0x41EC, 0xD, 0xDD, 0x81F4, 0x4},
            {0xF5, 0x1F7, 0x4, 0xFC, 0x1, 0x13, 0xFA, 0x41FF, 0xD, 0xF8, 0x41F2, 0x2, 0x40FA, 0x1E3, 0x31},
            {0x80E0, 0x1F8, 0x1A, 0x80E0, 0x1000, 0x1A, 0x40F0, 0x1F0, 0x1E, 0x40F0, 0x1000, 0x1E, 0x40F8, 0x1F8, 0x3C, 0x80E2, 0x1F2, 0x1B, 0x40EB, 0x7, 0x1C},
            {0x4000, 0x81E8, 0, 0, 0x4008, 0x4, 0x4010, 0x81E8, 0x6, 0x10, 0x4008, 0xA, 0x4020, 0x81E8, 0, 0x20, 0x4008, 0x4, 0x4030, 0x81E8, 0x6, 0x30, 0x4008, 0xA},
            {0x40F0, 0x81E8, 0, 0x40F0, 0x8017, 0x6, 0xF0, 0x4008, 0x4},
            {0x40F0, 0x81E8, 0x40, 0x40F0, 0x8017, 0x6, 0xF0, 0x4008, 0x44}, //0x70
            {0x40F0, 0x81E8, 0x100A, 0x40F0, 0x8017, 0x1006, 0xF0, 0x4008, 0x100E},
            {0xF4, 0x51F5, 0, 0xFC, 0x1001, 0x1F, 0xFB, 0x1F6, 0x1F, 0x80F7, 0x1002, 0x18, 0xFC, 0x51F8, 0xB},
            {0xF8, 0x1F8, 0x1B, 0xF8, 0x1000, 0x1B},
            {0xF0, 0x81F0, 0x11},
            {0x40F0, 0x1FA, 0x15, 0x80F8, 0x1FC, 0x14},
            {0xE0, 0x81F0, 0x6018},
            {0xEB, 0x41FA, 0x17, 0x40F2, 0x41E4, 0x2C, 0x40EC, 0x81F2, 0x2, 0x40F9, 0x1F9, 0x10, 0x40F8, 0x1F2, 0x10},
            {0x40F8, 0x1F8, 0x4, 0x40EF, 0x81EC, 0x7, 0xE4, 0x41FD, 0xD, 0x40F7, 0x1EC, 0x3B},
            {0xDE, 0x41EF, 0x7, 0xEE, 0x41F2, 0x47, 0xDD, 0x91EE, 0, 0x40F6, 0x1EF, 0xB, 0xF9, 0x2, 0x2C},
            {0xE7, 0x41F3, 0, 0x80E7, 0x3, 0x2, 0xEE, 0x41F4, 0x3, 0xF6, 0x4, 0x25},
            {0x40EC, 0x81F1, 0, 0xF9, 0x1F9, 0x3F, 0xF9, 0x2, 0x3F, 0xF8, 0x100A, 0x1F, 0x40F2, 0x1FB, 0x28, 0x40F0, 0x1EA, 0x2C},
            {0xEF, 0x100A, 0x3C, 0xEF, 0x1EE, 0x3C, 0x40F6, 0x1F8, 0x4, 0xE9, 0x41F0, 0xD, 0xE9, 0x5000, 0xD, 0x40F9, 0x1F0, 0x9, 0x40F9, 0x1000, 0x9},
            {0x40D1, 0x81F4, 0x5039, 0x40F0, 0x81EC, 0x5063, 0x40DF, 0x81EC, 0x501C, 0x40EF, 0x41EC, 0x505C},
            {0xE0, 0x81F0, 0},
            {0xF8, 0x41F8, 0},
            {0xF8, 0x6, 0x2C, 0x80F0, 0x1FE, 0xB, 0x80ED, 0x1F0, 0x12, 0xE7, 0x41F1, 0, 0x80E7, 0x1, 0x2, 0x40F7, 0x1F1, 0x3, 0xF8, 0x1, 0xE, 0xF0, 0x41F8, 0x8, 0xF8, 0x1F4, 0xD} //0x80
        };

        static constexpr const int EntityPositinalOffset[258] =
        {
             -98,  -66, //0x00
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -67,  -70,
             -98,  -66,
            -130, -130,
               0,    0,
               0,    0,
               0,    0,
               0,    0,
               0,    0,
               0,    0,
               0,    0,
             -98,  -66, //0x10
            -101,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -98,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66, //0x20
             -98,  -98,
             -98,  -66,
             -98,  -98,
             -98,  -98,
             -98,  -98,
             -98,  -78,
             -98,  -66,
             -67, -130,
             -98,  -98,
            -100,  -66,
            -100,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
            -130,  -66, //0x30
             -67,  -66,
            -130,  -66,
             -67,  -66,
            -130,  -66,
             -67,  -66,
            -130,  -66,
             -67,  -66,
            -130,  -66,
             -67,  -66,
            -130,  -66,
             -67,  -66,
             -98,  -66,
             -98,  -74,
             -98,  -66,
             -98,  -66,
            -100,  -98, //0x40
             -98,  -66,
             -98,  -66,
             -67,  -66,
             -67,  -66,
             -67,  -66,
             -67,  -66,
             -67,  -98,
            -100,  -66,
            -101,  -66,
             -98,  -66,
             -98,  -66,
            -101,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66, //0x50
             -98,   -3,
             -98,  -98,
             -98,  -66,
             -98,  -66,
             -98,  -98,
             -98,  -98,
             -98,  -66,
             -67, -130,
             -67, -130,
             -67, -130,
             -67, -130,
             -67, -130,
             -67, -130,
             -98,  -66,
             -35, -130,
             -98,  -66, //0x60
            -100,  -66,
             -67,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
            -194, -321,
             -98,  -66,
             -98,  -66,
             -98,  -66,
            -102,  -34,
             -98,  -66,
             -98,  -66,
             -98,  -66, //0x70
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
             -98,  -66,
            -100,  -66,
            -100,  -66,
            -101,  -66,
             -98,  -66,
            -100,  -66,
             -98,  -66,
               0,    0,
             -67,  -66,
             -98,  -98,
             -98,  -66  //0x80
        };
        // clang-format on
    };
} // namespace LevelComponents

#endif // ENTITY_H
