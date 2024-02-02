#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float x_mod, y_mod, z_mod;

void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_mod += 0.1f;
    }


}




int main(void)
{
    GLFWwindow* window;

   

    

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Jairo Jaropojop", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;

    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //create vertex shader(used for movements)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &v, NULL);

    glCompileShader(vertexShader);

   
   


   



    //create frag shader (our objects are turned into pixels/fragments which we will use to color in an object)
   GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragShader, 1, &f, NULL);

    glCompileShader(fragShader);

    //create shader program that'll just run both frag and vert together as one.
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);//compile to make sure computer remembers

   




    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool sucess = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );


    //get the  indices array OF mesh that will be placed into vbo
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index);
    }


    GLfloat vertices[]{//def of 3d mdel
        //xyz
        0.f, 0.5,0.f,
        -0.5f, -0.5f,0.f,
        0.5f,-0.5f,0.f
    };

    GLuint indices[] = {
        0,1,2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //null = VAO, VBO
    //VAO <- VBO

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        attributes.vertices.data(),//&attributes.vretices[0]
        GL_STATIC_DRAW//if not moving do static dynamic for moving
    );

    glVertexAttribPointer(
        0,//0 pos
        3, //xyz
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    //VBO=VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//VBO <-EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW);

    


   



    glEnableVertexAttribArray(0);
    //SET BINDINGS TO NULL
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //x_mod += 0.001f;


        unsigned int xLoc = glGetUniformLocation(shaderProg, "x");

        glUniform1f(xLoc, x_mod);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
       
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}