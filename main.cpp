// Assignment 7, BattleZone By: Patrick Carroll
// Date: 12/11/2023

#include "glad/src/glad.c"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;



const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec3 objectColor;

    void main() {
        // Set FragColor to constant green color
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
)";

GLfloat catVertices[] = {
    // Pyramid vertices
    -0.5f, -1.0f, -0.5f, // 0
    0.5f, -1.0f, -0.5f,  // 1
    0.5f, -1.0f, 0.5f,   // 2
    -0.5f, -1.0f, 0.5f,  // 3
    0.0f, 0.7f, 0.0f,    // 4 Correct

    // Bottom Face
    -0.5f, 0.7f, -0.5f, // 5
    0.5f, 0.7f, -0.5f,  // 6
    0.5f, 0.7f, 0.5f,   // 7
    -0.5f, 0.7f, 0.5f,  // 8

    // Top Face
    -0.5f, 1.4f, -0.5f, // 9
    0.5f, 1.4f, -0.5f,  // 10
    0.5f, 1.4f, 0.5f,   // 11
    -0.5f, 1.4f, 0.5f,  // 12
    0.0f, 1.4f, -0.5f,  // 13 Mid
    0.0f, 1.4f, 0.5f,   // 14 Mid
    -0.25f, 2.0f, 0.0f, // 15 Top
    0.25f, 2.0f, 0.0f,  // 16 Top
};
GLuint catIndices[] = {
    // Pyramid indices
    0,
    1,
    2,
    0,
    2,
    3,
    0,
    1,
    4,
    1,
    2,
    4,
    2,
    3,
    4,
    3,
    0,
    4,

    // Bottom Face
    5,
    6,
    7,
    5,
    7,
    8,
    6,
    7,
    4,
    7,
    8,
    4,
    8,
    5,
    4,
    5,
    6,
    4,

    // Top Face
    9,
    10,
    11,
    9,
    11,
    12,
    9,
    5,
    10,
    10,
    6,
    5,
    7,
    6,
    10,
    10,
    11,
    7,
    8,
    7,
    11,
    11,
    12,
    8,
    5,
    8,
    12,
    12,
    9,
    5,

    // Left Ear
    12,
    15,
    9,
    14,
    15,
    12,
    9,
    15,
    13,
    13,
    12,
    14,

    // Right Ear
    14,
    16,
    13,
    14,
    11,
    16,
    11,
    10,
    16,
    10,
    16,
    13,

};
GLfloat pyramidVertices[] = {
    // Pyramid vertices
    -40.0f, -1.0f, -40.0f, // 0
    40.0f, -1.0f, -40.0f,  // 1
    40.0f, -1.0f, 40.0f,   // 2
    -40.0f, -1.0f, 40.0f,  // 3
    0.0f, 40.0f, 0.0f,    // 4 Top of pyramid

};
GLuint pyramidIndices[] = {
    // Pyramid indices
    0,
    1,
    2,
    0,
    2,
    3,
    0,
    1,
    4,
    1,
    2,
    4,
    2,
    3,
    4,
    3,
    0,
    4,
};
GLfloat octahedronVertices[] = {
    0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, -1.0f, 0.0f};
GLuint octahedronIndices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 1,
    5, 1, 2,
    5, 2, 3,
    5, 3, 4,
    5, 4, 1};
