#ifndef SHADER_H
#define SHADER_H
#include <string>

#include "GLEW/glew.h"

namespace util
{
    class Shader
    {
        private:
        GLuint m_id;


        public:
        Shader(const char* vertexPath, const char* fragmentPath);


        void use();


        /**
         * Only set when called use() before
         */
        void setBool(const std::string &name, bool value);


        void setFloat(const std::string &name, float value);


        void setInt(const std::string &name, int value);


        GLuint getID();


        ~Shader();

    };
}
#endif