//
// Created by brucegoose on 7/5/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_COMMON_HPP
#define INFRASTRUCTURE_GRAPHICS_COMMON_HPP

#include <string>

/*
 * EGL headers.
 */
#include <EGL/egl.h>

/*
 * OpenGL headers.
 */
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>


#include "domain/installation.hpp"
#include "domain/display.hpp"

namespace infrastructure {

    struct GraphicsConfig {
        domain::Display display;
        domain::installation::Config installation_config;
        domain::installation::Layout installation_layout;
        static GraphicsConfig from_json(const nlohmann::json& j) {
            GraphicsConfig conf{};
            conf.display = domain::Display::from_json(j.at("display"));
            conf.installation_config = domain::installation::Config::from_json(j.at("installation_config"));
            conf.installation_layout = domain::installation::Layout::from_json(j.at("installation_layout"));
            return conf;
        }
    };

    // forward declaration, for the boys
    class Graphics;

    void ThrowOnGlError(const std::string &display_on_error) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            throw std::runtime_error(display_on_error);
        }
    }

}



#endif //INFRASTRUCTURE_GRAPHICS_COMMON_HPP
