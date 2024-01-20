/*
--------------------------------------------------------------------------------
PROJECT: SoCIN_Simulator
MODULE : No modules - Plugin Manager for the simulator and modules
FILE   : PluginManager.h
--------------------------------------------------------------------------------
DESCRIPTION: Classes to load dynamic libraries and instantiate and destroy
the components of the simulator.
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
CONTACT: Prof. Cesar Zeferino (zeferino@univali.br)
-------------------------------- Reviews ---------------------------------------
| Date       - Version - Author                      | Description
--------------------------------------------------------------------------------
| 23/09/2016 - 1.0     - Eduardo Alves da Silva      | Initial implementation
--------------------------------------------------------------------------------
*/
#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include <map>
#include <vector>
#include <systemc>

// Forward declarations - interfaces
class INoC;
class IRouter;
class IRouting;
class IInputFlowControl;
class IOutputFlowControl;
class IMemory;
class IPriorityGenerator;
class SoCINModule;

class PluginLoader {
private:
    void* libHandler;
    std::string fileName;
    std::string pluginName;
    bool loaded;

public:
    PluginLoader(std::string fileName, std::string pluginName);
    ~PluginLoader();

    bool load();
    bool unload();
    char* error();
    bool isLoaded();

    void* loadSymbol(std::string symbol);
};


class PluginManager {
private:
    PluginLoader* noc;
    PluginLoader* router;
    PluginLoader* routing;
    PluginLoader* flowControl;
    PluginLoader* memory;
    PluginLoader* priorityGenerator;

    std::vector<SoCINModule*> allocatedUnits;

    std::map<std::string, std::string> properties;
    bool pluginsLoaded;

    void parseProperty(char *line);

public:
    PluginManager();
    ~PluginManager();

    bool parseFile();
    bool loadPlugins();

    INoC* nocInstance(sc_core::sc_module_name name);

    IRouter* routerInstance(sc_core::sc_module_name name,
                            unsigned short ROUTER_ID,
                            unsigned short nPorts,
                            unsigned short nVirtualChannels);
    IRouting* routingInstance(sc_core::sc_module_name name,
                              unsigned short ROUTER_ID, unsigned short PORT_ID,
                              unsigned short nPorts);
    IInputFlowControl* inputFlowControlInstance(sc_core::sc_module_name name,
                                                unsigned short ROUTER_ID,
                                                unsigned short PORT_ID);
    IOutputFlowControl* outputFlowControlInstance(sc_core::sc_module_name name,
                                                  unsigned short ROUTER_ID,
                                                  unsigned short PORT_ID,
                                                  unsigned short bufferDepth);
    IMemory* memoryInstance(sc_core::sc_module_name name,
                            unsigned short ROUTER_ID,
                            unsigned short PORT_ID,
                            unsigned short memSize);
    IPriorityGenerator* priorityGeneratorInstance(sc_core::sc_module_name name,
                                                  unsigned short ROUTER_ID,
                                                  unsigned short PORT_ID,
                                                  unsigned short nPorts);

    void destroyNoC(INoC* );
    void destroyRouter(IRouter*);
    void destroyRouting(IRouting* );
    void destroyInputFlowControl(IInputFlowControl* );
    void destroyOutputFlowControl(IOutputFlowControl* );
    void destroyMemory(IMemory* );
    void destroyPriorityGenerator(IPriorityGenerator* );

    void deallocateUnits();

    void output_properties();

};



#endif // __PLUGINMANAGER_H__
