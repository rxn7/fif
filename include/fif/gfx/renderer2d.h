#pragma once

#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/primitiveType.h"

#include <memory>

namespace fif::gfx {
    namespace Renderer2D {
        OrthoCamera &getCamera();

        void init();
        void begin();
        void end();

        void renderQuad(const glm::vec2 &position, const glm::vec2 &size, float angle = 0.0f, const glm::u8vec4 &color = {255, 255, 255, 255});

        void renderCircleTriangle(const glm::vec2 &position, float diameter, std::uint16_t segmentCount = 0, const glm::u8vec4 &color = {255, 255, 255, 255});
        void renderCircleFrag(const glm::vec2 &position, float diameter, const glm::u8vec4 &color = {255, 255, 255, 255});
    } // namespace Renderer2D
} // namespace fif::gfx
