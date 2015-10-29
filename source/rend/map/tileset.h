#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <cstdint>
#include <map>
#include "tile.h"

class Tileset
{
public:
    Tileset() = default;
    bool HasTile(uint32_t id) const;
    const Tile& GetTile(uint32_t id) const;

    static Tileset Load(rapidxml::xml_document<> &doc);
private:
    uint32_t firstId_;
    std::vector<Tile> tiles_;
};

#endif // TILESET_H_INCLUDED
