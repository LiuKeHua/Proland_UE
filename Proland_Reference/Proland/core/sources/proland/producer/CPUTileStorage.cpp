#include "proland/producer/CPUTileStorage.h"

#include "ork/resource/ResourceTemplate.h"

using namespace std;
using namespace ork;

namespace proland
{

template<class T>
class CPUTileStorageResource : public ResourceTemplate<0, CPUTileStorage<T> >
{
public:
    CPUTileStorageResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, CPUTileStorage<T> >(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        int tileSize;
        int channels;
        int capacity;
        Resource::checkParameters(desc, e, "name,tileSize,channels,capacity,");
        Resource::getIntParameter(desc, e, "tileSize", &tileSize);
        Resource::getIntParameter(desc, e, "channels", &channels);
        Resource::getIntParameter(desc, e, "capacity", &capacity);
        CPUTileStorage<T>::init(tileSize, channels, capacity);
    }
};

extern const char cpuByteTileStorage[] = "cpuByteTileStorage";

extern const char cpuFloatTileStorage[] = "cpuFloatTileStorage";

static ResourceFactory::Type<cpuByteTileStorage, CPUTileStorageResource<unsigned char> > CPUByteTileStorageType;

static ResourceFactory::Type<cpuFloatTileStorage, CPUTileStorageResource<float> > CPUFloatTileStorageType;

}
