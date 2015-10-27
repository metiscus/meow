#ifndef IMAGEATLAS_H_INCLUDED
#define IMAGEATLAS_H_INCLUDED

#include <cstdint>
#include "resource.h"
#include <tuple>
#include <map>
#include <memory>
#include <rapidxml.hpp>

//Type UUID: ce2f105b-2538-43ea-9eb0-24b1fc1c97cb

class ImageAtlas : public Resource
{
public:
    typedef std::tuple<float, float, float, float> Region;

    ImageAtlas(const ResourceId& id);

    void AddRegion(const uint32_t& id, const Region& region);
    const Region GetRegion(const uint32_t& id) const;
    bool ContainsRegion(const uint32_t& id) const;

    ResourceId GetImageId() const;

    static const ResourceType TypeId;

    static std::shared_ptr<Resource> Load(rapidxml::xml_document<> &doc);
private:
    std::map<uint32_t, Region> regions_;
    ResourceId image_;
};


#endif // IMAGEATLAS_H_INCLUDED
