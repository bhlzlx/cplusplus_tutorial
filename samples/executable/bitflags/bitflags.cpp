#include <cstdio>
#include <cstdint>
#include <type_traits>

enum class texture_usage_flag_bits : uint8_t {
    color_attachment = 1,
    depth_attachment = 2,
    stencil_attachment = 4,
    shader_read_only = 8,
    shader_write_only = 16,
};

using texture_usage_t = uint8_t;
constexpr texture_usage_t typical_usage = (uint8_t)texture_usage_flag_bits::color_attachment | (uint8_t)texture_usage_flag_bits::shader_read_only;

template<typename Enum, typename Value, typename ISENUM = std::enable_if_t<std::is_enum<Enum>::value, bool>>
class BitFlags {
private:
    Value _value;
public:
    template<typename... Args>
    constexpr BitFlags(Args... args)
        : _value(create<Enum>(args...))
    {}
    template<typename T, typename ...Args>
    constexpr Value create( T t, Args... args ) const {
        return create(t) | create<T>(args...);
    }
    template<typename T>
    constexpr Value create( T t ) const {
        return (Value)t;
    }
    template<typename T>
    constexpr Value create() const{
        return 0;
    }
    constexpr bool has(Enum e) const {
        return (_value & create(e)) != 0;
    }
    void conbine(const BitFlags& other) {
        _value |= other._value;
    }
    operator Value() const {
        return _value;
    }
};

constexpr BitFlags<texture_usage_flag_bits, uint8_t> typical_usage_flags(texture_usage_flag_bits::color_attachment, texture_usage_flag_bits::shader_read_only);

int main() {
    static_assert(typical_usage_flags.has(texture_usage_flag_bits::color_attachment), "has color_attachment");
    uint8_t val = typical_usage_flags;
    return 0;
}