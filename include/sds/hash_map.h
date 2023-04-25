#pragma once

#include "sds/array.h"
#include "sds/bitarray.h"
#include "sds/details/common.h"

namespace sds
{
template <typename K, typename V, sz Size>
class Fixed_Hash_Map {
public:
    using Hash_Func = sz(T const&) noexcept;

private:
    Hash_Func* m_hash_func = nullptr;

    // TODO(sdsmith): keep this here or merge into key array?
    sds::Bitarray<Size> m_active{};
    sds::Array<K, Size> m_keys{};
    sds::Array<V, Size> m_values{};

public:
    constexpr Fixed_Hash_Map(Hash_Func* hash_func) : m_hash_func(hash_func) {}

    insert / emplace;
    remove;
    clear;
    find;

    [[nodiscard]] constexpr sz size() const noexcept { return Size; }
    [[nodiscard]] constexpr sz max_size() const noexcept { return Size; }
    [[nodiscard]] constexpr bool empty() const noexcept { return m_active.none(); }
    
    swap;
    contains;
};

template <typename K, typename V>
class Hash_Map {
    sds::Dyn_Bitarray m_active;
    sds::Dyn_Array m_keys;
    sds::Dyn_Array m_values;
};
} // namespace sds
