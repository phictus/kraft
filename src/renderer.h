#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

#include "util/type.h"

#define PROGRAM_COUNT 1
#define TEXTURE_COUNT 1

typedef struct {
    const char* vert;
    const char* frag;
} ProgramPath;

typedef enum {
    DEFAULT_PROGRAM = 0
} Program;

typedef enum {
    GRASS_TEXTURE = 0
} Texture;

typedef struct {
    const f32* vertex_data;
    size vertex_size;
    const u32* element_data;
    size element_size;
} MeshSpec;

typedef struct {
    u32 vertex_array;
    u32 vertex_buffer;
    u32 element_buffer;
    u32 element_count;
} Mesh;

void renderer_init();
void renderer_deinit();

void renderer_clear();

void renderer_create_mesh(Mesh* mesh, const MeshSpec* spec, u32 attrib_count, ...);
void renderer_delete_mesh(Mesh* mesh);
void renderer_mesh_draw(const Mesh* mesh);

void renderer_program_use(Program program);
void renderer_program_set_i32(Program program, const char* name, i32 value);
void renderer_program_set_mat4(Program program, const char* name, const mat4 value);

void renderer_texture_use(Texture texture, u32 index);

#endif
