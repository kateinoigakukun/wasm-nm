namespace Wasm
{

class Options
{
    bool imports;
    bool exports;
    bool privates;
    bool sizes;
};

class Symbol
{
    Options options;
};
}; // namespace Wasm