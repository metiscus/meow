#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

#include <cassert>
#include <boost/noncopyable.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>

typedef boost::uuids::uuid ResourceId;
typedef boost::uuids::uuid ResourceType;

class Resource : public boost::noncopyable
{
public:
    Resource(const ResourceId& uuid, const ResourceType& type)
        : uuid_(uuid)
        , type_(type)
        {}

    virtual ~Resource() = default;

    inline const ResourceId GetResourceId() const
    {
        return uuid_;
    }

    inline const ResourceType GetResourceType() const
    {
        return type_;
    }

    static ResourceId StringToResourceId(const char* ptr)
    {
        assert(ptr);
        boost::uuids::string_generator generator;
        return generator(ptr);
    }

    static ResourceType StringToResourceType(const char* ptr)
    {
        assert(ptr);
        boost::uuids::string_generator generator;
        return generator(ptr);
    }

private:
    const ResourceId uuid_;
    const ResourceType type_;
};

#endif // RESOURCE_H_INCLUDED
