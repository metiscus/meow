#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED

class Layer
{
public:
    Layer();
    virtual ~Layer() = default;

    inline const uint32_t& GetWidth() const
    {
        return width_;
    }

    inline const uint32_t& GetHeight() const
    {
        return height_;
    }

protected:
    uint32_t width_;
    uint32_t height_;
};

class TileLayer : public Layer
{
public:
    TileLayer() = default;
    const uint32_t& GetXY(const uint32_t& x, const uint32_t& y) const
    {
        return tiles_.at(x + y*width_);
    }

    void SetXY(const uint32_t& x, const uint32_t& y, const uint32_t& id)
    {
        tiles_[x + y*width_] = id;
    }

private:
    std::vector<uint32_t> tiles_;
};

#endif // LAYER_H_INCLUDED
