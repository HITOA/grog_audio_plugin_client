#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cctype>

//Extremely simple rdf ttl writer

struct Predicate;
struct Object;

struct Prefix {
    std::string uri = "";
    std::string name = "";
};

class Subject {
public:
    Subject() = default;
    Subject(const std::string& subject) : subject{ subject } {}
    ~Subject() {}

    std::shared_ptr<Predicate> CreatePredicate(const std::string& predicate) {
        std::shared_ptr<Predicate> p = std::make_shared<Predicate>(predicate);
        predicates.push_back(p);
        return p;
    }

public:
    std::string subject = "";
    std::vector<std::shared_ptr<Predicate>> predicates{};
};

class Predicate {
public:
    Predicate() = default;
    Predicate(const std::string& predicate) : predicate{ predicate } {}
    ~Predicate() {}

    Predicate* CreateObject(const std::string& object) {
        std::shared_ptr<Object> o = std::make_shared<Object>(object);
        objects.push_back(o);
        return this;
    }

    std::shared_ptr<Subject> CreateBNode() {
        std::shared_ptr<Subject> s = std::make_shared<Subject>();
        bnodes.push_back(s);
        return s;
    }

public:
    std::string predicate = "";
    std::vector<std::shared_ptr<Object>> objects{};
    std::vector<std::shared_ptr<Subject>> bnodes{}; //subject's string will be ignored and should be empty
};

class Object {
public:
    Object() = default;
    Object(const std::string& object) : object{ object } {}
    ~Object() {}

public:
    std::string object;
};

class RDF {
public:
    RDF() = default;
    ~RDF() {}

    std::shared_ptr<Subject> CreateSubject(const std::string& subject) {
        std::shared_ptr<Subject> s = std::make_shared<Subject>(subject);
        subjects.push_back(s);
        return s;
    }

    void CreatePrefix(const std::string& uri, const std::string& name) {
        std::shared_ptr<Prefix> p = std::make_shared<Prefix>();
        p->uri = uri;
        p->name = name;
        prefixes.push_back(p);
    }

    void WriteFile(const std::string& path) {
        if (subjects.size() < 1)
            return;
        
        std::string s{};

        for (size_t i = 0; i < prefixes.size(); ++i) {
            s += "@prefix ";
            s += prefixes[i]->name;
            s += ": ";
            s += prefixes[i]->uri;
            s += ".\n";
        }

        if (prefixes.size() > 0)
            s += "\n";

        size_t i = 0;
        while (true) {
            s += SerializeSubject(subjects[i]);
            ++i;
            if (i >= subjects.size())
                break;
            s += "\n";
        }
        
        std::ofstream f{ path, std::ios::trunc };
        f << s;
        f.close();
    }

private:
    std::string AddIndent(const std::string& str) {
        std::string r = "\t";
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == '\n') {
                r += "\n\t";
            } else {
                r += str[i];
            }
        }
        return r;
    }

    std::string SerializeSubject(std::shared_ptr<Subject> subject) {
        if (subject->predicates.size() < 1)
            return "";
        std::string s = subject->subject + "\n";

        size_t i = 0;
        while (true) {
            std::string p = AddIndent(SerializePredicate(subject->predicates[i]));
            s += p;
            ++i;
            if (i >= subject->predicates.size()) {
                s += ".";
                break;
            }
            s += ";\n";
        }

        return s;
    }

    std::string SerializePredicate(std::shared_ptr<Predicate> predicate) {
        std::string s = predicate->predicate;

        bool indent = predicate->bnodes.size() > 0 || predicate->objects.size() > 5;
        if (indent)
            s += "\n";
        
        size_t i = 0;
        while (predicate->objects.size() > 0) {
            std::string p = SerializeObject(predicate->objects[i]);
            if (indent)
                p = AddIndent(p);
            else
                p = " " + p;
            s += p;
            ++i;
            if (i >= predicate->objects.size()) {
                break;
            }
            s += ",";
            if (indent)
                s += "\n";
        }

        i = 0;
        while (predicate->bnodes.size() > 0) {
            std::string p = SerializeBNode(predicate->bnodes[i]);
            if (indent)
                p = AddIndent(p);
            else
                p = " " + p;
            s += p;
            ++i;
            if (i >= predicate->bnodes.size()) {
                break;
            }
            s += ",";
            if (indent)
                s += "\n";
        }

        return s;
    }

    std::string SerializeObject(std::shared_ptr<Object> object) {
        return object->object;
    }

    std::string SerializeBNode(std::shared_ptr<Subject> bnode) {
        std::string s = "[\n";

        size_t i = 0;
        while (true) {
            std::string p = AddIndent(SerializePredicate(bnode->predicates[i]));
            s += p;
            s += ";\n";
            ++i;
            if (i >= bnode->predicates.size())
                break;
        }

        s += "]";

        return s;
    }

