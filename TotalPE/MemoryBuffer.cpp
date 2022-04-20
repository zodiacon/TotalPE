#include "pch.h"
#include "MemoryBuffer.h"

MemoryBuffer::MemoryBuffer(uint32_t initialSize) : m_buffer(initialSize) {

}

MemoryBuffer::MemoryBuffer(const uint8_t* data, uint32_t size) : m_buffer(size) {
    ::memcpy(m_buffer.data(), data, size);
}

uint32_t MemoryBuffer::GetData(int64_t offset, uint8_t* buffer, uint32_t count) {
    if (count + offset > GetSize())
        count = max(0, int32_t(GetSize() - offset));
    if (count)
        ::memcpy(buffer, m_buffer.data() + offset, count);
    return count;
}

bool MemoryBuffer::Insert(int64_t offset, const uint8_t* data, uint32_t count) {
    m_buffer.insert(m_buffer.begin() + offset, data, data + count);
    return true;
}

bool MemoryBuffer::Delete(int64_t offset, size_t count) {
    m_buffer.erase(m_buffer.begin() + offset, m_buffer.begin() + offset + count);
    return true;
}

bool MemoryBuffer::SetData(int64_t offset, const uint8_t* data, uint32_t count) {
    if (offset + count >= (int64_t)m_buffer.size())
        m_buffer.resize(offset + count);
    ::memcpy(m_buffer.data() + offset, data, count);
    return true;
}

bool MemoryBuffer::SetData(int64_t offset, uint8_t value, uint32_t count) {
    if (offset + count >= (int64_t)m_buffer.size())
        m_buffer.resize(offset + count);
    ::memset(m_buffer.data() + offset, value, count);
    return true;
}

int64_t MemoryBuffer::GetSize() const {
    return m_buffer.size();
}

uint8_t* MemoryBuffer::GetRawData(int64_t offset) {
    return m_buffer.data() + offset;
}

bool MemoryBuffer::IsReadOnly() const {
    return false;
}
