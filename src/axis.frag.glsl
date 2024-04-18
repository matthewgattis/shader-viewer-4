#version 330

in vec3 out_Color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(mix(out_Color, vec3(1.0f, 1.0f, 1.0f), 0.5f), 1.0f);
}
