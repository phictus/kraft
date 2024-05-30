#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoordinate;

out vec2 v_TextureCoordinate;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    v_TextureCoordinate = a_TextureCoordinate;
}
