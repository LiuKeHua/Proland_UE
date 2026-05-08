#ifndef _PROLAND_ELEVATION_PRODUCER_H_
#define _PROLAND_ELEVATION_PRODUCER_H_

#include "proland/producer/TileProducer.h"

namespace proland
{

/**
 * A TileProducer to create elevation tiles on CPU from CPU residual tiles.
 * @ingroup dem
 * @authors Antoine Begault, Eric Bruneton
 */
class PROLAND_TERRAIN_API  CPUElevationProducer : public TileProducer
{
public:
    /**
     * Creates a new CPUElevationProducer.
     *
     * @param cache the cache to store the produced tiles. The underlying
     *      storage must be a CPUTileStorage of float type.
     * @param residualTiles the %producer producing the residual tiles. This
     *      %producer should produce its tiles in a CPUTileStorage of float type.
     *      The size of the residual tiles (without borders) must be a multiple
     *      of the size of the elevation tiles (without borders).
     */
    CPUElevationProducer(ptr<TileCache> cache, ptr<TileProducer> residualTiles);

    /**
     * Deletes this CPUElevationProducer.
     */
    virtual ~CPUElevationProducer();

    virtual void getReferencedProducers(std::vector< ptr<TileProducer> > &producers) const;

    virtual void setRootQuadSize(float size);

    virtual int getBorder();

    virtual bool prefetchTile(int level, int tx, int ty);

    /**
     * Returns the %terrain altitude at a given point, at a given level.
     * The corresponding tile must be in cache before calling this method.
     *
     * @param producer a CPUElevationProducer or an equivalent (i.e. a %
     *      producer using an underlying CPUTileStorage of float type).
     * @param level level at which we want to get the altitude.
     * @param x physical x coordinate of the point to get (in meters from the %terrain center).
     * @param y physical y coordinate of the point to get (in meters from the %terrain center).
     */
    static float getHeight(ptr<TileProducer> producer, int level, float x, float y);

protected:
    /**
     * Creates an uninitialized CPUElevationProducer.
     */
    CPUElevationProducer();

    /**
     * Initializes this CPUElevationProducer.
     *
     * @param cache the cache to store the produced tiles. The underlying
     *      storage must be a CPUTileStorage of float type.
     * @param residualTiles the %producer producing the residual tiles. This
     *      %producer should produce its tiles in a CPUTileStorage of float type.
     *      The size of the residual tiles (without borders) must be a multiple
     *      of the size of the elevation tiles (without borders).
     */
    void init(ptr<TileCache> cache, ptr<TileProducer> residualTiles);

    virtual ptr<Task> startCreateTile(int level, int tx, int ty, unsigned int deadline, ptr<Task> task, ptr<TaskGraph> owner);

    virtual void beginCreateTile();

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

    virtual void endCreateTile();

    virtual void stopCreateTile(int level, int tx, int ty);

    virtual void swap(ptr<CPUElevationProducer> p);

private:
    /**
     * The %producer producing the residual tiles. This %producer should produce
     * its tiles in a CPUTileStorage of float type. The size of the residual tiles
     * (without borders) must be a multiple of the size of the elevation tiles
     * (without borders).
     */
    ptr<TileProducer> residualTiles;
};

}

#endif
