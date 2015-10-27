#include "imageatlas.h"

const ResourceType ImageAtlas::TypeId =
{
    0xce, 0x2f, 0x10, 0x5b,
    0x25, 0x38,
    0x43, 0xea,
    0x9e, 0xb0,
    0x24, 0xb1, 0xfc, 0x1c, 0x97, 0xcb
};

ImageAtlas::ImageAtlas(const ResourceId& id)
    : Resource(id, TypeId)
{
    ;
}

void ImageAtlas::AddRegion(const uint32_t& id, const Region& region)
{
    regions_[id] = region;
}

const ImageAtlas::Region ImageAtlas::GetRegion(const uint32_t& id) const
{
    auto itr = regions_.find(id);
    if(itr != regions_.end())
    {
        return itr->second;
    }
    else{
        return ImageAtlas::Region();
    }
}

bool ImageAtlas::ContainsRegion(const uint32_t& id) const
{
    return regions_.count(id) > 0;
}

std::shared_ptr<Resource> ImageAtlas::Load(rapidxml::xml_document<> &doc)
{
    std::shared_ptr<Resource> ret;
    rapidxml::xml_node<> *node = doc.first_node("resource");
    assert(node);

    ResourceId resId = Resource::StringToResourceId(node->first_attribute("uuid")->value());
    ret.reset(new ImageAtlas(resId));

    //TODO: Actually perform the loading of the image atlas here
    return ret;
}
