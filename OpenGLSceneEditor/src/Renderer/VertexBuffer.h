#pragma once

class VertexBuffer
{
    private:
        unsigned int m_RendererID;

    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void BufferData(const void* data, unsigned int size);
        void BufferSubData(const void* data, unsigned int size, unsigned int offset);

        void Bind() const;
        void Unbind() const;
};
