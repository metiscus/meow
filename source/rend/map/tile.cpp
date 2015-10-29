#include "tile.h"

void Tile::AddProperty(const std::string& name, const std::string& value)
{
    properties_[name] = value;
}

bool Tile::HasProperty(const std::string& name) const
{
    return properties_.count(name) > 0;
}

std::string Tile::GetProperty(const std::string& name) const
{
    auto itr = properties_.find(name);
    if(itr != properties_.end())
    {
        return itr->second;
    }
    return std::string("");
}

std::vector<std::string> Tile::GetPropertyList() const
{
    std::vector<std::string> ret;
    for(auto itr : properties_)
    {
        ret.push_back(itr.first);
    }
    return ret;
}
