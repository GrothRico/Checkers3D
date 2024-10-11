#version 450 core

out vec4 FragColor;

in vec2 vertex_texture_coordinates;

uniform sampler2D Texture;

void main() {
    FragColor = texture(Texture, vertex_texture_coordinates);
}