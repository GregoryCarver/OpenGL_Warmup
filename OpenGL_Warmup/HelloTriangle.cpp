/*************************************************************************************************************************************************************************
* Gregory Carver
* CAP4730
* Last Edit: 2 Mar 2022
* Sources: https://learnopengl.com/Getting-started/Hello-Triangle, http://www.fredosaurus.com/notes-cpp/io/readtextfile.html
* TODO: Add reading in files, 
**************************************************************************************************************************************************************************/

// template based on material from learnopengl.com
#include <glew.h>
#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct ObjectData
{
    std::vector<float> vertices;
    std::vector<int> indices;

    ObjectData(const char* objectPath)
    {
        std::ifstream fileInput;
        fileInput.open(objectPath);
        std::string temp;
        while (fileInput >> temp)
        {
            if (temp == "v")
            {
                float x, y, z;
                fileInput >> x;
                fileInput >> y;
                fileInput >> z;
                vertices.push_back(x / 10);
                vertices.push_back(y / 10);
                vertices.push_back(z / 10);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
            }
            else if (temp == "f")
            {
                std::string first;
                std::string second;
                std::string third;
                std::string fourth;
                fileInput >> first;
                fileInput >> second;
                fileInput >> third;
                fileInput >> fourth;
                indices.push_back(std::stoi(first.substr(0, '/')));
                indices.push_back(std::stoi(second.substr(0, '/')));
                indices.push_back(std::stoi(third.substr(0, '/')));
                indices.push_back(std::stoi(first.substr(0, '/')));
                indices.push_back(std::stoi(third.substr(0, '/')));
                /*if (!fourth.empty() && !fourth.substr(0, '/').empty())
                {
                    for (int x = 0; x < fourth.substr(0, '/').size(); x++)
                    {
                        if (!std::isdigit(fourth.substr(0, '/')[x]))
                        {
                            continue;
                        }
                    }
                    
                }*/
                indices.push_back(std::stoi(fourth.substr(0, '/')));
            }
        }
    }
};

/******* Added shader class to load in shaders *******/
class Shader
{
    unsigned int shaderProgram;

public:
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        //Site used for syntax: http://www.fredosaurus.com/notes-cpp/io/readtextfile.html
        std::ifstream fileInput;
        std::string vertexCode = "";
        std::string fragmentCode = "";
        fileInput.open(vertexPath);
        if (!fileInput)
        {
            std::cout << "Vertex shader code failed to load from path: " << vertexPath << "!" << std::endl;
        }
        else
        {
            std::string temp;
            while (std::getline(fileInput, temp))
            {
                vertexCode += temp + "\n";
            }
        }
        fileInput.close();
        fileInput.open(fragmentPath);
        if (!fileInput)
        {
            std::cout << "Fragment shader code failed to load from path: " << fragmentPath << "!" << std::endl;
        }
        else
        {
            std::string temp;
            while (std::getline(fileInput, temp)) 
            {
                fragmentCode += temp + "\n";
            }
        }
        fileInput.close();

        const char* vertexShaderSource = vertexCode.c_str();
        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        const char* fragmentShaderSource = fragmentCode.c_str();
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Use()
    {
        glUseProgram(shaderProgram);
    }
    void Del()
    {
        glDeleteProgram(shaderProgram);
    }
};

class Camera
{

};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();

    /******* Custom shader class to load in shaders, and functions to use and delete the shader program *******/
    Shader shader = Shader("./source.vert", "./source.frag");
    //ObjectData cube = ObjectData("./soccerball.obj");

    /******* Added RGB values to vertices array, get's passed to vertex shader throuh attributes *******/
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = 
    {
        -0.5f, -.25f, 0.0f, 0.0f, 0.0f, 1.0f,  // left  
         0.5f, -.75f, 0.0f, 0.0f, 0.0f, 1.0f,  // right 
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // top   

        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.3f,  // left  
         0.5f, -0.5f, 0.0f, 0.0f, 0.3f, 0.0f,  // right 
         0.0f, -1.0f, 0.0f, 0.3f, 0.0f, 0.0f   // bottom
    };

    /******* Used https://learnopengl.com/Getting-started/Hello-Triangle for assistance implementing EBO *******/
    /******* Added EBO functionality *******/
    int indices[] =
    {
        0, 1, 2,
        3, 4, 5
    };

    //////////////////////////////////////TEST
    //for (int x = 0; x < cube.indices.size(); x++)
    //{
    //    std::cout << cube.indices[x] << std::endl;
    //}

    ////return 0;

    //float* vertices = &cube.vertices[0];
    //int* indices = &cube.indices[0];
    ///////////////////////////////////////////////TEST END

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int numVertices = sizeof(vertices) / 3;
    //unsigned int numVertices = cube.vertices.size() / 3;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, cube.vertices.size() * sizeof(float), vertices, GL_STATIC_DRAW);

    /******* Added bindings for EBO *******/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indices.size() * sizeof(int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        shader.Use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, numVertices);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  
    shader.Del();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}