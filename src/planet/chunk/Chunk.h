#ifndef _OXYBELIS_PLANET_CHUNK_CHUNK_H
#define _OXYBELIS_PLANET_CHUNK_CHUNK_H

#include <cstddef>
#include "graphics/Buffer.h"
#include "graphics/VertexArray.h"
#include "graphics/camera/Camera.h"
#include "graphics/shader/Program.h"
#include "planet/chunk/ChunkId.h"
#include "planet/terragen/TerrainData.h"

class Chunk {
    ChunkLocation loc;
    Lod lod;
    VertexArray vao;
    Buffer terrain;
    size_t vertices;
    Vec3D center;

public:
    size_t vram_usage;

    Chunk(const TerrainData& terrain);

    ChunkId id() const {
        return this->loc.id;
    }

    friend class ShadowRenderer;
    friend class TerrainRenderer;
};

#endif
