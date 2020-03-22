#include <cstdio>
#include <cstdint>
#include <utility>

struct Data {
private:
    uint16_t age;
    uint16_t id;
    unsigned char* bytes;
public:
    Data(uint16_t _age, uint16_t _id)
        : age(_age)
        , id(_id)
        , bytes( nullptr )
    {
        printf("ctor\n");
    }

    Data( Data&& _data) {
        age = _data.age;
        id = _data.id;
        bytes = _data.bytes;
        //
        _data.bytes = nullptr;
        printf("move ctor\n");
    }

    ~Data(){
        if(bytes){
            delete []bytes;
            bytes = nullptr;
        }
    }

};

Data createData( uint16_t _age, uint16_t _id ) {
    Data data( _age, _id );
    return std::move(data);
}

void handleData( Data& _data ) {
    printf("ref\n");
}

void handleData( const Data& _data) {
    printf("const ref\n");
}

void handleData( Data&& _data ) {
    printf("rvalue ref\n");
}

class Base {
private:
public:
    Base( Data&& _data ) {
    }
};

class Derived : public Base {
public:
    Derived( Data&& _data )
        : Base( std::move(_data) ) {

    }
};

int main() {
    Data data1 = createData(16,1);
    //
    const Data data2 = createData(16,1);
    Data&& data = createData(16, 1);
    //
    handleData(std::move(data1));
    handleData(data2);
    handleData(createData(16,1));
    handleData(data);
    printf("hello,world!");
    return 0;
}