#include "Texture.h"

#include "../vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path) :
    m_RendererID(0U),
    m_FilePath(path),
    m_LocalBuffer(nullptr),
    m_Width(0),
    m_Height(0),
    m_BPP(0),
    m_LoadingSuccess(false)
{
    // 1 means true, we want to flip image, since opengl expects (0, 0) coordinate to be in the left bottom, where images
    // have it usualy set in the left top
    stbi_set_flip_vertically_on_load(1);

    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall( glGenTextures(1, &m_RendererID) );
    GLCall( glBindTexture(GL_TEXTURE_2D, m_RendererID) );   // bind this texture to unit 0
                                                            // "unit" is also called "slot" sometimes

    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );

    if (!m_LocalBuffer)
    {
        std::cout << "[STBI]: " << "Couldn't load texture " << path << ".\n";
    }
    else
    {
        GLCall( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer) );

        GLCall( glGenerateMipmap(GL_TEXTURE_2D) );

        m_LoadingSuccess = true;
    }


    Unbind();

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
};

Texture::~Texture()
{
    GLCall( glDeleteTextures(1, &m_RendererID) );
}

void Texture::Bind(unsigned int slot) const
{
    GLCall( glActiveTexture(GL_TEXTURE0 + slot) );
    GLCall( glBindTexture(GL_TEXTURE_2D, m_RendererID) );
}

void Texture::Unbind() const
{
    GLCall( glBindTexture(GL_TEXTURE_2D, 0) );
}

