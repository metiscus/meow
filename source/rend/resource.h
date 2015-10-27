#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

#include <boost/uuid/uuid.hpp>
#include <boost/noncopyable.hpp>

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

private:
    const ResourceId uuid_;
    const ResourceType type_;
};

#endif // RESOURCE_H_INCLUDED