GLfloat tetrahedronVertices[] = {
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    1.0f,
};
GLuint tetrahedronIndices[] = {
    0,
    1,
    2,
    0,
    1,
    3,
    0,
    2,
    3,
    1,
    2,
    3,
};
GLfloat cubeVertices[] = {
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
};
GLuint cubeIndices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 3, 7, 7, 4, 0,
    1, 2, 6, 6, 5, 1,
    3, 2, 6, 6, 7, 3,
    0, 1, 5, 5, 4, 0,
};
GLfloat horizon [] = {
    -2.0f, 0.0f,
     2.0f, 0.0f
};
GLfloat verticesCrossHairLockOn [] = {
    // TOP
    0.08f, 0.0533f,
    0.12f, 0.16f,
    -0.08f, 0.0533f,
    -0.12f, 0.16f,
    0.12f, 0.16f,
    -0.12f, 0.16f,
    0.0f, 0.16f,
    0.0f, 0.3333f,

    // BOTTOM
    0.08f, -0.0533f,
    0.12f, -0.16f,
    -0.08f, -0.0533f,
    -0.12f, -0.16f,
    0.12f, -0.16f,
    -0.12f, -0.16f,
    0.0f, -0.16f,
    0.0f, -0.3333f
};
GLfloat verticesCrossHairNoLock[] = {
    // TOP
    0.12f, 0.0533f,
    0.12f, 0.16f,
    -0.12f, 0.0533f,
    -0.12f, 0.16f,
    0.12f, 0.16f,
    -0.12f, 0.16f,
    0.0f, 0.16f,
    0.0f, 0.3333f,
    // BOTTOM
    0.12f, -0.0533f,
    0.12f, -0.16f,
    -0.12f, -0.0533f,
    -0.12f, -0.16f,
    0.12f, -0.16f,
    -0.12f, -0.16f,
    0.0f, -0.16f,
    0.0f, -0.3333f
};
GLfloat verticesScreenCrack[] = {
    // Draw Cracks 1
    -0.2f, 0.2f,   -0.4f, 0.4f,
    -0.4f, 0.4f,   -0.7f, 0.5f,
    -0.7f, 0.5f,   -0.8f, 1.0f,
    // Draw cracks 2
    -0.2f, 0.2f,   -0.3f, 0.7f,
    -0.3f, 0.7f,   -0.2f, 1.0f,
    // Draw cracks 3
    -0.2f, 0.2f,    0.0f, 0.7f,
     0.0f, 0.7f,    0.2f, 1.0f,
    // Draw cracks 4
    -0.2f, 0.2f,    0.3f, 0.7f,
     0.3f, 0.7f,    0.4f, 1.0f,
    // Draw cracks 5
    -0.2f, 0.2f,    0.7f, 0.5f,
     0.7f, 0.5f,    0.8f, 1.0f,
    // Draw Cracks 1
    -0.2f, 0.2f,   -0.7f, -0.5f,
    -0.7f, -0.5f,  -0.4f, -0.4f,
    -0.4f, -0.4f,  -0.2f, -1.0f,
    // Draw cracks 2
    -0.2f, 0.2f,    0.5f, -0.7f,
     0.5f, -0.7f,   0.3f, -1.0f,
    // Draw cracks 3
    -0.2f, 0.2f,    0.7f, -0.5f,
     0.7f, -0.5f,   0.8f, -1.0f,
    // Draw cracks 4
    -0.2f, 0.2f,    0.7f, 0.5f,
     0.7f, 0.5f,    0.8f, 1.0f
};
GLfloat gRaito = 1.618f; // Golden ratio
GLfloat icosahedronVertices[] = {
    0.0f, 1.0f, 1 * gRaito,   // 0
    0.0f, 1.0f, -1 * gRaito,  // 1
    0.0f, -1.0f, 1 * gRaito,  // 2
    0.0f, -1.0f, -1 * gRaito, // 3

    1.0f, 1 * gRaito, 0.0f,   // 4
    -1.0f, 1 * gRaito, 0.0f,  // 5
    1.0f, -1 * gRaito, 0.0f,  // 6
    -1.0f, -1 * gRaito, 0.0f, // 7

    1 * gRaito, 0.0f, 1.0f,   // 8
    -1 * gRaito, 0.0f, 1.0f,  // 9
    1 * gRaito, 0.0f, -1.0f,  // 10
    -1 * gRaito, 0.0f, -1.0f, // 11
};
GLuint icosahedronIndices[] = {
    0, 4, 8,  // 0
    0, 4, 5,  // 1
    0, 2, 8,  // 2
    0, 2, 9,  // 3
    0, 5, 9,  // 3
    3, 7, 11, // 4
    3, 7, 6,  //  6
    3, 1, 11, // 7
    3, 1, 10, // 8
    3, 6, 10, //  9
    2, 6, 8,  // 10
    2, 6, 7,  // 11
    2, 7, 9,  // 12
    4, 8, 10, // 13
    5, 9, 11, // 14
    1, 4, 10, // 15
    1, 5, 4,  // 16
    2, 9, 7,  // 17
    11, 9, 7, // 18
    10, 8, 6, // 19
};

