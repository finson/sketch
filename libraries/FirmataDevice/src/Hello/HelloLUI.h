#ifndef HelloLUI_h
#define HelloLUI_h

class HelloLUI: public LogicalUnitInfo {

public:

    HelloLUI() : LogicalUnitInfo() {
        who = strdup("Earthlings");
    }

    HelloLUI(const char *to) : LogicalUnitInfo() {
        who = strdup(to);
    }

    ~HelloLUI() {
        free(who);
    }

    void setWho(const char *newWho) {
        free(who);
        who = strdup(newWho);
    }

    const char *getWho() {
        return who;
    }

private:
    char *who;
};

#endif
