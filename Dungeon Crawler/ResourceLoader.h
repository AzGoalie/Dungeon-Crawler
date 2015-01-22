#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Game.h"

#include <string>
#include <unordered_map>

template <class T>
struct Resource
{
    std::string filename;
    int references;
    T* resource;
};


// Used to load any resource (Texture, Shaders, Files)
// 'm_Cache' sets if you want to use a cache or not

// TODO: Should probably make all resources inherit from a base resource class
// and call load instead of constructors
template <class T>
class ResourceLoader
{
private:
    std::unordered_map<std::string, Resource<T> > m_ResourceMap;
    
    ResourceLoader(const ResourceLoader&){}
    ResourceLoader &operator = (const ResourceLoader& ) { return *this; }
    
    bool m_Cache;
    
public:
    
    ResourceLoader();
    ~ResourceLoader();
    
    bool Init(bool cache = true);
    void Shutdown();
    
    T* Load(std::string filename);
    void Unload(std::string filename);
    void ReleaseAll();
};

// Implementation because of templates
template <class T>
ResourceLoader<T>::ResourceLoader()
{}

template <class T>
ResourceLoader<T>::~ResourceLoader()
{
    Shutdown();
}

template <class T>
bool ResourceLoader<T>::Init(bool cache)
{
    m_Cache = cache;
    
    return true;
}

template <class T>
void ResourceLoader<T>::Shutdown()
{
    if (!m_ResourceMap.empty())
        ReleaseAll();
}

template <class T>
T* ResourceLoader<T>::Load(std::string filename)
{
    if (filename.empty())
        return nullptr;
    
    typename std::unordered_map< std::string, Resource<T> >::iterator it = m_ResourceMap.find(filename);
    if (it != m_ResourceMap.end() && m_Cache)
    {
        it->second.references++;
        return it->second.resource;
    }
    
    Resource<T> resource;
    resource.references = 1;
    resource.filename = filename;
    resource.resource = new T(filename.c_str());
    m_ResourceMap.insert(std::pair<std::string, Resource<T> > (filename, resource));
    
    return resource.resource;
}

template <class T>
void ResourceLoader<T>::Unload(std::string filename)
{
    if (filename.empty())
        return;
    typename std::unordered_map<std::string, Resource<T> >::iterator it = m_ResourceMap.find(filename);
    if (it != m_ResourceMap.end())
    {
        it->second.references--;
        if (it->second.references == 0)
        {
            delete it->second.resource;
            m_ResourceMap.erase(it);
        }
    }
    
}

template <class T>
void ResourceLoader<T>::ReleaseAll()
{
    typename std::unordered_map< std::string, Resource<T> >::iterator it = m_ResourceMap.begin();
    
    while (it != m_ResourceMap.end())
    {
        delete it->second.resource;
        it = m_ResourceMap.erase(it);
    }
}

#endif
