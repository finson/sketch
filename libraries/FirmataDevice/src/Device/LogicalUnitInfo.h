#ifndef LogicalUnitInfo_h
#define LogicalUnitInfo_h

class LogicalUnitInfo {

public:
  LogicalUnitInfo();
  virtual ~LogicalUnitInfo();

  void *getDeviceObject();
  void setDeviceObject(void *obj);

private:
  void *theDeviceObject;
};

#endif
