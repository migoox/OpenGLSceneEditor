#pragma once
#include <iostream>
#include "Debug.h"

class Texture
{
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline std::string GetFilePath() const { return m_FilePath; }

    bool IsLoadingSucceded() { return m_LoadingSuccess; }
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    bool m_LoadingSuccess;
};