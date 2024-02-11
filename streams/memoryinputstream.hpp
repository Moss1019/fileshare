#pragma once

class MemoryInputStream
{
private:
    unsigned m_currentPos = 0;

    unsigned m_length = 0;

    char *m_data = nullptr;

public:
    MemoryInputStream(const MemoryInputStream &other) = delete;

    MemoryInputStream operator=(const MemoryInputStream &other) = delete;

    MemoryInputStream(const char *data, unsigned length);

    ~MemoryInputStream();

    unsigned remaining() const;

    unsigned length() const;

    void read(int *data);

    void read(unsigned *data);

    void read(long *data);

    void read(bool *data);

    void read(unsigned long *data);

    void read(void *data, unsigned length);
};
