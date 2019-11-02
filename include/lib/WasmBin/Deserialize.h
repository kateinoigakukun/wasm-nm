#pragma once

#include <fstream>
#include "Module.h"

namespace WasmBin
{
class Deserializer
{
public:
    Deserializer(std::ifstream &stream)
        : stream(stream)
    {
    }
    Module *deserializeModule();

    Section *deserializeSection();
    CustomSection *deserializeCustomSection(uint32_t size);
    std::vector<WasmSymbolInfo> deserializeSymbolTable();


    uint64_t deserializeVarUint64();
    uint32_t deserializeVarUint32();
    uint8_t  deserializeVarUint7();

    uint32_t deserializeUint32();

    std::tuple<std::string, size_t> deserializeString();


private:
    std::ifstream &stream;
};
}; // namespace WasmBin
