/*
 * Copyright [2024] [Shuang Zhu / Sol]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef TA_BUFFER_H
#define TA_BUFFER_H

#include <sstream>
#include <fstream>
#include <vector>

#include "TA_EndianConversion.h"

namespace CoreAsync {

class TA_BufferReader;
class TA_BufferWriter;

template <typename Opt>
class TA_BasicBufferOperator
{
public:
    TA_BasicBufferOperator() = delete;

    TA_BasicBufferOperator(const std::string &file, std::size_t size) : m_buffer(size)
    {
        init(file);
    }

    virtual ~TA_BasicBufferOperator()
    {
        if(m_fileStream.is_open())
            m_fileStream.close();
    }

    TA_BasicBufferOperator(const TA_BasicBufferOperator &op) = delete;
    TA_BasicBufferOperator(TA_BasicBufferOperator &&op) = delete;

    bool isValid() const
    {
        return m_fileStream.is_open();
    }

    template <typename T>
    bool read(T t)
    {
        static_assert(std::is_same_v<Opt, TA_BufferReader>, "Read is not the member of current type");
        return static_cast<Opt *>(this)->read(t);
    }

    template <typename T>
    bool write(T t)
    {
        static_assert(std::is_same_v<Opt, TA_BufferWriter>, "Write is not the member of current type");
        return static_cast<Opt *>(this)->write(t);
    }

    void flush()
    {
        static_assert(std::is_same_v<Opt, TA_BufferWriter>, "Flush is not the member of current type");
        return static_cast<Opt *>(this)->flush();
    }

private:
    void init(const std::string &file)
    {
        static_cast<Opt *>(this)->init(file);
    }

protected:
    std::fstream m_fileStream;
    std::vector<char> m_buffer;

};


class TA_BufferReader : public TA_BasicBufferOperator<TA_BufferReader>
{
    friend class TA_BasicBufferOperator<TA_BufferReader>;
public:
    TA_BufferReader() = delete;
    TA_BufferReader(const std::string &file, std::size_t size);
    ~TA_BufferReader();

    TA_BufferReader(const TA_BufferReader &writer) = delete;
    TA_BufferReader(TA_BufferReader &&writer) = delete;

    template <EndianConvertedType T>
    bool read(T t)
    {
        if(!isValid() || m_fileStream.eof() || m_fileStream.fail() || m_fileStream.bad())
            return false;
        if(m_iStrStream.str().empty() || m_validSize - m_iStrStream.tellg() < sizeof(t))
        {
            if(!fillBuffer() || m_validSize < sizeof(t))
                return false;
        }
        m_iStrStream.read(reinterpret_cast<char*>(&t), sizeof(t));
        return true;
    }

private:
    void init(const std::string &file);
    bool fillBuffer();

private:
    std::istringstream m_iStrStream {};
    std::size_t m_validSize {0};

};

class TA_BufferWriter : public TA_BasicBufferOperator<TA_BufferWriter>
{
    friend class TA_BasicBufferOperator<TA_BufferWriter>;
public:
    TA_BufferWriter() = delete;
    TA_BufferWriter(const std::string &file, std::size_t size);

    ~TA_BufferWriter();

    TA_BufferWriter(const TA_BufferWriter &writer) = delete;
    TA_BufferWriter(TA_BufferWriter &&writer) = delete;

    TA_BufferWriter & operator = (const TA_BufferWriter &writer) = delete;
    TA_BufferWriter & operator = (TA_BufferWriter &&writer) = delete;

    template <EndianConvertedType T>
    void write(T t)
    {
        if(m_oStrStream.tellp() + static_cast<std::streampos>(sizeof(t)) > static_cast<std::streampos>(m_buffer.size()))
        {
            flush();
        }
        m_oStrStream.write(reinterpret_cast<const char *>(&t), sizeof(t));
    }

    void flush();

private:
    void init(const std::string &file);

private:
    std::ostringstream m_oStrStream;

};

using BufferReader = TA_BasicBufferOperator<TA_BufferReader>;
using BufferWriter = TA_BasicBufferOperator<TA_BufferWriter>;

}


#endif // TA_BUFFER_H
