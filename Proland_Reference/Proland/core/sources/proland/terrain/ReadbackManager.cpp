#include "proland/terrain/ReadbackManager.h"

namespace proland
{

const size_t BUFFER_ALIGNMENT = 32;

ReadbackManager::ReadbackManager(int maxReadbackPerFrame, int readbackDelay, int bufferSize) :
    Object("ReadbackManager"),
    maxReadbackPerFrame(maxReadbackPerFrame),
    readbackDelay(readbackDelay),
    bufferSize(bufferSize)
{
    readCount = new int[readbackDelay];
    toRead = new ptr<GPUBuffer>*[readbackDelay];
    toReadCallbacks = new ptr<Callback>*[readbackDelay];

    for (int i = 0; i < readbackDelay; ++i) {
        readCount[i] = 0;
        toRead[i] = new ptr<GPUBuffer>[maxReadbackPerFrame];
        toReadCallbacks[i] = new ptr<Callback>[maxReadbackPerFrame];
        for (int j = 0; j < maxReadbackPerFrame; ++j) {
            toRead[i][j] = new GPUBuffer();
            toRead[i][j]->setData(bufferSize, NULL, STREAM_READ);
        }
    }
}

ReadbackManager::~ReadbackManager()
{
    for (int i = 0; i < readbackDelay; ++i) {
        delete[] toRead[i];
        delete[] toReadCallbacks[i];
    }
    delete[] toRead;
    delete[] toReadCallbacks;
    delete[] readCount;
}

bool ReadbackManager::canReadback()
{
    return readCount[0] < maxReadbackPerFrame;
}

bool ReadbackManager::readback(ptr<FrameBuffer> fb, int x, int y, int w, int h, TextureFormat f, PixelType t, ptr<Callback> cb)
{
    if (readCount[0] < maxReadbackPerFrame) {
        int index = readCount[0];
        fb->readPixels(x, y, w, h, f, t, Buffer::Parameters(), *(toRead[0][index]));
        toReadCallbacks[0][index] = cb;
        ++readCount[0];
        return true;
    } else {
        assert(false); // should not happen, call canReadback before
        return false;
    }
}

void ReadbackManager::newFrame()
{
    int lastIndex = readbackDelay - 1;
    for (int i = 0; i < readCount[lastIndex]; ++i) {
        BufferAccess a = READ_ONLY;
        volatile void *data = toRead[lastIndex][i]->map(a);
        toReadCallbacks[lastIndex][i]->dataRead(data);
        toReadCallbacks[lastIndex][i] = NULL;
        toRead[lastIndex][i]->unmap();
    }

    // rotate buffer to the left and clear readCount
    ptr<GPUBuffer> *bufs = toRead[lastIndex];
    ptr<Callback> *calls = toReadCallbacks[lastIndex];

    for (int i = readbackDelay - 1; i > 0; --i) {
        readCount[i] = readCount[i - 1];
        toRead[i] = toRead[i - 1];
        toReadCallbacks[i] = toReadCallbacks[i - 1];
    }
    readCount[0] = 0;
    toRead[0] = bufs;
    toReadCallbacks[0] = calls;
}

}
