#ifndef _PROLAND_EMPTYORTHOLAYER_H_
#define _PROLAND_EMPTYORTHOLAYER_H_

#include "ork/math/vec4.h"
#include "proland/producer/TileLayer.h"

using namespace ork;

namespace proland
{

/**
 * An OrthoGPUProducer layer that simply fills tiles with a constant color.
 * This layer is useful when the OrthoGPUProducer is used without an
 * OrthoCPUProducer. It can provide a background color for other layers.
 * @ingroup ortho
 * @authors Eric Bruneton, Antoine Begault
 */
class PROLAND_TERRAIN_API  EmptyOrthoLayer : public TileLayer
{
public:
    /**
     * Creates a new EmptyOrthoLayer.
     *
     * @param color the color to be used to fill the produced tiles.
     */
    EmptyOrthoLayer(vec4f color);

    /**
     * Deletes this EmptyOrthoLayer.
     */
    virtual ~EmptyOrthoLayer();

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

protected:
    /**
     * Creates an uninitialized EmptyOrthoLayer.
     */
    EmptyOrthoLayer();

    /**
     * Initializes this EmptyOrthoLayer.
     *
     * @param color the color to be used to fill the produced tiles.
     */
    void init(vec4f color);

    virtual void swap(ptr<EmptyOrthoLayer> p);

private:
    /**
     * The color to be used to fill the produced tiles.
     */
    vec4f color;
};

}

#endif
