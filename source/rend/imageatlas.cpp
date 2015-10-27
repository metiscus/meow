#include "imageatlas.h"
#include <sstream>

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

ResourceId ImageAtlas::GetImageId() const
{
    return image_;
}

std::shared_ptr<Resource> ImageAtlas::Load(rapidxml::xml_document<> &doc)
{
    std::shared_ptr<Resource> ret;
    rapidxml::xml_node<> *resourceNode = doc.first_node("resource");
    assert(resourceNode);

    ResourceId resId = Resource::StringToResourceId(resourceNode->first_attribute("uuid")->value());
    ImageAtlas* pAtlas = new ImageAtlas(resId);
    ret.reset(pAtlas);

    for(rapidxml::xml_node<> *region = resourceNode->first_node("region"); region!=nullptr; region = region->next_sibling("region"))
    {
        rapidxml::xml_attribute<> *attribute = region->first_attribute("id");
        uint32_t regionId = 0;

        std::stringstream str;
        str<<attribute->value();
        str>>regionId;

        str.str("");
        str.clear();
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;
        str<<region->value();
        str>>x;
        str>>y;
        str>>z;
        str>>w;
        Region r = std::make_tuple(x, y, z, w);
        pAtlas->regions_.insert(std::make_pair(regionId, r));

        // Find referenced image
        rapidxml::xml_node<> *reference = resourceNode->first_node("reference");
        if(reference)
        {
            pAtlas->image_ = Resource::StringToResourceId(resourceNode->first_attribute("uuid")->value());
        }
    }
    return ret;
}