private:
    std::vector<std::shared_ptr<Prefix>> prefixes{};
    std::vector<std::shared_ptr<Subject>> subjects{};
};

std::string MakeTTLURI(const std::string& str) {
    return std::string{ "<" } + str + ">";
}

std::string MakeTTLString(const std::string& str) {
    return std::string{ "\"" } + str + "\"";
}

std::string MakeSymbol(const std::string& str) {
    std::string r = "";
    for (size_t i = 0; i < str.size(); ++i) {
        if (isalnum(str[i])) {
            r += tolower(str[i]);
        } else if (isspace(str[i])) {
            r += "_";
        }
    }
    return r;
}

uint32_t AddAudioPort(std::shared_ptr<Predicate> portPredicate, const std::string& name, 
    Grog::AudioBuffer* buffer, bool input, uint32_t idx) {

    static const char* suffix[32] = { nullptr };
    static const char* pg[32] = { nullptr };

    switch (buffer->GetType()) {
        case Grog::AudioBufferType::Mono:
            suffix[0] = "";
            pg[0] = "pg:center";
            break;
        case Grog::AudioBufferType::Stereo:
            suffix[0] = "Left";
            suffix[1] = "Right";
            pg[0] = "pg:left";
            pg[1] = "pg:right";
            break;
        default:
            break;
    }

    for (uint32_t i = 0; i < buffer->GetChannelCount(); ++i) {
        std::string fullname = name + " " + suffix[i];
        std::string symbol = MakeSymbol(fullname);

        auto bnode = portPredicate->CreateBNode();

        if (input)
            bnode->CreatePredicate("a")->CreateObject("lv2:InputPort")->CreateObject("lv2:AudioPort");
        else
            bnode->CreatePredicate("a")->CreateObject("lv2:OutputPort")->CreateObject("lv2:AudioPort");

        bnode->CreatePredicate("lv2:index")->CreateObject(std::to_string(idx + i));
        bnode->CreatePredicate("lv2:symbol")->CreateObject(MakeTTLString(symbol));
        bnode->CreatePredicate("lv2:name")->CreateObject(MakeTTLString(fullname));
        bnode->CreatePredicate("lv2:designation")->CreateObject(pg[i]);
    }

    return buffer->GetChannelCount();
}

void AddControlPort(std::shared_ptr<Predicate> portPredicate, Grog::ControlPort* port, uint32_t idx) {
    std::string fullname = port->GetName();
    std::string symbol = MakeSymbol(fullname);

    auto bnode = portPredicate->CreateBNode();

    bnode->CreatePredicate("a")->CreateObject("lv2:InputPort")->CreateObject("lv2:ControlPort");

    bnode->CreatePredicate("lv2:index")->CreateObject(std::to_string(idx));
    bnode->CreatePredicate("lv2:symbol")->CreateObject(MakeTTLString(symbol));
    bnode->CreatePredicate("lv2:name")->CreateObject(MakeTTLString(fullname));
    bnode->CreatePredicate("lv2:default")->CreateObject(std::to_string(port->GetDefaultValue()));
    bnode->CreatePredicate("lv2:minimum")->CreateObject(std::to_string(port->GetMinimumValue()));
    bnode->CreatePredicate("lv2:maximum")->CreateObject(std::to_string(port->GetMaximumValue()));
}

