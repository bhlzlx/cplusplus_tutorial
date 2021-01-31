# C++17 PMR

## 编译器支持

| 编译器 | 版本 |
| - | - |
| MSVC | VS 2019(推荐)|
| GCC | 9.2|
| Clang | ？|

## 简单介绍

C++17 有如下优点

1. `STL`中甚至结合自定义内存池（DSA）能轻松处理内存对齐，这是`PMR`出现之前无法做到的
2. 针对不同类型的对象定制存储位置，且对象类型相同
   * 多线程情景，有一批对象永远只在同一线程中分配和释放，完全可以定制避免锁竞争（快！
   * 有部分对象生命周期只在栈内，且大小较小，使用栈内存即可（快！
   * `std::list`对象存储可以让节点以及节点内数据连续分配，提高`cache`命中率（快！
   * 即使不同的`allocator`对象，其类型也是相同的，可以通用（需要注意move语义的消耗
3. 减少内存碎片
4. 更易于收集分配信息

## 基本类型

1. std::pmr::polymorphic_allcoator\<T\>
2. std::pmr::meomory_resource
3. std::pmr::new_delete_resource
4. std::pmr::null_memory_resource
5. std::pmr::monotonic_buffer_resource
6. std::pmr::synchronized_pool_resource
7. std::pmr::unsynchronized_pool_resource

## 使用 PMR

### 自定义 `memory_resource`

```cpp
class CustomMemoryResource : public std::pmr::memory_resource {
private:
    std::string                 _name;
    std::pmr::memory_resource*  _upstream;
public:
    explicit DebugMemoryResource( std::string name, std::pmr::memory_resource* upstream = std::pmr::get_default_resource() )
        : _name(name)
        , _upstream(upstream)
    {
    }
private:
    virtual void* do_allocate( size_t size, size_t alignment ) override {
        // todo : return ptr;
    }

    virtual void do_deallocate(void* ptr, size_t size, size_t alignment) override {
    }
    // https://en.cppreference.com/w/cpp/memory/memory_resource/do_is_equal
    /*Two memory_resources compare equal if and only if memory allocated from one memory_resource can be deallocated from the other and vice versa.*/
    virtual bool do_is_equal(const memory_resource& other) const noexcept override {
    }
};
```

### 特化STL容器

以`std::vector`为例

```cpp
template<class T>
using pmr_vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
```

或直接使用

```cpp
std::pmr::vector
```

### 创建`allocator`实例，并初始化`STL容器`

```cpp
CustomMemoryResource memoryResource;
std::pmr::vector<ElementType> vecElements( &memoryResource );
```

就这么简单

## 技术要点

### 层层兜底

当我们指定一个分配器`a1`给特定的一批对象作为`分配器`使用的时候，如果`a1`容量不够，那没办法，只能返回空指针，但是我们又不希望逻辑层判断，没关系，我们可以拿另外一个`a2`给它兜底（将`a2`传入`a1`的构造函数里即可），当`a1`内存不足的时候自动从`a2`继续分配。

`分配器`构造函数里默认使用`get_default_resource`,而`get_default_resource`默认又是`new_delete_resource`。

我们可以利用这个特性收集分配信息，评估内存使用量，保证内存分配不会失败。

另外，还内置了`null_memory_resource`，想严格控制内存分配行为，可以给指定`分配器`设置上层兜底的`分配器`，当前分配器分配失败的时候立刻抛`std::bad_alloc`异常。

### 其它常见的默认分配器

```cpp
std::pmr::monotonic_buffer_resource
std::pmr::synchronized_pool_resource
std::pmr::unsynchronized_pool_resource
```

#### std::pmr::monotonic_buffer_resource

使用情景之一，如果我们经常在函数栈内创建一个 `std::vector` 它不只会占用栈上内存，还会从堆中分配内存去存储元素，但是元素个数并不多，我们希望它直接使用栈内存，这样速度更快，对`std::list`这样的效果尤其明显，可以提高`cache`命中率！。可以书写如下代码：

```cpp
 uint8_t buffer[2048];
std::pmr::monotonic_buffer_resource stackPool( std::data(buffer), std::size(buffer), &debugMemoryResource);
std::pmr::list<char> charList( &stackPool );
for( uint32_t i = 0; i<('z'-'a'); ++i) {
    charList.push_back('a' + i);
}
```

然后，我们使用GDB命令：  
`-exec x/256c buffer`  
可以观察到内存内容

#### std::pmr::synchronized_pool_resource

这个无需过多解释，其实就是线程安全（有锁）的内存分配器

#### std::pmr::unsynchronized_pool_resource

这个是非线程安全的内存分配器

### std::move 的行为

因为pmr的分配器是有状态的，比如

```cpp
std::pmr::vector<uint32_t> v1{ {1,2,3,4}, &allocator };
std::pmr::vector<uint32_t> v2 = std::move(v1); // 有复制开销
```

虽然是`move`语义，但是这两个的allocator是不一样的，所以它仍然有复制开销。我们稍微改一下，就能避免这个复制开销。

```cpp
std::pmr::vector<uint32_t> v1{ {1,2,3,4}, &allocator };
std::pmr::vector<uint32_t> v2( std::move(v1), &allocator ) ; // 没有复制开销
```

## 复杂一些的情况

`STL`容器只会按元素内存布局去分配内存,如果元素内有成员是`std::string`这种非POD类型怎么办？添加 `allocator_type` 类型！  
示例如下：

```cpp
class ComplexDataType {
private:
    size_t              _id;
    std::pmr::string    _name;
public:
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    allocator_type get_allocator() {
        return _name.get_allocator();
    }

    ComplexDataType( allocator_type allocator = {} )
        : _id(0)
        , _name(allocator)
    {
    }
    ComplexDataType( ComplexDataType&& other, allocator_type allocator = {} ) noexcept
        : _id(other._id)
        , _name( std::move(other._name), allocator )
    {
    }
    ComplexDataType( size_t id, const std::pmr::string& name, allocator_type allocator = {} )
        : _id(0)
        , _name(name, allocator)
    {}
};

constexpr size_t SizeofComplexDataType = sizeof(ComplexDataType);

DebugMemoryResource debugMemoryResource("simple debug resource");

int main() {

    std::byte stackBuffer[1024*1024]; // 1MB buffer
    std::pmr::monotonic_buffer_resource stackResource (
        std::data(stackBuffer),
        std::size(stackBuffer),
        std::pmr::null_memory_resource()
    );
    DebugMemoryResource debugMR("debug mr", &stackResource);
    std::pmr::vector<ComplexDataType>::allocator_type allocator( &debugMR );
    std::pmr::vector<ComplexDataType> v1(allocator);
    v1.emplace_back(1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    v1.emplace_back(2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    return 0;
}
```

输出：

```log
debug mr:[Allocate] 48 bytes            --> 一个元素
debug mr:[Allocate] 33 bytes            --> aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
debug mr:[Allocate] 96 bytes            --> 扩容
debug mr:[Allocate] 33 bytes            --> bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
debug mr:[Deallocate] 48 bytes          --> 回收一个元素时候的内存
debug mr:[Deallocate] 33 bytes          --> v1 析构
debug mr:[Deallocate] 33 bytes          --> v1 析构
debug mr:[Deallocate] 96 bytes          --> v1 析构
```
