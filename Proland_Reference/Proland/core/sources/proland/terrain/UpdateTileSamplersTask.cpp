#include "proland/terrain/UpdateTileSamplersTask.h"

#include "ork/resource/ResourceTemplate.h"
#include "proland/terrain/TileSampler.h"

using namespace std;

namespace proland
{

UpdateTileSamplersTask::UpdateTileSamplersTask() : AbstractTask("UpdateTileSamplersTask")
{
}

UpdateTileSamplersTask::UpdateTileSamplersTask(const QualifiedName &terrain) :
    AbstractTask("UpdateTileSamplersTask")
{
    init(terrain);
}

void UpdateTileSamplersTask::init(const QualifiedName &terrain)
{
    this->terrain = terrain;
}

UpdateTileSamplersTask::~UpdateTileSamplersTask()
{
}

ptr<Task> UpdateTileSamplersTask::getTask(ptr<Object> context)
{
    ptr<SceneNode> n = context.cast<Method>()->getOwner();
    ptr<SceneNode> target = terrain.getTarget(n);
    ptr<TerrainNode> t = NULL;
    if (target == NULL) {
        t = n->getOwner()->getResourceManager()->loadResource(terrain.name).cast<TerrainNode>();
    } else {
        t = target->getField(terrain.name).cast<TerrainNode>();
    }
    if (t == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("TERRAIN", "UpdateTileSamplers : cannot find terrain '" + terrain.target + "." + terrain.name + "'");
        }
        throw exception();
    }
    ptr<TaskGraph> result = new TaskGraph();
    SceneNode::FieldIterator i = n->getFields();
    while (i.hasNext()) {
        ptr<TileSampler> u = i.next().cast<TileSampler>();
        if (u != NULL) {
            ptr<Task> ut = u->update(n->getOwner(), t->root);
            if (ut.cast<TaskGraph>() == NULL || !ut.cast<TaskGraph>()->isEmpty()) {
                result->addTask(ut);
            }
        }
    }
    return result;
}

void UpdateTileSamplersTask::swap(ptr<UpdateTileSamplersTask> t)
{
    std::swap(*this, *t);
}

class UpdateTileSamplersTaskResource : public ResourceTemplate<40, UpdateTileSamplersTask>
{
public:
    UpdateTileSamplersTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, UpdateTileSamplersTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,");
        string n = getParameter(desc, e, "name");
        init(QualifiedName(n));
    }
};

extern const char updateTileSamplers[] = "updateTileSamplers";

static ResourceFactory::Type<updateTileSamplers, UpdateTileSamplersTaskResource> UpdateTileSamplersTaskType;

}
