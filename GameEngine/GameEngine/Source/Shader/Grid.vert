#version 460 core

vec3 gridPlane[4] = vec3[](
    vec3(-1, 1, -1),
    vec3(-1, -1, -1),
    vec3(1, -1, -1),
    vec3(1, 1, -1)
);

//Output Data
out vec3 nearPoint;
out vec3 farPoint;

uniform mat4 u_VP;

void main()
{
    vec3 point = gridPlane[gl_VertexID];

    vec4 near = inverse(u_VP) * vec4(point.xy, -1, 1);
    nearPoint = near.xyz / near.w;

    vec4 far = inverse(u_VP) * vec4(point.xy, 1, 1);
    farPoint = far.xyz / far.w;

    gl_Position = vec4(point, 1);
}