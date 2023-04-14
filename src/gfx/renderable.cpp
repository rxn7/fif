#include "fif/gfx/renderable.h"
#include "fif/core/assertion.h"

#include "glad/glad.h"
#include <memory>

const std::string VERTEX = R"(
#version 330 core

layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec2 l_UV;
layout(location = 2) in vec4 l_Color;

out vec4 v_Color;
out vec2 v_UV;

void main() {
	v_Color = l_Color;
	v_UV = l_UV;
	gl_Position = vec4(l_Position, 1.0);
})";

const std::string FRAGMENT = R"(
#version 330 core

in vec4 v_Color;
in vec2 v_UV;

out vec4 f_Color;

void main() {
	f_Color = v_Color;
})";

static std::shared_ptr<fif::gfx::Shader> s_PrimitiveShader = nullptr;

fif::gfx::Renderable::Renderable(const std::vector<Vertex> &vertices, const std::vector<std::uint32_t> &elements) {
	glCreateVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, color)));

	glGenBuffers(1, &m_Ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(std::uint32_t), elements.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	if(s_PrimitiveShader.get() == nullptr)
		s_PrimitiveShader = std::make_unique<Shader>(VERTEX, FRAGMENT);

	mp_Shader = s_PrimitiveShader;
}

void fif::gfx::Renderable::render() const {
	mp_Shader->bind();

	glBindVertexArray(m_Vao);
	glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}