struct CollisionBoxWalls {
    glm::vec3 position;
    glm::vec3 size;
    CollisionBoxWalls(glm::vec3 pos, const glm::vec3& s) : position(pos), size(s) {}
};

struct CollisionBoxSelf {
    glm::vec3 position;
    glm::vec3 size;
    CollisionBoxSelf(glm::vec3 pos, const glm::vec3& s) : position(pos), size(s) {}
};

// 2D crosshair vertices for lock on and no lock on crack
GLuint CrossHairNoLockVBO, CrossHairNoLockVAO;
GLuint CrossHairLockOnVBO, CrossHairLockOnVAO;
GLuint HorizonVBO, HorizonVAO;
GLuint ScreenCrackVBO, ScreenCrackVAO;
GLint modelLoc, viewLoc, projectionLoc, objectColorLoc;

bool qpress = true; // Wireframe mode
bool wKeyPressed = false;   // Camera movement
bool aKeyPressed = false;   // Camera movement
bool sKeyPressed = false;   // Camera movement
bool dKeyPressed = false;   // Camera movement

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

// Game variables
bool lockon = false; // Lock on to enemy
bool screenCrack = false; // Screen crack
bool createwalls = true; // Create walls
bool octahedronRight = false; // Octahedron direction
glm::vec3 octahedronPos = glm::vec3(0.0f, 0.0f, 50.0f); // Octahedron position
bool shoot = false; // Shoot

// Object Vector to Store random Cubes
std::vector<glm::vec3> cubeLocations;

// Set object colors
glm::vec3 cubeColor;
float cameraSpeed = 10.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

GLFWwindow* window;
GLuint  vertexShader, fragmentShader, shaderProgram;
GLuint  cubeVAO, cubeVBO, cubeEBO, pyramidVAO, pyramidVBO, pyramidEBO, octahedronVAO, octahedronVBO, octahedronEBO, tetrahedronVAO,
        tetrahedronVBO, tetrahedronEBO, catVAO, catVBO, catEBO, icohedronVAO, icohedronVBO, icohedronEBO;

struct ParticleShoot {
    glm::vec3 position;
    glm::vec3 velocity;
    float life;
};
struct ParticleVolcano {
    glm::vec3 position;
    glm::vec3 velocity;
    float life;
};

std::vector<ParticleShoot> particlesShoot;
std::vector<ParticleVolcano> particlesVolcano;

void glfwErrorCB(int error, const char* description) {
    fputs(description, stderr);
}

