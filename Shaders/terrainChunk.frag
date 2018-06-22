#version 330 core
out vec4 fragcolor;
in vec3 normal;

vec3 lightPos = vec3(0, -1, 0);
void main(void)
{
    float diffuse = dot(-normal, lightPos);
    diffuse *= 0.5;
    fragcolor = vec4(normal.x, normal.y, normal.z, 1.0);
}
