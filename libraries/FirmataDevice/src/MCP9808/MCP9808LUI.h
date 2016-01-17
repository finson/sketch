#ifndef MCP9808LUI_h
#define MCP9808LUI_h

class MCP9808LUI: public LogicalUnitInfo {

public:

  MCP9808LUI() : LogicalUnitInfo(), i2cAddress(-1) {}

  ~MCP9808LUI() {}

  void setI2CAddress(const int addr) {
    i2cAddress = addr;
  }

  const int getI2CAddress() {
    return i2cAddress;
  }

private:
  int i2cAddress;
};

#endif
