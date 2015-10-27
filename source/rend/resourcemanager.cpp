#include "resourcemanager.h"

#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/log/trivial.hpp>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <rapidxml.hpp>

static ResourceManager* instance = nullptr;

void ResourceManager::CreateInstance()
{
    if(!instance)
    {
        instance = new ResourceManager();
        BOOST_LOG_TRIVIAL(trace)<<"ResourceManager::CreateInstance() : " << (void*)instance;
    }
}

void ResourceManager::DestroyInstance()
{
    BOOST_LOG_TRIVIAL(trace)<<"ResourceManager::DestroyInstance() : " << (void*)instance;
    delete instance;
    instance = nullptr;
}

ResourceManager& ResourceManager::GetInstance()
{
    return *instance;
}

ResourceManager::ResourceManager()
{
    resourcePath_ = "data";

    char *resourcePathEnv = getenv("RESOURCE_PATH");
    if(resourcePathEnv)
    {
        resourcePath_ = resourcePathEnv;
    }

    UpdateResources();
}

ResourceManager::~ResourceManager()
{
    ;
}

bool ResourceManager::LoadResource(ResourceId id)
{
    auto fileMapItr = resourceFiles_.find(id);
    if(fileMapItr == resourceFiles_.end())
    {
        BOOST_LOG_TRIVIAL(warning)<<"ResourceManager::LoadResource invalid resource id: "<< boost::lexical_cast<std::string>(id);
        return false;
    }
    else
    {
        auto resourceItr = resources_.find(id);
        if(resourceItr!=resources_.end())
        {
            return true;
        }
        else
        {
            //Load the resource file
            return LoadResourceFile(id, fileMapItr->second);
        }
    }

    return true;
}

std::shared_ptr<Resource> ResourceManager::GetResource(ResourceId id)
{
    std::shared_ptr<Resource> res;
    auto itr = resources_.find(id);
    if(itr == resources_.end())
    {
        BOOST_LOG_TRIVIAL(warning)<<"ResourceManager::GetResource invalid resource id: "<< boost::lexical_cast<std::string>(id);
    }
    else
    {
        res = itr->second;
    }
    return res;
}

void ResourceManager::AddResourceLoader(const ResourceLoader& loader)
{
    loaders_.insert(std::make_pair(loader.type, loader));
}

void ResourceManager::UpdateResources()
{
    resourceFiles_.clear();

    BOOST_LOG_TRIVIAL(trace)<<"ResourceManager::UpdateResources "<<resourcePath_;
    boost::filesystem::path path(resourcePath_);
    if(boost::filesystem::exists(path))
    {
        // iterate the resource path looking for resource files
        std::vector<boost::filesystem::path> files;
        std::copy(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(), std::back_inserter(files));

        for(auto p : files)
        {
            BOOST_LOG_TRIVIAL(trace)<<"ResourceManager::UpdateResources :"<<p.string();
            if(boost::filesystem::is_regular_file(p) && p.extension() == std::string(".xml"))
            {
                CacheResourceId(p.string());
            }
        }
    }
}

bool ResourceManager::LoadResourceFile(ResourceId id, const std::string& filename)
{
    ///\TODO: Add some better error checking / reporting in here
    std::string file = FileToString(filename);
    char *fileCStr = strdup(file.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(fileCStr);

    bool ret = false;
    rapidxml::xml_node<> *node = doc.first_node("resource");
    BOOST_LOG_TRIVIAL(trace)<<"Loading file "<<filename<<" for resource "<<boost::lexical_cast<std::string>(id);
    if(node)
    {
        for(rapidxml::xml_node<> *dependency = node->first_node("reference"); dependency!=nullptr; dependency = dependency->next_sibling("reference"))
        {
            rapidxml::xml_attribute<> *referenceIdNode = dependency->first_attribute("uuid");
            ResourceId referenceId = Resource::StringToResourceId(referenceIdNode->value());
            if(!IsResourceLoaded(referenceId))
            {
                BOOST_LOG_TRIVIAL(trace)<<"Loading resource "<<boost::lexical_cast<std::string>(referenceId)<<" as dependency";
                if(!LoadResource(referenceId))
                {
                    BOOST_LOG_TRIVIAL(error)<<"Unable to load " << boost::lexical_cast<std::string>(referenceId) << " as dependency for "
                        << boost::lexical_cast<std::string>(id);
                    free(fileCStr);
                    return false;
                }
            }
        }

        rapidxml::xml_attribute<> *attr = node->first_attribute("type");
        if(attr)
        {
            ResourceType type = Resource::StringToResourceType(attr->value());
            auto itr = loaders_.find(type);
            if(itr != loaders_.end())
            {
                std::shared_ptr<Resource> resource = itr->second.load_fun(doc);
                if(!!resource)
                {
                    ret = true;
                    resources_[id] = resource;
                }
            }
        }
        else
        {
            //\TODO: add error reporting
        }
    }
    else
    {
        //\TODO: add error reporting
    }
    free(fileCStr);

    return ret;
}

bool ResourceManager::IsResourceLoaded(ResourceId id) const
{
    auto itr = resources_.find(id);
    if(itr == resources_.end())
    {
        return false;
    }
    return true;
}

std::string ResourceManager::FileToString(const std::string& filename) const
{
    std::string file;
    std::ifstream in(filename);
    while(in.is_open() && !in.eof())
    {
        std::string line;
        std::getline(in, line);
        file += line + "\n";
    }
    in.close();
    return file;
}

void ResourceManager::CacheResourceId(const std::string& filepath)
{
    std::string file = FileToString(filepath);
    rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_non_destructive>(&file[0]);
    rapidxml::xml_node<> *node = doc.first_node("resource");
    if(!node)
    {
        return;
    }

    rapidxml::xml_attribute<> *attr = node->first_attribute("uuid");
    if(!attr)
    {
        return;
    }

    std::string uuidStr(attr->value(), attr->value_size());
    ResourceId uuid = Resource::StringToResourceId(uuidStr.c_str());
    resourceFiles_.insert(std::make_pair(uuid, filepath));

    BOOST_LOG_TRIVIAL(trace)<<"ResourceManager::CacheResourceId: "<<uuidStr<<":"<<filepath;
}

extern "C"
{
    ResourceLoader nullLoader;

    void test_resourcemgr()
    {
        ResourceManager::CreateInstance();
        ResourceId id;
        try {
            id = Resource::StringToResourceId("8eb401f0-78ed-11e5-8bcf-feff819cdc9f");

            nullLoader.type = Resource::StringToResourceType("ce2f105b-2538-43ea-9eb0-24b1fc1c97cb");
            nullLoader.load_fun = [](rapidxml::xml_document<> &doc) -> std::shared_ptr<Resource>
            {
                BOOST_LOG_TRIVIAL(trace)<<"Null Resource Loader";
                return std::shared_ptr<Resource>();
            };
        }
        catch(...)
        {
        }
        ResourceManager::GetInstance().AddResourceLoader(nullLoader);
        ResourceManager::GetInstance().LoadResource(id);
        ResourceManager::DestroyInstance();
    }
}

namespace rapidxml
{
    void parse_error_handler(char const* str, void* ptr)
    {
        fprintf(stderr, "[rapidxml] : %s", str);
    }
}
