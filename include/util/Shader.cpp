#include "util/Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

util::Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    /**
     * Read from glsl files
     * Create the const char* code
     */
    std::ifstream vStreamFile;
    std::ifstream fStreamFile;
    vStreamFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    fStreamFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    std::string vSrc;
    std::string fSrc;
    try
    {
        vStreamFile.open(vertexPath);
        fStreamFile.open(fragmentPath);

        std::stringstream vStream;
        std::stringstream fStream;

        vStream << vStreamFile.rdbuf();
        fStream << fStreamFile.rdbuf();

        vSrc = vStream.str();
        fSrc = fStream.str();

        vStreamFile.close();
        fStreamFile.close();
    }
    catch(const std::ios::failure f)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << f.what() << '\n';
    }

    const char* vCode = vSrc.c_str();
    const char* fCode = fSrc.c_str();
    
    /**
     * Compile the 2 shaders
     */
    GLuint vShader;
    GLuint fShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader,1,&vCode,nullptr);
    glShaderSource(fShader,1,&fCode,nullptr);

    int s;
    char log[512];

    /*Compilation for vertex shader*/
    glCompileShader(vShader);
    glGetShaderiv(vShader,GL_COMPILE_STATUS,&s);
    if (!s)
    {
        glGetShaderInfoLog(vShader,512,nullptr,log);
        std::cerr<< "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<log << '\n';
    }

    /*Compilation for fragment shader*/
    glCompileShader(fShader);
    glGetShaderiv(fShader,GL_COMPILE_STATUS,&s);
    if (!s)
    {
        glGetShaderInfoLog(fShader,512,nullptr,log);
        std::cerr<< "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<log << '\n';
    }

    /**
     * Link the two shader
     */
    m_id = glCreateProgram();
    glAttachShader(m_id,vShader);
    glAttachShader(m_id,fShader);
    glLinkProgram(m_id);
    glGetProgramiv(m_id,GL_LINK_STATUS,&s);
    if (!s)
    {
        std::cerr<< "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"<< log << '\n';
    }

    /*Delete the 2 shaders*/
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}


void util::Shader::use()
{
    glUseProgram(m_id);
}


void util::Shader::setBool(const std::string &name, bool value)
{         
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value); 
}


void util::Shader::setInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); 
}


void util::Shader::setFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); 
} 


GLuint util::Shader::getID()
{
    return m_id;
}


util::Shader::~Shader()
{
    glDeleteProgram(m_id);
}