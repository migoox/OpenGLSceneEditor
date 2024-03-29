#include "Renderer.h"
#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0), m_LoadingSuccess(false)
{
    ShaderProgramSource source = ParseShader(filepath);

    //std::cout << "VERTEX" << std::endl << source.VertexSource << std::endl;
    //std::cout << "FRAGMENT" << std::endl << source.FragmentSource << std::endl;

    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

    GLCall( glUseProgram(m_RendererID) );
}

Shader::~Shader()
{
    GLCall( glDeleteProgram(m_RendererID) );
}

void Shader::Bind() const
{
    GLCall( glUseProgram(m_RendererID) );
}

void Shader::Unbind() const
{
    GLCall( glUseProgram(0) );
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall( int location = glGetUniformLocation(m_RendererID, name.c_str()) );
    if (location == -1)
        std::cout << "[OpenGL] Warning: No active uniform variable with name " << name << " found" << std::endl;

    m_UniformLocationCache[name] = location;

    return location;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall( glUniform1i(GetUniformLocation(name), value) );
}

void Shader::SetUniform2i(const std::string& name, int v1, int v2)
{
    GLCall( glUniform2i(GetUniformLocation(name), v1, v2) );
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall( glUniform1f(GetUniformLocation(name), value) );
}

void Shader::SetUniform2f(const std::string& name, float f0, float f1)
{
    GLCall( glUniform2f(GetUniformLocation(name), f0, f1) );
}

void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
{
    GLCall( glUniform3f(GetUniformLocation(name), f0, f1, f2) );
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 vec)
{
    GLCall( glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z) );
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GLCall( glUniform4f(GetUniformLocation(name), f0, f1, f2, f3) );
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 vec)
{
    GLCall( glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w) );
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall( glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]) );
}

enum ShaderType
{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

struct ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = NONE;

    if (!stream.is_open())
    {
        std::cout << "[Shader]: " << "Error: Cannot load shader: " << filepath << "\n";
        m_LoadingSuccess = false;
    }
    else
    {
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = FRAGMENT;
            }
            else
            {
                ss[(int)type] << line << '\n';
            }
        }
        m_LoadingSuccess = true;
    }
    stream.close();

    struct ShaderProgramSource sps = { ss[0].str(), ss[1].str() };
    return sps;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall( unsigned int id = glCreateShader(type) );
    const char* src = source.c_str();
    GLCall( glShaderSource(id, 1, &src, nullptr) );
    GLCall( glCompileShader(id) );

    // Error handling
    int result;
    GLCall( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
    std::cout << "[Shader]: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " << result << std::endl;
    if ( result == GL_FALSE )
    {
        int length;
        GLCall( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) );
        char* message = (char*) alloca(length * sizeof(char));
        GLCall( glGetShaderInfoLog(id, length, &length, message) );
        std::cout << "[Shader]: "
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << std::endl;
        std::cout << message << std::endl;
        GLCall( glDeleteShader(id) );
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // create a shader program
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall( glAttachShader(program, vs) );
    GLCall( glAttachShader(program, fs) );

    GLCall( glLinkProgram(program) );

    GLint program_linked;

    GLCall( glGetProgramiv(program, GL_LINK_STATUS, &program_linked) );
    std::cout << "[Shader]: Program link status: " << program_linked << std::endl;
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall( glGetProgramInfoLog(program, 1024, &log_length, message) );
        std::cout << "[Shader]: Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall( glValidateProgram(program) );

    GLCall( glDeleteShader(vs) );
    GLCall( glDeleteShader(fs) );

    return program;
}
