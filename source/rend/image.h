#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <cstdint>
#include "resource.h"
#include <vector>
#include <memory>
#include <rapidxml.hpp>

//Type UUID: 89d94ad2-a732-49de-8ef5-ca9579c000d3

class Image : public Resource
{
public:
    Image(const ResourceId& id);
    void SetImageData(const std::vector<uint8_t>& data, const uint32_t& width, const uint32_t& height, const uint32_t& channels);
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetChannels() const;
    const std::vector<uint8_t>& GetData() const;

    static const ResourceType TypeId;

    static std::shared_ptr<Resource> Load(rapidxml::xml_document<> &doc);
private:
    uint32_t width_;
    uint32_t height_;
    uint32_t channels_;
    std::vector<uint8_t> data_;
};

#endif
