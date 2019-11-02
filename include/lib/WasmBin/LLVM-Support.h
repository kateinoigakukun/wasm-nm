#pragma once

#include <vector>

namespace WasmBin {

struct WasmInitFunc {
  uint32_t Priority;
  uint32_t Symbol;
};

struct WasmDataReference {
  uint32_t Segment;
  uint32_t Offset;
  uint32_t Size;
};

struct WasmSymbolInfo {
  std::string Name;
  uint8_t Kind;
  uint32_t Flags;
  std::string ImportModule; // For undefined symbols the module of the import
  std::string ImportName;   // For undefined symbols the name of the import
  union {
    // For function or global symbols, the index in function or global index
    // space.
    uint32_t ElementIndex;
    // For a data symbols, the address of the data relative to segment.
    WasmDataReference DataRef;
  };
};

struct LinkingData
{
    uint32_t Version;
    std::vector<WasmInitFunc> InitFunctions;
    std::vector<std::string> Comdats;
    std::vector<WasmSymbolInfo> SymbolTable;
};

const unsigned WASM_SYMBOL_UNDEFINED = 0x10;
const unsigned WASM_SYMBOL_EXPLICIT_NAME = 0x40;


enum : unsigned {
  WASM_SEGMENT_INFO = 0x5,
  WASM_INIT_FUNCS = 0x6,
  WASM_COMDAT_INFO = 0x7,
  WASM_SYMBOL_TABLE = 0x8,
};

enum WasmSymbolType : unsigned {
  WASM_SYMBOL_TYPE_FUNCTION = 0x0,
  WASM_SYMBOL_TYPE_DATA = 0x1,
  WASM_SYMBOL_TYPE_GLOBAL = 0x2,
  WASM_SYMBOL_TYPE_SECTION = 0x3,
  WASM_SYMBOL_TYPE_EVENT = 0x4,
};
}
