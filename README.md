# Helpers

## Object lifetime
For experimenting with object lifetime, use the following and run the code. Most helpful for godbolt/compiler explorer and small experiments.

C++23:
```cpp
#include <stacktrace>
#include <source_location>
#include <iostream>

void print_context_impl(const std::source_location &loc = std::source_location::current()) noexcept {
    std::cout << loc.function_name() << std::stacktrace::current(2, 1);
}

//#define print_context(...) 
#define print_context(...) print_context_impl(__VA_ARGS__)

class Lifetime {
public:
    Lifetime() noexcept { print_context(); }
    Lifetime(int) noexcept { print_context(); }
    Lifetime(const Lifetime&) noexcept { print_context(); }
    Lifetime(Lifetime&&) noexcept { print_context(); }
    ~Lifetime() noexcept { print_context(); }
    Lifetime& operator=(const Lifetime&) noexcept { print_context(); return *this; }
    Lifetime& operator=(Lifetime&&) noexcept { print_context(); return *this; }
};
```

Previous standards:
```cpp
class Lifetime {
public:
    Lifetime() noexcept { std::puts(__FUNCTION__); }
    Lifetime(int) noexcept { std::puts(__FUNCTION__); }
    Lifetime(const Lifetime&) noexcept { std::puts(__FUNCTION__); }
    Lifetime(Lifetime&&) noexcept { std::puts(__FUNCTION__); }
    ~Lifetime() noexcept { std::puts(__FUNCTION__); }
    Lifetime& operator=(const Lifetime&) noexcept { std::puts(__FUNCTION__); return *this; }
    Lifetime& operator=(Lifetime&&) noexcept { std::puts(__FUNCTION__); return *this; }
};
```