// Input callback functions
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        shoot=!shoot; // Shoot
    }
    if (key == GLFW_KEY_F && action == GLFW_RELEASE) {
        shoot=!shoot; // Shoot
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        qpress=!qpress; // Wire Frame
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        screenCrack=!screenCrack; // Screen Crack
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        lockon=!lockon; // Lock On
    }
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            wKeyPressed = true;
        } else if (action == GLFW_RELEASE) {
            wKeyPressed = false;
        }
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            sKeyPressed = true;
        } else if (action == GLFW_RELEASE) {
            sKeyPressed = false;
        }
    }
    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            aKeyPressed = true;
        } else if (action == GLFW_RELEASE) {
            aKeyPressed = false;
        }
    }
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            dKeyPressed = true;
        } else if (action == GLFW_RELEASE) {
            dKeyPressed = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// 3d functions
void drawOctahedron(glm::vec3 octahedronPosition){
    // Draw the octahedron center
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(cubeColor));
    glBindVertexArray(octahedronVAO);
    // Set the model matrix for the octahedron
    glm::mat4 octahedronModel = glm::translate(glm::mat4(1.0f), glm::vec3(octahedronPosition));
    // Update the model and view matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(octahedronModel));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Draw the octahedron using its indices
    glDrawElements(GL_TRIANGLES, sizeof(octahedronIndices), GL_UNSIGNED_INT, 0);
}
void drawTetrahedron(glm::vec3 tetrahedronPosition) {
    // Draw the tetrahedron center
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(cubeColor));
    glBindVertexArray(tetrahedronVAO);
    // Set the model matrix for the tetrahedron
    glm::mat4 tetrahedronModel = glm::translate(glm::mat4(1.0f), glm::vec3(tetrahedronPosition));
    // Update the model and view matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(tetrahedronModel));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Draw the tetrahedron using its indices
    glDrawElements(GL_TRIANGLES, sizeof(tetrahedronIndices), GL_UNSIGNED_INT, 0);
}
void drawPyramid(glm::vec3 pyramidPosition, glm::vec3 pyramidScale) {
    // Draw the pyramid center
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(cubeColor));
    glBindVertexArray(pyramidVAO);
    // Set the model matrix for the pyramid
    glm::mat4 pyramidModel = glm::translate(glm::mat4(1.0f), glm::vec3(pyramidPosition));
    pyramidModel = glm::scale(pyramidModel, pyramidScale);
    // Update the model and view matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Draw the pyramid using its indices
    glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices), GL_UNSIGNED_INT, 0);
}
void drawCat(glm::vec3 catPosition){
    // Draw the cat center
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(cubeColor));
    glBindVertexArray(catVAO);
    // Set the model matrix for the cat
    glm::mat4 catModel = glm::translate(glm::mat4(1.0f), glm::vec3(catPosition));
    // Update the model and view matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(catModel));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Draw the cat using its indices
    glDrawElements(GL_TRIANGLES, sizeof(catIndices), GL_UNSIGNED_INT, 0);
}
void drawCube(glm::vec3 cubePosition) {
    // Draw the cube center
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(cubeColor));
    glBindVertexArray(cubeVAO);
    // Set the model matrix for the cube
    glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), glm::vec3(cubePosition));
    // Update the model and view matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Draw the cube using its indices
    glDrawElements(GL_TRIANGLES, sizeof(cubeIndices), GL_UNSIGNED_INT, 0);
}
void drawCubeHUD(){
    // Calculate the view matrix
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    // Create a transformation matrix for the cube
    glm::mat4 cubeTransform = glm::translate(glm::mat4(1.0f), cameraPos + cameraFront * 2.0f);
    // Calculate the rotation axis using the cross product
    glm::vec3 rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), cameraFront);
    // Calculate the rotation angle using the dot product
    float angle = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, -1.0f), cameraFront)));
    // Apply the rotation to the cube
    cubeTransform = glm::rotate(cubeTransform, glm::radians(angle), rotationAxis);
    // Set the model matrix for the cube
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(cubeTransform));
    // Set the view matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Bind the VAO and draw the cube
    glBindVertexArray(cubeVAO);
    //glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / (3 * sizeof(GLfloat)));
    glBindVertexArray(0);
}
void drawIcohedron(glm::vec3 icohedronPosition){
    // Draw the icohedron center
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(cubeColor));
    glBindVertexArray(icohedronVAO);
    // Set the model matrix for the icohedron
    glm::mat4 icohedronModel = glm::translate(glm::mat4(1.0f), icohedronPosition);
    icohedronModel = glm::scale(icohedronModel, glm::vec3(0.01f, 0.01f, 0.01f));
    // Update the model and view matrices in the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(icohedronModel));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Draw the icohedron using its indices
    glDrawElements(GL_TRIANGLES, sizeof(icosahedronIndices), GL_UNSIGNED_INT, 0);
}

