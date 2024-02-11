#pragma once

class MemoryOutputStream
{
private:
    unsigned m_length = 0;

    unsigned m_capacity = 0;

    char *m_data = nullptr;

    void reallocate(unsigned length);

public:
    MemoryOutputStream (const MemoryOutputStream &other) = delete;

    MemoryOutputStream operator=(const MemoryOutputStream &other) = delete;

    MemoryOutputStream();

    ~MemoryOutputStream();

    unsigned length() const;

    const char *data() const;

    void reset();

    void write(int data);

    void write(unsigned data);

    void write(long data);

    void write(bool data);

    void write(unsigned long data);

    void write(const void *data, unsigned length);
};
