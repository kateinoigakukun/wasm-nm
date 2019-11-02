#include "lib/WasmBin/Deserialize.h"
#include "lib/WasmBin/LLVM-Support.h"
#include <stdio.h>
#include <stdlib.h>
#include <tuple>
#include <assert.h>
#include <vector>

namespace WasmBin
{
Module *Deserializer::deserializeModule()
{
    auto module = new Module();
    stream.read(module->Magic, 4);
    module->Version = deserializeUint32();
    while (!stream.eof())
    {
        auto section = deserializeSection();
        if (section)
            module->Sections.push_back(section);
    }
    return module;
}

Section *Deserializer::deserializeSection()
{
    char id;
    stream.read(&id, 1);
    auto size = deserializeVarUint32();
    switch (id)
    {
    case 0:
        return deserializeCustomSection(size);
    default:
        stream.ignore(size);
        break;
    }
    return nullptr;
}



CustomSection *Deserializer::deserializeCustomSection(uint32_t payloadSize)
{
    std::string name;
    size_t nameLength;
    std::tie(name, nameLength) = deserializeString();

    std::vector<char> buf;
    buf.resize(payloadSize-nameLength);
    stream.read((char *)&buf[0], payloadSize-nameLength);
    return new CustomSection(name, buf);
}

std::tuple<std::string, size_t> Deserializer::deserializeString()
{
    auto length = deserializeVarUint32();
    char buf[length];
    stream.read(buf, length);
    return std::make_tuple(std::string(buf), length);
}
uint32_t Deserializer::deserializeVarUint32()
{
    uint32_t res = 0;
    uint32_t shift = 0;

    while (true) {
        if (shift > 31) {
            assert(false && "Invalid VarUint32");
        }

        char b;
        stream.read(&b, 1);
        res |= (b & 0x7f) << shift;
        shift += 7;
        if ((b >> 7) == 0) {
            break;
        }
    }
    return res;
}

uint32_t Deserializer::deserializeUint32()
{
    uint32_t result = 0;
    char bytes[4];
    stream.read(bytes, 4);
    for (int i = 0; i < 4; i++)
    {
        result += bytes[i] << (i * 8);
    }
    return result;
}

}; // namespace WasmBin
