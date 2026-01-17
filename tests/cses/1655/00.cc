#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binary_trie.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::binary_trie<u32, mld::pmr::managers::reusing<>, 1> bt(pa);
    bt.insert(0);

    u32 z = 0;
    for (auto &&x : a) bt.insert(z ^= x);

    u32 ans = 0;

    z = 0;
    for (auto &&x : a) ans = std::max(ans, bt.xor_max(z) ^ z), z ^= x;

    std::cout << ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
