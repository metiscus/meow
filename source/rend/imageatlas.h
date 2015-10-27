#ifndef IMAGEATLAS_H_INCLUDED
#define IMAGEATLAS_H_INCLUDED

#include <cstdint>
#include "resource.h"
#include <tuple>
#include <map>

//Type UUID: ce2f105b-2538-43ea-9eb0-24b1fc1c97cb

class ImageAtlas : public Resource
{
public:
    typedef std::tuple<float, float, float, float> Region;

    ImageAtlas(const ResourceId& id);

    void AddRegion(const uint32_t& id, const Region& region);
    const Region& GetRegion(const uint32_t& id) const;
    bool ContainsRegion(const uint32_t& id) const;

    static const ResourceType TypeId;

private:
    std::map<uint32_t, Region> regions_;

    static std::shared_ptr<Resource> Load(rapidxml::xml_document<> &doc);
};


#endif // IMAGEATLAS_H_INCLUDED
