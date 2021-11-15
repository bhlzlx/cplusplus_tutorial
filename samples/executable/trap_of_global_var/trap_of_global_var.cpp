#include <cstdio>
#include <cstdlib>
#include <vector>

class ClazzSys {
private:
    std::vector<int> _member;
public:
    ClazzSys()
        : _member {1,2,3,4,5}
    {
    }
    void add( int val ) {
        return _member.push_back(val);
    }
};
ClazzSys* GetSys();

class ClazzInv {
private:
    std::vector<int> _member;
public:
    ClazzInv() {
        GetSys()->add(0);
    }
    const std::vector<int>& get() const {
        return _member;
    }
};

static ClazzInv inv;
static ClazzSys sys;

ClazzSys* GetSys() {
    return &sys;
}

// template<class T>
// class PlaceHolder {
// private:
//     uint8_t _opaque[sizeof(T)];
// public:
//     T& ref() {
//         return *(T*)_opaque[0];
//     }
//     template<class ...ARGS>
//     void initialize(ARGS&&...args) {
//         new((void*)_opaque) T(std::forward(args)...);
//     }
//     void destroy() {
//         &ref()->~T();
//     }
// };

// PlaceHolder<ClazzSys> sys1;
// PlaceHolder<ClazzSys> inv1;

int main( int argc, char const** argv ) {
    // sys1.initialize();
    // inv1.initialize();
    // sys1.ref().add(0);
    return 0;
}
