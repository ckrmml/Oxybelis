#include "planet/Planet.h"

namespace {
    // Height constants taken from VoxelStorm planet renderer.
    size_t lod_from_alt(double alt_sq) {
        if (alt_sq <= std::pow(4'000.0, 2.0))
            return 7;
        else if (alt_sq <= std::pow(10'000.0, 2.0))
            return 6;
        else if (alt_sq <= std::pow(32'000.0, 2.0))
            return 5;
        else if (alt_sq <= std::pow(60'000.0, 2.0))
            return 4;
        else if (alt_sq <= std::pow(250'000.0, 2.0))
            return 3;
        else if (alt_sq <= std::pow(1'000'000.0, 2.0))
            return 2;
        else if (alt_sq <= std::pow(1'600'000.0, 2.0))
            return 1;
        else
            return 0;
    }
}

Planet::Planet(const Vec3D& translation, const QuatD& rotation, double radius, TerrainGeneratorBase& tg):
    loader(tg),
    patch(NONE),
    pending_patch(NONE),
    translation(translation),
    rotation(rotation),
    radius(radius) {
}
    
void Planet::update(const Camera& cam) {
    auto relative = cam.translation - this->translation;
    double dst = length(relative) - this->radius;
    auto lod = lod_from_alt(dst * dst);
    auto loc = ChunkLocation(relative, lod, this->radius);

    if (((!this->patch || this->patch->center.id != loc.id) && !this->pending_patch) ||
        (this->pending_patch && this->pending_patch->center.id != loc.id)) {
        this->pending_patch = ChunkPatch(relative, lod, this->radius, this->loader);
    }

    if (this->pending_patch && this->pending_patch->update()) {
        this->patch = std::move(this->pending_patch);
        this->pending_patch = NONE;
        this->loader.collect_garbage();
    }
}

bool Planet::has_drawable_terrain() const {
    return this->patch || this->pending_patch;
}
