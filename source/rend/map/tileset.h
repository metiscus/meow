#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <cstdint>
#include <map>
#include "tile.h"
#include "../resource.h"

class Tileset
{
public:
    Tileset();
    bool HasTile(uint32_t id) const;
    const Tile& GetTile(uint32_t id) const;

    inline const uint32_t& GetWidth() const
    {
        return width_;
    }

    inline const uint32_t& GetHeight() const
    {
        return height_;
    }

    inline const ResourceId& GetImageAtlasId() const
    {
        return imageAtlasId_;
    }

    static Tileset Load(rapidxml::xml_document<> &doc);

private:
    uint32_t firstId_;
    std::vector<Tile> tiles_;
    uint32_t width_;
    uint32_t height_;
    ResourceId imageAtlasId_;
};

#endif // TILESET_H_INCLUDED
