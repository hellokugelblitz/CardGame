// glad is needed since modern openGL is not a library you link against directly... 
// the openGL driver exposes function pointers at runtime

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include <Shader.h>

// helper input function definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int generateFrag(const char *shaderSource);
unsigned int generateShaderProgram(unsigned int fragmentShader, unsigned int vertexShader);

// program settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// for system output messages
int success;
char infoLog[512];

// Controles
const unsigned int WIREFRAME_KEY = GLFW_KEY_GRAVE_ACCENT;

// app settings
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n" // really important line -- here we are syaing the shaders expects per-vertex data
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSourceGreen = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.8f, 0.2f, 1.0f);\n"
    "}\n\0";

int main() {
    // glfw window init with some slight error "handling"
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // some configurations... here we are telling it to using glfw to use version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // tell GLFW we want to explicitly use the core-profile.

    // creating the window object... holds all the "windowing" data and is passed frquently as an arg for glfw's other functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // doing this so that OpenGL knows how we want to display the data coordinates with respect to the window.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // init for GLAD...
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // create vertex shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    //checking vertex shader for errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // CREATING FRAGMENT SHADERS
    unsigned int fragmentShader = generateFrag(fragmentShaderSource);
    unsigned int fragmentShaderGreen = generateFrag(fragmentShaderSourceGreen);

    // CREATING SHADER PROGRAM WITH OUR SHADER CLASS
    Shader firstShader("../shaders/shaderlearningv.glsl", "../shaders/shaderlearningf.glsl");
    Shader secondShader("../shaders/shaderlearningv.glsl", "../shaders/greentest.glsl");

    // vector specification
    float triangle_one[] = {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    }; 

    float triangle_two[] = {
    //  x      y     z
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    };

    // triangle rendering...

    // Generate our vertex specification stuff
    unsigned int VertexBufferOne, VertexArrayOne, VertexBufferTwo, VertexArrayTwo;
    glGenVertexArrays(1, &VertexArrayOne);
    glGenVertexArrays(1, &VertexArrayTwo);
    glGenBuffers(1, &VertexBufferOne);
    glGenBuffers(1, &VertexBufferTwo);
    
    // ------------------------ triangle one ------------------------

    // bind our VAO and VBO
    glBindVertexArray(VertexArrayOne); // tell openGL that we are now using this array
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferOne);

    // This line here is super important. It captures the currently bound GL_ARRAY_BUFFER and saves it as a reference, linking the two
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 0 is not true/false here it is Gluint index for which attribute we want to enable (in this case position)

    // insert data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_one), triangle_one, GL_STATIC_DRAW);

    // ------------------------ triangle two ------------------------

    // BIND & SET VAO ATRRIBUTES
    glBindVertexArray(VertexArrayTwo);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferTwo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // LOAD DATA INTO SECOND BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_two), triangle_two, GL_STATIC_DRAW);
    
    glBindVertexArray(0); // unbinds the vertex array entirely.

    // render loop 
    while (!glfwWindowShouldClose(window)) {
        // input handling (at the start of every iteration)
        processInput(window);
    
        // basic rendering commands go here
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f); // background color this is "clearing" any empty space with this color
        glClear(GL_COLOR_BUFFER_BIT);

        // triangles
        // glUseProgram(shaderProgram);
        firstShader.use();

        // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VertexArrayOne);
        // glBindBuffer(GL_ARRAY_BUFFER, VertexBufferOne);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glUseProgram(shaderProgramGreen);
        secondShader.use();

        glBindVertexArray(VertexArrayTwo);
        // glBindBuffer(GL_ARRAY_BUFFER, VertexBufferTwo);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents(); // generally enables checks for keyboard and mouse inputs
    }

    // release everything (this is technically optional)
    glDeleteVertexArrays(1, &VertexArrayOne);
    glDeleteBuffers(1, &VertexBufferOne);
    glDeleteVertexArrays(1, &VertexArrayTwo);
    glDeleteBuffers(1, &VertexBufferTwo);

    // glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    // close window and terminate stuff
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// ------------------------------ Input Function Definitions ---------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window)
{   
    // example of user input handling.
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // W makes the window into wireframe mode...
    if(glfwGetKey(window, WIREFRAME_KEY) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, WIREFRAME_KEY) == GLFW_RELEASE)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

// ------------------------------ Shader Function Definitions ---------------------------

unsigned int generateFrag(const char *shaderSource)
{
    unsigned int newFrag = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(newFrag, 1, &shaderSource, nullptr);
    glCompileShader(newFrag);

    // fragment shader error handling
    glGetShaderiv(newFrag, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(newFrag, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return newFrag;
}

unsigned int generateShaderProgram(unsigned int fragmentShader, unsigned int vertexShader)
{
    // create shader program and attavh thingys
    unsigned int newShaderProgram;
    newShaderProgram = glCreateProgram(); // creating the graphics pipeline that will be used for rendering to 2d screen

    // attaching the shaders
    glAttachShader(newShaderProgram, vertexShader);
    glAttachShader(newShaderProgram, fragmentShader);
    glLinkProgram(newShaderProgram);

    // error handling for shader program creation
    glGetProgramiv(newShaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(newShaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); // we want to free up this space afterwards
    glDeleteShader(fragmentShader);

    return newShaderProgram;
}

