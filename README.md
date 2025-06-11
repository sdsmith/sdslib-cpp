# Helpers

For experimenting with object lifetime, use the following and run the code. Most helpful for godbolt/compiler explorer and small experiments.

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
