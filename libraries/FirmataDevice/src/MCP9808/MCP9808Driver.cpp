  #include "MCP9808Driver.h"

  MCP9808Driver::MCP9808Driver(char *dName, int baseAddr, int addrCount){}

  int MCP9808Driver::open(char *name){
    return open(name,0);
  }
  int MCP9808Driver::open(char *name, int flags){
    return -1;
  }

  int MCP9808Driver::status(int handle, int reg, int count, byte *buf){
    return -1;
  }

  int MCP9808Driver::control(int handle, int reg, int count, byte *buf){
    return -1;
  }

  int MCP9808Driver::read(int handle, int count, byte *buf){
    return -1;
  }
  int MCP9808Driver::write(int handle, int count, byte *buf){
    return -1;
  }

  int MCP9808Driver::close(int handle){
    return -1;
  }
