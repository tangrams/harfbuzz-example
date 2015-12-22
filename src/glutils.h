#pragma once

#include "GLFW/glfw3.h"

namespace gl {
    GLFWwindow* g_window;
    GLuint program;
    GLuint vertex;
    GLuint fragment;
    GLint positionLoc;
    GLint uvLoc;

    typedef struct {
        float x, y;
        float s, t;
    } Vertex;

    typedef struct {
        unsigned short* indices;
        unsigned char* textureData;
        unsigned int textureId;
        Vertex* vertices;
        unsigned int nbIndices;
        unsigned int nbVertices;
        GLuint vertexBuffer;
        GLuint indexBuffer;
    } Mesh;

    static const GLchar* vertexSrc =
        R"END(
        attribute vec4 a_position;
    attribute vec2 a_uv;

    varying vec2 f_uv;

    void main() {
        gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * a_position;
        f_uv = a_uv;
    }
    )END";

    static const GLchar* fragmentSrc =
        R"END(
        uniform sampler2D u_tex;

    varying vec2 f_uv;

    void main() {
        gl_FragColor = texture2D(u_tex, f_uv);
    }
    )END";

    void createShaderProgram() {
        program = glCreateProgram();
        vertex = glCreateShader(GL_VERTEX_SHADER);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragment, 1, &fragmentSrc, NULL);
        glCompileShader(fragment);
        glAttachShader(program, fragment);

        glShaderSource(vertex, 1, &vertexSrc, NULL);
        glCompileShader(vertex);
        glAttachShader(program, vertex);

        glLinkProgram(program);

        glUniform1i(glGetUniformLocation(program, "u_tex"), 0);

        positionLoc = glGetAttribLocation(program, "a_position");
        uvLoc =	glGetAttribLocation(program, "a_uv");
    }

    void render(const std::vector<Mesh*> meshes) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        for(auto mesh: meshes) {
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
            glBindTexture(GL_TEXTURE_2D, mesh->textureId);

            glEnableVertexAttribArray(positionLoc);
            glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glEnableVertexAttribArray(uvLoc);
            glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const GLvoid*)(2 * sizeof(float)));

            glDrawElements(GL_TRIANGLES, mesh->nbIndices, GL_UNSIGNED_SHORT, 0);
        }

        glUseProgram(0);
    }

    void resize(GLFWwindow* window, int w, int h) {
        glViewport(0, 0, w, h);
    }

    void finish() {
        glDeleteShader(fragment);
        glDeleteShader(vertex);
        glDeleteProgram(program);
    }

    void initGL(int argc, char** argv) {
        int width = 800;
        int height = 400;

        if(!glfwInit()) {
            std::cerr << "glfw init failed" << std::endl;
            exit(-1);
        }

        g_window = glfwCreateWindow(width, height, "harfbuzz-ex", NULL, NULL);

        if(!g_window) {
            glfwTerminate();
            std::cerr << "window creation failed" << std::endl;
            exit(-1);
        }

        glfwMakeContextCurrent(g_window);

        glfwSetWindowSizeCallback(g_window, resize);

        glClearColor(1, 1, 1, 1);
        glViewport(0, 0, width, height);

        createShaderProgram();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void loop(const std::vector<Mesh*> meshes) {
        while(!glfwWindowShouldClose(g_window)) {
            glfwSwapBuffers(g_window);

            render(meshes);

            glfwPollEvents();
        }
        glfwTerminate();
    }

    void uploadMeshes(const std::vector<Mesh*> meshes) {
        for(auto mesh: meshes) {
            std::vector<float> data;

            for(int i = 0; i < mesh->nbVertices; i++) {
                data.push_back(mesh->vertices[i].x);
                data.push_back(mesh->vertices[i].y);
                data.push_back(mesh->vertices[i].s);
                data.push_back(mesh->vertices[i].t);
            }

            glBindTexture(GL_TEXTURE_2D, mesh->textureId);

            glGenBuffers(1, &mesh->vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

            glGenBuffers(1, &mesh->indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->nbIndices * sizeof(unsigned short), mesh->indices, GL_STATIC_DRAW);
        }
    }

    void deleteMeshes(const std::vector<Mesh*> meshes) {
        for(auto mesh: meshes) {
            delete[] mesh->textureData;

            glDeleteTextures(1, &mesh->textureId);
            glDeleteBuffers(1, &mesh->indexBuffer);
            glDeleteBuffers(1, &mesh->vertexBuffer);

            delete[] mesh->vertices;
            delete[] mesh->indices;
            delete mesh;
        }
    }

    unsigned int getTextureId(int width, int height) {
        GLuint textureId;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        return textureId;
    }

    void uploadTextureData(unsigned int textureId, int width, int height, unsigned char* data) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
    }
}
