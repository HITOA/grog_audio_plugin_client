#include <iostream>
#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>


int main(int argc, char** argv) {
    if (argc < 5)
        return -1;
    
    const char* targetFileDir = argv[1];
    const char* targetFileName = argv[2];
    const char* targetName = argv[3];
    const char* targetLV2URI = argv[4];

    Grog::AudioPlugin* plugin = Grog::InstantiatePlugin();
    Grog::PortManager manager{};
    plugin->InitializePort(manager);

    return 0;
}