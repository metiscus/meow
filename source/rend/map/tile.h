#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class Tile
{
public:
    Tile() = default;

    void AddProperty(const std::string& name, const std::string& value);
    bool HasProperty(const std::string& name) const;
    std::string GetProperty(const std::string& name) const;
    std::vector<std::string> GetPropertyList() const;

    static Tile Load(rapidxml::xml_document<> &doc);
private:
    std::map<std::string, std::string> properties_;
};

#endif // TILE_H_INCLUDED
