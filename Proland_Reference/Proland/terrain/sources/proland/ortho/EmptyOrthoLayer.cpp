#include "proland/ortho/EmptyOrthoLayer.h"

#include "ork/resource/ResourceTemplate.h"
#include "ork/render/FrameBuffer.h"
#include "ork/scenegraph/SceneManager.h"

using namespace std;
using namespace ork;

namespace proland
{

EmptyOrthoLayer::EmptyOrthoLayer() :
    TileLayer("EmptyOrthoLayer")
{
}

EmptyOrthoLayer::EmptyOrthoLayer(vec4f color) :
    TileLayer("EmptyOrthoLayer")
{
    TileLayer::init(false);
    init(color);
}

void EmptyOrthoLayer::init(vec4f color)
{
    this->color = color;
}

EmptyOrthoLayer::~EmptyOrthoLayer()
{
}

bool EmptyOrthoLayer::doCreateTile(int level, int tx, int ty, TileStorage::Slot *data)
{
    if (Logger::DEBUG_LOGGER != NULL) {
        ostringstream oss;
        oss << "Empty tile " << getProducerId() << " " << level << " " << tx << " " << ty;
        Logger::DEBUG_LOGGER->log("ORTHO", oss.str());
    }

    ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();
    fb->setClearColor(color);
    fb->clear(true, false, false);
    fb->setClearColor(vec4f::ZERO);

    return true;
}

void EmptyOrthoLayer::swap(ptr<EmptyOrthoLayer> p)
{
    TileLayer::swap(p);
    std::swap(color, p->color);
}

class EmptyOrthoLayerResource : public ResourceTemplate<40, EmptyOrthoLayer>
{
public:
    EmptyOrthoLayerResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc,
            const TiXmlElement *e = NULL) :
        ResourceTemplate<40, EmptyOrthoLayer> (manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        vec4f color = vec4f(1.0f, 1.0f, 1.0f, 1.0f);

        checkParameters(desc, e, "name,color,");

        if (e->Attribute("color") != NULL) {
            string c = getParameter(desc, e, "color") + ",";
            string::size_type start = 0;
            string::size_type index;
            for (int i = 0; i < 3; i++) {
                index = c.find(',', start);
                color[i] = (float) atof(c.substr(start, index - start).c_str()) / 255;
                start = index + 1;
            }
        }

        init(color);
    }

    virtual bool prepareUpdate()
    {
        oldValue = NULL;
        newDesc = NULL;

        return true;
    }
};

extern const char emptyOrthoLayer[] = "emptyOrthoLayer";

static ResourceFactory::Type<emptyOrthoLayer, EmptyOrthoLayerResource> EmptyOrthoLayerType;

}