void AddMidiPort(std::shared_ptr<Predicate> portPredicate, Grog::MidiBuffer* port, uint32_t idx) {
    std::string fullname = "Midi In";
    std::string symbol = MakeSymbol(fullname);

    auto bnode = portPredicate->CreateBNode();

    bnode->CreatePredicate("a")->CreateObject("lv2:InputPort")->CreateObject("atom:AtomPort");

    bnode->CreatePredicate("atom:bufferType")->CreateObject("atom:Sequence");
    bnode->CreatePredicate("atom:supports")->CreateObject("midi:MidiEvent");
    bnode->CreatePredicate("lv2:index")->CreateObject(std::to_string(idx));
    bnode->CreatePredicate("lv2:symbol")->CreateObject(MakeTTLString(symbol));
    bnode->CreatePredicate("lv2:name")->CreateObject(MakeTTLString(fullname));
}

int main(int argc, char** argv) {
    if (argc < 5)
        return -1;
    
    std::string targetFileDir = argv[1];
    std::string targetFileName = argv[2];
    std::string targetName = argv[3];
    std::string targetLV2URI = argv[4];

    RDF manifest{};
    manifest.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/lv2core#"), "lv2");
    manifest.CreatePrefix(MakeTTLURI("http://www.w3.org/2000/01/rdf-schema#"), "rdfs");
    auto manifestPluginSubject = manifest.CreateSubject(MakeTTLURI(targetLV2URI));
    manifestPluginSubject->CreatePredicate("a")->CreateObject("lv2:Plugin");
    manifestPluginSubject->CreatePredicate("lv2:binary")->CreateObject(MakeTTLURI(targetFileName));
    manifestPluginSubject->CreatePredicate("rdfs:seeAlso")->CreateObject(MakeTTLURI(targetName + ".ttl"));

    manifest.WriteFile(targetFileDir + "/manifest.ttl");

    Grog::AudioPlugin* pluginInstance = Grog::InstantiatePlugin();
    
    RDF plugin{};
    
    plugin.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/lv2core#"), "lv2");
    plugin.CreatePrefix(MakeTTLURI("http://usefulinc.com/ns/doap#"), "doap");
    plugin.CreatePrefix(MakeTTLURI("http://www.w3.org/1999/02/22-rdf-syntax-ns#"), "rdf");
    plugin.CreatePrefix(MakeTTLURI("http://www.w3.org/2000/01/rdf-schema#"), "rdfs");
    plugin.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/ext/atom#"), "atom");
    plugin.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/ext/port-props#"), "props");
    plugin.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/extensions/units#"), "units");
    plugin.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/ext/urid#"), "urid");
    plugin.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/ext/port-groups#"), "pg");
    plugin.CreatePrefix(MakeTTLURI("http://lv2plug.in/ns/ext/midi#"), "midi");

    auto pluginSubject = plugin.CreateSubject(MakeTTLURI(targetLV2URI));
    pluginSubject->CreatePredicate("a")->CreateObject("lv2:Plugin");
    pluginSubject->CreatePredicate("doap:name")->CreateObject(MakeTTLString(pluginInstance->GetName()));

    auto pluginRequiredFeaturePredicate = pluginSubject->CreatePredicate("lv2:requiredFeature");
    pluginRequiredFeaturePredicate->CreateObject("urid:map");

    auto pluginOptionalFeaturePredicate = pluginSubject->CreatePredicate("lv2:optionalFeature");
    pluginOptionalFeaturePredicate->CreateObject("lv2:hardRTCapable");

    auto pluginPortPredicate = pluginSubject->CreatePredicate("lv2:port");

    uint32_t portIdx = 0;

    if (pluginInstance->GetAudioInputBuffer())
        portIdx += AddAudioPort(pluginPortPredicate, "Audio In", pluginInstance->GetAudioInputBuffer(), true, portIdx);

    if (pluginInstance->GetAudioOutputBuffer())
        portIdx += AddAudioPort(pluginPortPredicate, "Audio Out", pluginInstance->GetAudioOutputBuffer(), false, portIdx);

    for (uint32_t i = 0; i < pluginInstance->GetControlPortCount(); ++i)
        AddControlPort(pluginPortPredicate, pluginInstance->GetControlPort(i), portIdx++);

    if (pluginInstance->GetMidiInputBuffer())
        AddMidiPort(pluginPortPredicate, pluginInstance->GetMidiInputBuffer(), portIdx++);

    plugin.WriteFile(targetFileDir + "/" + targetName + ".ttl");

    return 0;
}