// 2d functions
void setupHorizon(){
    glGenVertexArrays(1, &HorizonVAO);
    glGenBuffers(1, &HorizonVBO);
    glBindVertexArray(HorizonVAO);
    glBindBuffer(GL_ARRAY_BUFFER, HorizonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(horizon), horizon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void drawHorizon(){
    glColor3f(0.0f, 1.0f, 0.0f);
    glBindVertexArray(HorizonVAO);
    glDrawArrays(GL_LINES, 0, sizeof(horizon) / (2 * sizeof(GLfloat)));
    glBindVertexArray(0);
}
void setupCrossHairNoLock() {
    glGenVertexArrays(1, &CrossHairNoLockVAO);
    glGenBuffers(1, &CrossHairNoLockVBO);
    glBindVertexArray(CrossHairNoLockVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CrossHairNoLockVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCrossHairNoLock), verticesCrossHairNoLock, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void setupCrossHairLockOn() {
    glGenVertexArrays(1, &CrossHairLockOnVAO);
    glGenBuffers(1, &CrossHairLockOnVBO);
    glBindVertexArray(CrossHairLockOnVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CrossHairLockOnVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCrossHairLockOn), verticesCrossHairLockOn, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void drawCrossHairNoLock() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBindVertexArray(CrossHairNoLockVAO);
    glDrawArrays(GL_LINES, 0, sizeof(verticesCrossHairNoLock) / (2 * sizeof(GLfloat)));
    glBindVertexArray(0);
}
void drawCrossHairLockOn() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBindVertexArray(CrossHairLockOnVAO);
    glDrawArrays(GL_LINES, 0, sizeof(verticesCrossHairLockOn) / (2 * sizeof(GLfloat)));
    glBindVertexArray(0);
}
void drawCrossHair(bool lockOn){
    if (lockOn){
        drawCrossHairLockOn();
    } else {
        drawCrossHairNoLock();
    }
}
void setupScreenCrack() {
    glGenVertexArrays(1, &ScreenCrackVAO);
    glGenBuffers(1, &ScreenCrackVBO);
    glBindVertexArray(ScreenCrackVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ScreenCrackVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesScreenCrack), verticesScreenCrack, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void drawScreenCrack() {
    glColor3f(0.0f, 1.0f, 0.0f);
    // Translate the screen crack vertices to the camera position
    glBindVertexArray(ScreenCrackVAO);
    glDrawArrays(GL_LINES, 0, sizeof(verticesScreenCrack) / (2 * sizeof(GLfloat)));
    glBindVertexArray(0);
}
void cleanup2D(){
    glDeleteVertexArrays(1, &CrossHairNoLockVAO);
    glDeleteBuffers(1, &CrossHairNoLockVBO);
    glDeleteVertexArrays(1, &CrossHairLockOnVAO);
    glDeleteBuffers(1, &CrossHairLockOnVBO);
    glDeleteVertexArrays(1, &ScreenCrackVAO);
    glDeleteBuffers(1, &ScreenCrackVBO);
    glDeleteVertexArrays(1, &HorizonVAO);
    glDeleteBuffers(1, &HorizonVBO);
}

// Shoot Particle functions
void initializeParticlesShoot() {
    particlesShoot.clear();
}
void updateParticlesShoot(float deltaTime) {
    // Update particle positions and lifetimes
    for (auto& particleShoot : particlesShoot) {
        particleShoot.position += particleShoot.velocity * deltaTime;
        particleShoot.life -= deltaTime;
    }

    // Remove dead particles
    particlesShoot.erase(std::remove_if(particlesShoot.begin(), particlesShoot.end(),
        [](const ParticleShoot& particleShoot) { return particleShoot.life <= 0.0f; }),
        particlesShoot.end());
}
void drawParticlesShoot() {
    for (const auto& particleShoot : particlesShoot) {
        //printf("Particle Position: %f, %f, %f\n", particle.position.x, particle.position.y, particle.position.z);
        drawIcohedron(particleShoot.position);
    }
}
void spawnParticleShoot() {
    ParticleShoot particleShoot;
    particleShoot.position = cameraPos;
    particleShoot.velocity = cameraFront * 10.0f;
    particleShoot.life = 3.0f;
    particlesShoot.push_back(particleShoot);
}

// Volcano Particle functions
void initializeParticlesVolcano(){
    particlesVolcano.clear();
}
void drawParticlesVolcano() {
    for (const auto& particleVolcano : particlesVolcano) {
        //printf("Particle Position: %f, %f, %f\n", particle.position.x, particle.position.y, particle.position.z);
        drawIcohedron(particleVolcano.position);
    }
}
void updateParticlesVolcano(float deltaTime){
    for (auto& particleVolcano : particlesVolcano) {
        particleVolcano.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * deltaTime;
        particleVolcano.position += particleVolcano.velocity * deltaTime;
        particleVolcano.life -= deltaTime;
    }

    // Remove dead particles
    particlesVolcano.erase(std::remove_if(particlesVolcano.begin(), particlesVolcano.end(),
        [](const ParticleVolcano& particleVolcano) { return particleVolcano.life <= 0.0f; }),
        particlesVolcano.end());
}
void spawnParticleVolcano(glm::vec3 volcanoTopPosition){
    ParticleVolcano particleVolcano;
    particleVolcano.position = volcanoTopPosition;
    glm::vec3 randomDirection = glm::normalize(glm::vec3(
        static_cast<float>(std::rand() % 100 - 50),  // Random X direction
        static_cast<float>(std::rand() % 50 + 50),   // Random Y direction (upwards)
        static_cast<float>(std::rand() % 100 - 50)   // Random Z direction
    ));
    particleVolcano.velocity = randomDirection * 20.0f;
    particleVolcano.life = 3.0f;
    particlesVolcano.push_back(particleVolcano);
}

// Cleanup Particles
void cleanupParticles() {
    particlesVolcano.clear();
    particlesShoot.clear();
}

CollisionBoxSelf collisionBoxSelf(cameraPos, glm::vec3(1.0f, 1.0f, 1.0f));

// Collision Detection
void updateCollisionBoxSelf(){
    collisionBoxSelf.position = cameraPos;
}
void setupCollisionBoxSelf(){
    collisionBoxSelf.position = cameraPos;
    collisionBoxSelf.size = glm::vec3(1.0f, 1.0f, 1.0f);
}
std::vector<CollisionBoxWalls> collisionBoxWalls; // Store wall locations and sizes
void setupCollisionBoxWalls(){
    for (const auto& location : cubeLocations) {
        glm::vec3 defaultSize(1.0f, 1.0f, 1.0f);
        collisionBoxWalls.push_back(CollisionBoxWalls(location, defaultSize));
    }
}
glm::vec3 prevCameraPos = cameraPos;
void updatePrevCameraPos(){
    prevCameraPos = cameraPos;
}
void testCollisionSelfWalls() {
    // Store the previous camera position
    glm::vec3 originalCameraPos = cameraPos;

    for (const auto& location : collisionBoxWalls) {
        if (collisionBoxSelf.position.x + collisionBoxSelf.size.x >= location.position.x - location.size.x &&
            collisionBoxSelf.position.x - collisionBoxSelf.size.x <= location.position.x + location.size.x &&
            collisionBoxSelf.position.z + collisionBoxSelf.size.z >= location.position.z - location.size.z &&
            collisionBoxSelf.position.z - collisionBoxSelf.size.z <= location.position.z + location.size.z) {
            screenCrack = true;
            cameraPos = prevCameraPos;
        }
    }
    prevCameraPos = originalCameraPos;
}

// Render function
void render() { 
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the specified shader program
    glUseProgram(shaderProgram);

    // Toggle wireframe mode with 'W' key
    if (qpress) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Filled mode
    }

    // 3D Space
    // Set up view and projection matrices
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1080.0f / 720.0f, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the cubes (Terrain Objects (Walls))
    // Draw 100 random cubes
    for (const auto& location : cubeLocations) {
        drawCube(location);
    }
    drawCat(glm::vec3(0.0f, 0.0f, -4.0f));
    drawPyramid(glm::vec3(-20.0f, 0.0f, -200.0f), glm::vec3(1.0f, 1.0f, 1.f));
    drawPyramid(glm::vec3(555.0f, 0.0f, -200.0f), glm::vec3(1.0f, 1.0f, 1.f));

    // Bonus Object Enemy Strafes left and right
    drawOctahedron(octahedronPos);
    if (octahedronRight) {octahedronPos.x += 0.2f;}
    else {octahedronPos.x -= 0.1f;}
    if (octahedronPos.x > 50.0f) {octahedronPos.x = -50.0f;}
    if (octahedronPos.x < -50.0f) {octahedronPos.x = 50.0f;}

    // float currentFrame = static_cast<float>(glfwGetTime());
    // deltaTime = currentFrame - lastFrame;
    // lastFrame = currentFrame;
    updateParticlesShoot(deltaTime);
    if(shoot){
        spawnParticleShoot();
    }
    drawParticlesShoot();   

    updateParticlesVolcano(deltaTime);
    if (lockon){
        spawnParticleVolcano(glm::vec3(-20.0f, 40.0f, -200.0f));
        spawnParticleVolcano(glm::vec3(555.0f, 40.0f, -200.0f));
    }
    drawParticlesVolcano();

    // Draw HUD Cube for 2d and object detection
    drawCubeHUD();

    updateCollisionBoxSelf();
    testCollisionSelfWalls();
    updatePrevCameraPos();

    // Draw 2D Space in Cube
    drawHorizon();
    drawCrossHair(lockon);
    if (screenCrack){drawScreenCrack();}    

    

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// Main function
int main() {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(1280, 720, "Assignment 7 - BattleZone", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Create and compile shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create and bind vertex arrays and buffers for the cube
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Create and bind vertex arrays and buffers for the pyramid
    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);
    glGenBuffers(1, &pyramidEBO);
    glBindVertexArray(pyramidVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Create and bind vertex arrays and buffers for the octahedron
    glGenVertexArrays(1, &octahedronVAO);
    glGenBuffers(1, &octahedronVBO);
    glGenBuffers(1, &octahedronEBO);
    glBindVertexArray(octahedronVAO);
    glBindBuffer(GL_ARRAY_BUFFER, octahedronVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(octahedronVertices), octahedronVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, octahedronEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(octahedronIndices), octahedronIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Create and bind vertex arrays and buffers for the tetrahedron
    glGenVertexArrays(1, &tetrahedronVAO);
    glGenBuffers(1, &tetrahedronVBO);
    glGenBuffers(1, &tetrahedronEBO);
    glBindVertexArray(tetrahedronVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tetrahedronVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronVertices), tetrahedronVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetrahedronEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedronIndices), tetrahedronIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Create and bind vertex arrays and buffers for the cat
    glGenVertexArrays(1, &catVAO);
    glGenBuffers(1, &catVBO);
    glGenBuffers(1, &catEBO);
    glBindVertexArray(catVAO);
    glBindBuffer(GL_ARRAY_BUFFER, catVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(catVertices), catVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, catEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(catIndices), catIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Create and bind vertex arrays and buffers for the icohedron
    glGenVertexArrays(1, &icohedronVAO);
    glGenBuffers(1, &icohedronVBO);
    glGenBuffers(1, &icohedronEBO);
    glBindVertexArray(icohedronVAO);
    glBindBuffer(GL_ARRAY_BUFFER, icohedronVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(icosahedronVertices), icosahedronVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icohedronEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(icosahedronIndices), icosahedronIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    
    // Set up view and projection matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // set up 2d
    setupCrossHairLockOn();
    setupCrossHairNoLock();
    setupScreenCrack(); 
    setupHorizon();

    // Set up collision self
    setupCollisionBoxSelf();

    // initialize the particles
    initializeParticlesShoot();
    initializeParticlesVolcano();

    // Set up 100 Random Cubes
    for (int i = 0; i < 100; i++) {
        float xCube = static_cast<float>(rand() % 200 - 100);
        float zCube = static_cast<float>(rand() % 200 - 100);
        cubeLocations.push_back(glm::vec3(xCube, 0.0f, zCube));
    }

    // Set up collision walls
    setupCollisionBoxWalls();

    while (!glfwWindowShouldClose(window)) {

        // Camera movement
        if (wKeyPressed) {
            cameraPos += cameraSpeed * cameraFront * deltaTime;
        }
        if (sKeyPressed) {
            cameraPos -= cameraSpeed * cameraFront * deltaTime;
        }
        if (aKeyPressed) {
            yaw -= 50.0f * deltaTime;
        }
        if (dKeyPressed) {
            yaw += 50.0f * deltaTime;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwSetKeyCallback(window, processInput);

        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup and terminate GLFW
    cleanupParticles();
    cleanup2D();
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
