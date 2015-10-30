#include "tileset.h"

Tileset::Tileset()
    : width_(0U)
    , height_(0U)
    , firstId_(0U)
{

}

Tileset Tileset::Load(rapidxml::xml_document<> &doc)
{
    return Tileset();
}
