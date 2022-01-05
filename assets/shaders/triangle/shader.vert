#version 460

layout(std430, binding = 0) buffer faces_buffer {
    uint faces[];
};

out vec3 c_col;
out vec3 c_normal;

uniform mat4 mvp;

#define ITNNV 0
#define ITNPV 1
#define ITPNV 2
#define ITPPV 3
#define IBNPV 4
#define IBNNV 5
#define IBPPV 6
#define IBPNV 7

vec3 CVS[8] = {
    vec3(0, 1, 0),
    vec3(0, 1, 1),
    vec3(1, 1, 0),
    vec3(1, 1, 1),
    vec3(0, 0, 0),
    vec3(0, 0, 1),
    vec3(1, 0, 0),
    vec3(1, 0, 1),
};

#define FN 0
#define FS 1
#define FW 2
#define FE 3
#define FU 4
#define FD 5

vec3 NORMAL_LUT[6] = {
    vec3( 0, -1,  0),
    vec3( 0,  1,  0),
    vec3( 0,  0,  1),
    vec3( 0,  0, -1),
    vec3( 1,  0,  0),
    vec3(-1,  0,  0),
};

int BLOCK_VERTEX_LUT[6][6] = {
{IBNNV, IBPNV, ITPPV, ITNPV, IBNNV, ITPPV},
{IBPPV, IBNPV, ITPNV, IBNPV, ITNNV, ITPNV},
{IBPNV, IBPPV, ITPNV, IBPNV, ITPNV, ITPPV},
{IBNPV, IBNNV, ITNNV, ITNNV, IBNNV, ITNPV},
{ITNNV, ITNPV, ITPNV, ITNPV, ITPPV, ITPNV},
{IBNNV, IBNPV, IBPNV, IBNPV, IBPPV, IBPNV}
};

void main()
{
    int face_idx = gl_VertexID / 6;
    int face_vertex_num = gl_VertexID % 6;

    uint face_data = faces[face_idx];

    vec3 block_pos;
    block_pos.x = (face_data >> 28) & uint(0xF);
    block_pos.y = (face_data >> 20) & uint(0xFF);
    block_pos.z = (face_data >> 16) & uint(0xF);

//    uint meta = uint(face_data.w);
//    uint direction = (meta & uint(0xF000)) >> 12;
//    uint id = (meta & uint(0xFFFF0000)) >> 16;
    uint direction = (face_data >> 13) & uint(0x7);
    uint id = (face_data >> 5) & uint(0xFF);
    uint meta = (face_data >> 1) & uint(0xF);

    id = id << 4;
    id |= meta & uint(0xF);

    uint seed = id;

    vec3 pos = CVS[BLOCK_VERTEX_LUT[direction][face_vertex_num]] + block_pos;

    gl_Position = mvp * vec4(pos.xyz, 1.0);
    c_normal = NORMAL_LUT[direction];

    seed = (seed ^ uint(61)) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    vec3 col;
    seed ^= (seed << 13);
    seed ^= (seed >> 17);
    seed ^= (seed << 5);
    col.x = float(seed) / 4294967296.0;
    seed ^= (seed << 13);
    seed ^= (seed >> 17);
    seed ^= (seed << 5);
    col.y = float(seed) / 4294967296.0;
    seed ^= (seed << 13);
    seed ^= (seed >> 17);
    seed ^= (seed << 5);
    col.z = float(seed) / 4294967296.0;
    c_col = col.xyz;
}