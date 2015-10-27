#include "image.h"
#include <cassert>
#include <boost/log/trivial.hpp>
#include <boost/uuid/string_generator.hpp>

const ResourceType Image::TypeId =
{
    0x89, 0xd9, 0x4a, 0xd2,
    0xa7, 0x32,
    0x49, 0xde,
    0x8e, 0xf5,
    0xca, 0x95, 0x79, 0xc0, 0x00, 0xd3
};

Image::Image(const ResourceId& id)
    : Resource(id, TypeId)
    , width_(0U)
    , height_(0U)
    , channels_(0U)
{
    ;
}

void Image::SetImageData(const std::vector<uint8_t>& data, const uint32_t& width, const uint32_t& height, const uint32_t& channels)
{
    data_ = data;
    width_ = width;
    height_ = height;
    channels_ = channels;
}

uint32_t Image::GetWidth() const
{
    return width_;
}

uint32_t Image::GetHeight() const
{
    return height_;
}

uint32_t Image::GetChannels() const
{
    return channels_;
}

const std::vector<uint8_t>& Image::GetData() const
{
    return data_;
}

std::shared_ptr<Resource> Image::Load(rapidxml::xml_document<> &doc)
{
    std::shared_ptr<Resource> ret;
    rapidxml::xml_node<> *node = doc.first_node("resource");
    assert(node);

    ResourceId resId = Resource::StringToResourceId(node->first_attribute("uuid")->value());
    ret.reset(new Image(resId));

    //TODO: Actually perform the loading of the image file here
    return ret;
}
