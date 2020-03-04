#include <cstdio>
#include <cstdint>
#include <cstring>

// 函数示例
uint32_t factorial( uint32_t _value ) {
    uint32_t v = _value;
    while(--_value) {
        v *= _value;
    }
    return v;
}

// 结构体示例
struct Student {
private:
    char    m_name[32];
    int     m_age;
    int     m_id;
public:
    Student()
        : m_name("")
        , m_age(0)
        , m_id(0) {
        //
    }
    //
    int 
    getAge(){
        return m_age;
    }
    //
    void
    setAge( int _age ) {
        this->m_age = _age;
    }
    //
    const char* 
    getName() {
        return m_name;
    }
    //
    void 
    setName( const char* _name ) {
        strncpy( m_name, _name, 32 );
    }
    //
    int 
    getID() {
        return m_id;
    }
    //
    void 
    setID( int _id ) {
        m_id = _id;
    }
};

// 类示例
class CStudent {
protected:
    char    m_name[32];
    int     m_age;
    int     m_id;
public:
    CStudent()
        : m_name("")
        , m_age(0)
        , m_id(0) {
        //
    }
    //
    int 
    getAge(){
        return m_age;
    }
    //
    void
    setAge( int _age ) {
        this->m_age = _age;
    }
    //
    const char* 
    getName() {
        return m_name;
    }
    //
    void 
    setName( const char* _name ) {
        strncpy( m_name, _name, 32 );
    }
    //
    int 
    getID() {
        return m_id;
    }
    //
    void 
    setID( int _id ) {
        m_id = _id;
    }
};

enum Type {
    StudentType = 5,
    TeacherType,
};

union ComposedData {
    char    bytes[4];
    int     ivalue;
};

int main() {
    char c = 'a';
    const char* message1 = "hello,world";
    char message2[] = "hello,world";
    bool b = true;
    // bool b = false;
    printf("%s, %s\n",message1, message2);
    printf("%c\n", c);
    {
        int d = 128;
        printf("%d\n", d);
    }

    CStudent stu;
    stu.setAge(5);
    int age = stu.getAge();
    printf("%d\n", age);
    // printf( "stu => age = %d, id = %d, name = %s", stu.age, stu.id, stu.name );
    Type type = Type::StudentType;
    //
    ComposedData u;
    constexpr int composeDataSize = sizeof(u);
    u.bytes[0] = u.bytes[1] = u.bytes[2] = u.bytes[3] = 0;
    printf("%d\n", u.bytes[0]);
    u.ivalue = 0xffffffff;
    printf("%d\n", u.bytes[0]);
    return 0;
}