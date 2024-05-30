#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stdarg.h>

#include "util/debug.h"
#include "renderer.h"

static u32 programs[PROGRAM_COUNT];
static u32 textures[TEXTURE_COUNT];

static void opengl_debug_callback(u32 source, u32 type, u32 id, u32 severity, i32 length, const char* message, const void* user_param) {
    LOG_ERROR("%s\n", message);
}

// CALL free() ON RETURN VALUE
static char* read_file_as_string(const char* path) {
    FILE* stream;
    ASSERT(fopen_s(&stream, path, "r") == 0, "Reading %s failed!\n", path);

    fseek(stream, 0, SEEK_END);
    i64 length = ftell(stream);
    char* buffer = malloc((length + 1) * sizeof(char));
    buffer[length] = '\0';

    fseek(stream, 0, SEEK_SET);
    fread(buffer, 1, length, stream);

    fclose(stream);
    return buffer;
}

// CALL glDeleteShader() ON RETURN VALUE
static u32 create_shader(u32 type, const char* path) {
    u32 shader = glCreateShader(type);
    char* source = read_file_as_string(path);
    glShaderSource(shader, 1, (const char**)&source, NULL);
    glCompileShader(shader);
    free(source);

    i32 status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    ASSERT(status == GL_TRUE, "Compiling %s failed!\n", path);

    return shader;
}

// CALL glDeleteProgram() ON RETURN VALUE
static u32 create_program(ProgramPath path) {
    u32 program = glCreateProgram();

    u32 vert_shader = create_shader(GL_VERTEX_SHADER, path.vert);
    u32 frag_shader = create_shader(GL_FRAGMENT_SHADER, path.frag);

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);
    glValidateProgram(program);

    i32 status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    ASSERT(status == GL_TRUE, "Linking %s with %s failed!\n", path.vert, path.frag);

    glDetachShader(program, vert_shader);
    glDetachShader(program, frag_shader);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}

// CALL glDeleteTextures ON RETURN VALUE
static u32 create_texture(const char* path) {
    i32 width, height, channels_in_file;
    u8* buffer = stbi_load(path, &width, &height, &channels_in_file, 0);
    ASSERT(buffer, "Loading %s failed!\n", path);

    u32 texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    stbi_image_free(buffer);
    return texture;
}

void renderer_init() {
    ASSERT(gladLoadGL(glfwGetProcAddress), "GLAD initialization failed!\n");

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, NULL);

    glClearColor(0.4f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LOG_INFO("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    LOG_INFO("OpenGL Version: %s\n", glGetString(GL_VERSION));

    ProgramPath program_paths[PROGRAM_COUNT] = {
        [DEFAULT_PROGRAM] = (ProgramPath){
            .vert = "assets/programs/default.vert.glsl",
            .frag = "assets/programs/default.frag.glsl"
        }
    };

    for (size i = 0; i < PROGRAM_COUNT; i++) {
        programs[i] = create_program(program_paths[i]);
    }

    stbi_set_flip_vertically_on_load(true);

    const char* texture_paths[TEXTURE_COUNT] = {
        [GRASS_TEXTURE] = "assets/textures/grass.png"
    };

    for (size i = 0; i < TEXTURE_COUNT; i++) {
        textures[i] = create_texture(texture_paths[i]);
    }
}

void renderer_deinit() {
    for (size i = 0; i < TEXTURE_COUNT; i++) {
        glDeleteTextures(1, &textures[i]);
    }

    for (size i = 0; i < PROGRAM_COUNT; i++) {
        glDeleteProgram(programs[i]);
    }
}

void renderer_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_create_mesh(Mesh* mesh, const MeshSpec* spec, u32 attrib_count, ...) {
    glCreateVertexArrays(1, &mesh->vertex_array);
    glCreateBuffers(1, &mesh->vertex_buffer);
    glCreateBuffers(1, &mesh->element_buffer);

    va_list attribs;
    va_start(attribs, attrib_count);
    size stride = 0;
    for (size i = 0; i < attrib_count; i++) {
        u32 attrib = va_arg(attribs, u32);
        glEnableVertexArrayAttrib(mesh->vertex_array, i);
        glVertexArrayAttribBinding(mesh->vertex_array, i, 0);
        glVertexArrayAttribFormat(mesh->vertex_array, i, attrib, GL_FLOAT, GL_FALSE, stride);
        stride += attrib * sizeof(f32);
    }
    va_end(attribs);

    glNamedBufferData(mesh->vertex_buffer, spec->vertex_size, spec->vertex_data, GL_STATIC_DRAW);
    glNamedBufferData(mesh->element_buffer, spec->element_size, spec->element_data, GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(mesh->vertex_array, 0, mesh->vertex_buffer, 0, stride);
    glVertexArrayElementBuffer(mesh->vertex_array, mesh->element_buffer);

    mesh->element_count = (u32)(spec->element_size / sizeof(u32));
}

void renderer_delete_mesh(Mesh* mesh) {
    glDeleteBuffers(1, &mesh->element_buffer);
    glDeleteBuffers(1, &mesh->vertex_buffer);
    glDeleteVertexArrays(1, &mesh->vertex_array);
}

void renderer_mesh_draw(const Mesh* mesh) {
    glBindVertexArray(mesh->vertex_array);
    glDrawElements(GL_TRIANGLES, mesh->element_count, GL_UNSIGNED_INT, NULL);
}

void renderer_program_use(Program program) {
    glUseProgram(programs[program]);
}

void renderer_program_set_i32(Program program, const char* name, i32 value) {
    i32 location = glGetUniformLocation(programs[program], name);
    glUniform1i(location, value);
}

void renderer_program_set_mat4(Program program, const char* name, const mat4 value) {
    i32 location = glGetUniformLocation(programs[program], name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value[0]);
}

void renderer_texture_use(Texture texture, u32 unit) {
    glBindTextureUnit(unit, textures[texture]);
}
