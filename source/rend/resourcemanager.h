#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <rapidxml.hpp>

class Resource;

typedef boost::uuids::uuid ResourceId;
typedef boost::uuids::uuid ResourceType;

struct ResourceLoader
{
    ResourceType type;

    std::shared_ptr<Resource> (*load_fun)(rapidxml::xml_document<> &doc);
};

class ResourceManager
{
public:
    static void CreateInstance();
    static void DestroyInstance();
    static ResourceManager& GetInstance();

    bool LoadResource(ResourceId resource);
    std::shared_ptr<Resource> GetResource(ResourceId resource);

    void AddResourceLoader(const ResourceLoader& loader);

private:
    std::string resourcePath_;

    std::map<ResourceId, std::string> resourceFiles_;
    std::map<ResourceId, std::shared_ptr<Resource> > resources_;
    std::map<ResourceType, ResourceLoader> loaders_;

    ResourceManager();
    ~ResourceManager();

    void UpdateResources();
    void CacheResourceId(const std::string& filepath);
    std::string FileToString(const std::string& filename);
    bool LoadResourceFile(ResourceId id, const std::string& filename);
};

extern "C"
{
    void test_resourcemgr();
}

#endif // RESOURCEMANAGER_H_INCLUDED
