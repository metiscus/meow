#include "image.h"

const ResourceType Image::TypeId =
{
    0x89, 0xd9, 0x4a, 0xd2,
    0xa7, 0x32,
    0x49, 0xde,
    0x8e, 0xf5,
    0xca, 0x95, 0x79, 0xc0, 0x00, 0xd3
};

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
