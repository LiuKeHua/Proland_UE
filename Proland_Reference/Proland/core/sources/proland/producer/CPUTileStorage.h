#ifndef _PROLAND_CPU_TILE_STORAGE_H_
#define _PROLAND_CPU_TILE_STORAGE_H_

#include "proland/producer/TileStorage.h"

namespace proland
{

/**
 * A TileStorage that store tiles on CPU.
 * @ingroup producer
 * @authors Eric Bruneton, Antoine Begault
 *
 * @tparam T the type of each tile pixel component (e.g. char, float, etc).
 */
template<class T>
class CPUTileStorage : public TileStorage
{
public:
    /**
     * A slot managed by a CPUTileStorage.
     */
    class PROLAND_CORE_API CPUSlot : public Slot
    {
    public:
        /**
         * The data of the tile stored in this slot.
         */
        T* data;

        /**
         * The number of elements in the data array.
         */
        int size;

        /**
         * Creates a new CPUSlot. This constructor creates a new array to store
         * the tile data.
         *
         * @param owner the TileStorage that manages this slot.
         * @param size the number of elements in the data array.
         */
        CPUSlot(TileStorage *owner, int size) : Slot(owner)
        {
            this->data = new T[size];
            this->size = size;
        }

        /**
         * Deletes this CPUSlot. This deletes the #data array.
         */
        virtual ~CPUSlot()
        {
            if (data != NULL) {
                delete[] data;
            }
        }
    };

    /**
     * Creates a new CPUTileStorage.
     *
     * @param tileSize the size in pixels of each (square) tile.
     * @param channels the number of components per pixel of each tile. Each
     *      component is of type T.
     * @param capacity the number of slots managed by this tile storage.
     */
    CPUTileStorage(int tileSize, int channels, int capacity) : TileStorage()
    {
        init(tileSize, channels, capacity);
    }

    /**
     * Deletes this CPUTileStorage.
     */
    virtual ~CPUTileStorage()
    {
    }

    /**
     * Returns the number of components per pixel of each tile. Each component
     * is of type T.
     */
    int getChannels()
    {
        return channels;
    }

protected:
    /**
     * Creates an uninitialized CPUTileStorage.
     */
    CPUTileStorage() : TileStorage()
    {
    }

    /**
     * Initializes this CPUTileStorage.
     *
     * @param tileSize the size in pixels of each (square) tile.
     * @param channels the number of components per pixel of each tile. Each
     *      component is of type T.
     * @param capacity the number of slots managed by this tile storage.
     */
    void init(int tileSize, int channels, int capacity)
    {
        TileStorage::init(tileSize, capacity);
        this->channels = channels;
        int size = tileSize * tileSize * channels;
        for (int i = 0; i < capacity; ++i) {
            freeSlots.push_back(new CPUSlot(this, size));
        }
    }

    void swap(ptr<CPUTileStorage<T> > t)
    {
    }

private:
    /**
     * The number of components per pixel of each tile.
     */
    int channels;
};

}

#endif
