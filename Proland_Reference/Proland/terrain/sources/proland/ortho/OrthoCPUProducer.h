#ifndef _PROLAND_ORTHO_CPU_PRODUCER_H_
#define _PROLAND_ORTHO_CPU_PRODUCER_H_

#include <string>

#include "proland/producer/TileProducer.h"

namespace proland
{

/**
 * @defgroup ortho ortho
 * Provides producers and layers for %terrain textures.
 * @ingroup proland
 */

/**
 * A TileProducer to load any kind of texture tile from disk to CPU memory.
 * See \ref sec-orthocpu.
 * @ingroup ortho
 * @authors Eric Bruneton, Antoine Begault
 */
class PROLAND_TERRAIN_API  OrthoCPUProducer : public TileProducer
{
public:
    /**
     * Creates a new OrthoCPUProducer.
     *
     * @param cache the cache to store the produced tiles. The underlying
     *      storage must be a CPUTileStorage of unsigned char type. The size
     *      of tiles in this storage size must be equal to the size of the
     *      tiles stored on disk, borders included.
     * @param name the name of the file containing the tiles to load.
     */
    OrthoCPUProducer(ptr<TileCache> cache, const char *name);

    /**
     * Deletes this OrthoCPUProducer.
     */
    virtual ~OrthoCPUProducer();

    virtual int getBorder();

    virtual bool hasTile(int level, int tx, int ty);

    /**
     * Returns true if the produced tiles are compressed in DXT format.
     */
    bool isCompressed();

protected:
    /**
     * Creates an uninitialized OrthoCPUProducer.
     */
    OrthoCPUProducer();

    /**
     * Initializes this OrthoCPUProducer.
     *
     * @param cache the cache to store the produced tiles. The underlying
     *      storage must be a CPUTileStorage of unsigned char type. The size
     *      of tiles in this storage size must be equal to the size of the
     *      tiles stored on disk, borders included.
     * @param name the name of the file containing the tiles to load.
     */
    virtual void init(ptr<TileCache> cache, const char *name);

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

    virtual void swap(ptr<OrthoCPUProducer> p);

private:
    /**
     * The name of the file containing the residual tiles to load.
     */
    std::string name;

    /**
     * The number of components per pixel in the tiles to load.
     */
    int channels;

    /**
     * The size of the tiles to load, without borders. A tile contains
     * (tileSize+4)*(tileSize+4)*channels samples.
     */
    int tileSize;

    /**
     * The size in pixels of the border around each tile. A tile contains
     * (tileSize+4)*(tileSize+4)*channels samples.
     */
    int border;

    /**
     * The maximum level of the stored tiles on disk (inclusive).
     */
    int maxLevel;

    /**
     * true if the produced tiles are compressed in DXT format.
     */
    bool dxt;

    /**
     * Offset of the first stored tile on disk. The offsets indicated in
     * the tile offsets array #offsets are relative to this offset.
     */
    int header;

    /**
     * The offsets of each tile on disk, relatively to #offset, for each
     * tile id (see #getTileId).
     */
    long long* offsets;

    /**
     * A mutex used to serializes accesses to the file storing the tiles.
     */
    void *mutex;

    /**
     * The file storing the tiles on disk.
     */
    FILE *tileFile;

    /**
     * A key to store thread specific buffers used to produce the tiles.
     */
    static void *key;

    /**
     * Returns the id of the given tile. This id is used to find the offset
     * the tile data on disk, using #offsets.
     *
     * @param level the level of the tile.
     * @param tx the logical x coordinate of the tile.
     * @param ty the logical y coordinate of the tile.
     * @return the id of the given tile.
     */
    int getTileId(int level, int tx, int ty);
};

}

#endif
