#ifndef SHADER_H // include gaurds, stopping the header from being included multiple times in the same compilation unit.
#define SHADER_H

#include <glad/glad.h>

#include <string> // allows use of std::string (c++ string class)
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{

    public:
        unsigned int ID; // we want the shader to have an ID
        
        //Constructor will take a file path for the shaders I want to include
        Shader(const char* vertexPath, const char* fragmentPath);

        // activate the shader
        void use();

        // utility uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
};

#endif