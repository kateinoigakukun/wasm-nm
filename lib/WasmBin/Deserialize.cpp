#include "lib/WasmBin/Deserialize.h"
#include "lib/WasmBin/LLVM-Support.h"
#include <stdio.h>
#include <stdlib.h>
#include <tuple>
#include <assert.h>
#include <vector>
#include <iostream>

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

    if (name == "linking") {
        auto begin = stream.tellg();
        auto linkingData = LinkingData();
        linkingData.Version = deserializeVarUint32();
        uint8_t type;
        stream.read((char *)&type, 1);
        uint32_t size = deserializeVarUint32(); (void)size;

        switch (type) {
            case WASM_SYMBOL_TABLE:
            {
                auto symbolTable = deserializeSymbolTable();
                linkingData.SymbolTable = symbolTable;
                break;
            }
            default:
                break;
        }

        stream.seekg(begin);
        stream.ignore(payloadSize);
        return new LinkingSection(linkingData);
    } else {
        stream.ignore(payloadSize-nameLength);
    }
    return new CustomSection(name, std::vector<char>());
}


std::vector<WasmSymbolInfo> Deserializer::deserializeSymbolTable() {
    uint32_t length = deserializeVarUint32();
    auto symbolTable = std::vector<WasmSymbolInfo>();
    symbolTable.resize(length);

    while (length--) {
        auto info = WasmSymbolInfo();
        stream.read((char *)&info.Kind, 1);
        info.Flags = deserializeVarUint32();
        bool isDefined = (info.Flags & WASM_SYMBOL_UNDEFINED) == 0;

        switch (info.Kind) {
            case WASM_SYMBOL_TYPE_FUNCTION:
            {
                info.ElementIndex = deserializeVarUint32();
                if (isDefined) {
                    auto name = std::get<0>(deserializeString());
                    info.Name = name;
                } else {
                    if ((info.Flags & WASM_SYMBOL_EXPLICIT_NAME) != 0) {
                        info.Name = std::get<0>(deserializeString());
                    }
                }
                break;
            }
            case WASM_SYMBOL_TYPE_GLOBAL:
            {

                info.ElementIndex = deserializeVarUint32();
                if (isDefined) {
                    info.Name = std::get<0>(deserializeString());
                } else {
                    if ((info.Flags & WASM_SYMBOL_EXPLICIT_NAME) != 0) {
                        info.Name = std::get<0>(deserializeString());
                    }
                }
                break;
            }
            case WASM_SYMBOL_TYPE_DATA:
            {

                info.Name = std::get<0>(deserializeString());
                if (!isDefined) {
                    break;
                }

                uint32_t Index = deserializeVarUint32(); (void)Index;
                uint32_t Offset = deserializeVarUint32(); (void)Offset;
                uint32_t Size = deserializeVarUint32(); (void)Size;
                break;
            }
            case WASM_SYMBOL_TYPE_SECTION:
            {
                info.ElementIndex = deserializeVarUint32();
                break;
            }
            case WASM_SYMBOL_TYPE_EVENT:
            {
                info.ElementIndex = deserializeVarUint32();
                info.Name = std::get<0>(deserializeString());
                break;
            }
            default:
            {
                assert(false && "unimplemented");break;
            }
        }
        symbolTable.push_back(info);
    }
    return symbolTable;
}

std::tuple<std::string, size_t> Deserializer::deserializeString()
{
    auto length = deserializeVarUint32();
    char buf[length + 1];
    buf[length] = '\0';
    stream.read(buf, length);
    auto str = std::string(buf);
    return std::make_tuple(str, length);
}
uint32_t Deserializer::deserializeVarUint32()
{
    uint32_t res = 0;
    uint32_t shift = 0;

    while (true) {
        if (shift > 31) {
            assert(false && "Invalid VarUint32");
        }

        uint8_t b;
        stream.read((char *)&b, 1);
        res |= ((b & 0x7f) << shift);
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
