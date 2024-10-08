#ifndef _OXYBELIS_PLANET_RENDER_ATMOSPHERERENDERER_H
#define _OXYBELIS_PLANET_RENDER_ATMOSPHERERENDERER_H

#include <memory>
#include "atmosphere/model.h"
#include "graphics/shader/Program.h"
#include "graphics/camera/Camera.h"
#include "graphics/models/Quad.h"
#include "math/Mat.h"
#include "planet/render/RenderInfo.h"

class AtmosphereRenderer {
    std::unique_ptr<atmosphere::Model> model;

    Program shader;
    Uniform inv_proj_mat;
    Uniform model_mat;
    Uniform camera_origin;
    Uniform camera_up;
    Uniform camera_dir;

    Quad quad;

public:
    AtmosphereRenderer(GLuint color_tex, GLuint nd_tex, GLuint dndz_tex, GLuint zminmax_tex, GLuint skybox_tex, GLdouble radius, double outer_radius);
    void render(const RenderInfo& info) const;
};

#endif
