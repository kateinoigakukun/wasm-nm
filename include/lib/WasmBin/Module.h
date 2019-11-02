#pragma once

#include <stdint.h>
#include <vector>
#include "lib/WasmBin/LLVM-Support.h"


namespace WasmBin
{

class Section
{
public:
    enum Kind
    {
        Custom,
        Type,
        Import,
        Function,
        Table,
        Memory,
        Global,
        Export,
        Start,
        Element,
        Code,
        Data,
    };

    virtual const Kind getKind() = 0;
};

class CustomSection : public Section
{
public:
    std::string name;
    std::vector<char> payload;

    CustomSection(std::string name, std::vector<char> payload)
    : name(name), payload(payload) {}

    const Kind getKind() { return Kind::Custom; }
};

class LinkingSection: public CustomSection
{
public:
    LinkingSection(LinkingData data)
    : CustomSection("linking", std::vector<char>()), data(data) { }
    LinkingData data;
};

class TypeSection : public Section
{
    const Kind getKind() { return Kind::Type; }
};

class Module
{
public:
    char Magic[4];
    uint32_t Version;
    std::vector<Section *> Sections;
};


}; // namespace WasmBin
