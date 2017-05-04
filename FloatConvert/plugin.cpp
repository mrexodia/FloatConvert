#include "plugin.h"
#include "ldconvert\ldconvert.h"

#ifdef _WIN64
#pragma comment(lib, "ldconvert/ldconvert_x64.lib")
#else
#pragma comment(lib, "ldconvert/ldconvert_x86.lib")
#endif //_WIN64

static bool cbCommand(int argc, char* argv[])
{
    if(argc < 2)
    {
        _plugin_logputs("Usage: FloatConvert 3.14159265");
        return false;
    }
    unsigned char data[10];
    memset(data, 0, sizeof(data));
    if(!str2ld(argv[1], data))
    {
        _plugin_logputs("Conversion failed...");
        return false;
    }
    duint setting = 0;
    if(!BridgeSettingGetUint("Gui", "FpuRegistersLittleEndian", &setting) || !setting)
        for(size_t i = 0; i < sizeof(data) / 2; i++)
        {
            char temp = data[i];
            data[i] = data[sizeof(data) - i - 1];
            data[sizeof(data) - i - 1] = temp;
        }
    std::string result;
    for(auto ch : data)
    {
        char byte[3];
        sprintf_s(byte, "%02X", ch);
        result += byte;
    }
    _plugin_logputs(result.c_str());
    return true;
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
    if(!_plugin_registercommand(pluginHandle, PLUGIN_NAME, cbCommand, false))
        _plugin_logputs("[" PLUGIN_NAME "] Error registering the \"" PLUGIN_NAME "\" command!");
    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop()
{
    return true;
}

//Do GUI/Menu related things here.
void pluginSetup()
{
}
