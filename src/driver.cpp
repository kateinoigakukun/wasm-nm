#include "lib/WasmBin/Deserialize.h"
#include <fstream>
#include <iostream>
#include "lib/WasmBin/Module.h"

int main(int argc, char *argv[]) {

    const char *binaryFilename = argv[1];
    std::ifstream input(binaryFilename);
    std::cout << binaryFilename << std::endl;
    WasmBin::Deserializer deserializer(input);
    auto module = deserializer.deserializeModule();
    if (module->Magic[0] != 0x00 ||
        module->Magic[1] != 0x61 ||
        module->Magic[2] != 0x73 ||
        module->Magic[3] != 0x6D) {
        std::cout << "Invalid magic" << std::endl;
        return 1;
    }

    for (auto it = std::begin(module->Sections); it != std::end(module->Sections); ++it) {
        switch ((*it)->getKind()) {
            case WasmBin::Section::Custom:
            {
                auto customSection = static_cast<WasmBin::CustomSection*>(*it);
                std::cout << customSection->name << std::endl;
                break;
            }
            default:
                break;
        }
    }
    return 0;
}
