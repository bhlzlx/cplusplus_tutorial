#include <cstdint>
#include <cstdio>
#include <ctime>
#include <random>

/* RAII */

enum class Action {
    Enter,
    Exit
};

struct Msg {
    char const* func;
    Action      action;
};

class Test {
    class Helper {
        std::vector<Msg>* _msgs;
        char const* _func;
    public:
        Helper(std::vector<Msg>* msgs, char const* func) : _msgs(msgs), _func(func) {
            _msgs->push_back({_func, Action::Enter});
        }
        ~Helper() {
            _msgs->push_back({_func, Action::Exit});
        }
    };
private:
    std::vector<Msg> _msgs;
public:
#define TRACE_ON 1
#if TRACE_ON
#define TRACE(func) Helper _helper(&_msgs, func)
#else
#define TRACE(func) 
#endif
    void matchFunction(bool flag) {
        TRACE(__FUNCTION__);
        if(flag) {
            return ;
        }
        if(!matchArgs()) {
            return;
        }
        switch(matchIdentifier()) {
            case 1:
                return matchStatement0();
            case 2:
                return matchStatement1();
                break;
            case 3:
                return matchStatement2();
                break;
            default:
                int a = 0;
                while(a<10) {
                    if(!matchIdentifier()) {
                        return;
                    }
                    a++;
                }
                break;
        }
        return ;
    }

    bool matchArgs() {
        TRACE(__FUNCTION__);
        return true;
    }

    int matchIdentifier() {
        TRACE(__FUNCTION__);
        return 1;
    }

    void matchStatement0() {
        TRACE(__FUNCTION__);
    }

    void matchStatement1() {
        TRACE(__FUNCTION__);
    }

    void matchStatement2() {
        TRACE(__FUNCTION__);
    }

    Test() {
        printf("Test()\n");
    }
    ~Test() {
        printf("~Test()\n");
    }
};

int main() {
    Test test;    
    test.matchFunction(false);
    return 0;
}