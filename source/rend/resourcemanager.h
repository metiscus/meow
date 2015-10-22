#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>
#include <map>
#include <vector>

class Resource;

typedef boost::uuids::uuid ResourceId;

struct ResourceLoader
{

};

class ResourceManager
{
public:
    static void CreateInstance();
    static void DestroyInstance();
    static ResourceManager& GetInstance();

    bool LoadResource(ResourceId resource);
    std::shared_ptr<Resource> GetResource(ResourceId resource);

private:
    std::string resourcePath_;

    std::map<ResourceId, std::string> resourceFiles_;
    std::map<ResourceId, std::shared_ptr<Resource> > resources_;

    ResourceManager();
    ~ResourceManager();

    void UpdateResources();
    void CacheResourceId(const std::string& filepath);
    std::string FileToString(const std::string& filename);
    bool LoadResourceFile(const std::string& filename);
};

extern "C"
{
    void test_resourcemgr();
}

#endif // RESOURCEMANAGER_H_INCLUDED
