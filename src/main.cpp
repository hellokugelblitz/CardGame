#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// some basic functionality -- this is a callback for the window resize
/*
    There are many callbacks functions we can set to register our 
    own functions. For example, we can make a callback function to 
    process joystick input changes, process error messages etc. We 
    register the callback functions after we've created the window 
    and before the render loop is initiated. 
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window) 
{   
    // example of user input handling.
    // if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    // {
    //     glfwSetWindowShouldClose(window, true);
    // }
}

unsigned int compileVertexShader() 
{
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // checking for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return vertexShader;
}

// Fragment shader colors the scene (comes after the vertex shader...)
unsigned int compileFragmentShader() 
{
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
        "}\n";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    return fragmentShader;
}

unsigned int compileShaderProgram() 
{
    unsigned int vertexShader = compileVertexShader();
    unsigned int fragmentShader = compileFragmentShader();

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // error handling
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int createTriangle() 
{
    float triangle_vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    }; 

    unsigned int VBO; // AI added VAO not sure what that is.
    //glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // return VAO; // return the VAO for rendering
    return VBO; // delete this
}

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "CardGame", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // init for GLAD...
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // doing this so that OpenGL knows how we want to display the data coordinates with respect to the window.
    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // triangle and renderer stuff




    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input handling (at the start of every iteration)
        processInput(window);
    
        // rendering commands go here
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f); // background color this is "clearing" any empty space with this color
        glClear(GL_COLOR_BUFFER_BIT);

        // check and call events and swap the buffers
        glfwPollEvents(); // generally enables checks for keyboard and mouse inputs
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
