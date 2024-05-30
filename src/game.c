#include "window.h"
#include "renderer.h"
#include "camera.h"
#include "game.h"

static Mesh mesh;
static mat4 transform;

void game_init() {
    window_init();
    renderer_init();
    camera_init();

    const f32 vertex_data[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    const u32 element_data[] = {0, 2, 1, 0, 2, 3};
    MeshSpec spec = {
        .vertex_data = vertex_data,
        .vertex_size = 4 * 5 * sizeof(f32),
        .element_data = element_data,
        .element_size = 6 * sizeof(u32)
    };
    renderer_create_mesh(&mesh, &spec, 2, 3, 2);
    glm_mat4_identity(transform);
    glm_translate(transform, (vec3){0.0f, 0.0f, -2.0f});
}

void game_deinit() {
    renderer_delete_mesh(&mesh);
    renderer_deinit();
    window_deinit();
}

bool game_is_running() {
    return window_is_open();
}

void game_update() {
    window_poll_events();

    if (window_is_key_pressed(KEY_ESCAPE)) {
        window_close();
    }

    camera_update();

    renderer_clear();

    renderer_program_use(DEFAULT_PROGRAM);
    renderer_program_set_mat4(DEFAULT_PROGRAM, "u_ViewProjection", *camera_get_view_projection());
    renderer_program_set_mat4(DEFAULT_PROGRAM, "u_Transform", transform);
    renderer_program_set_i32(DEFAULT_PROGRAM, "u_Texture", 0);
    renderer_texture_use(GRASS_TEXTURE, 0);
    renderer_mesh_draw(&mesh);

    window_swap_buffers();
}
