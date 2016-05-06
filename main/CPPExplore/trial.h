class A {
public:
A();
enum class B : int {
    PIN = 0,
    NEEDLE = 1
  };
char *myName;
};


#define JAMIT(scope) \
myName = strdup(scope);

A::A()  {
  
  JAMIT(__func__)
  Serial.println(myName);

}
