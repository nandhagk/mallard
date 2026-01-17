#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binary_trie.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::binary_trie<u64, mld::pmr::managers::reusing<>, 3> bt(pa);

    auto key = [&](u32 i) { return u64(a[i]) << 18 | i; };

    u64 ans = 0;
    for (u32 i = 0; i < k - 1; ++i) {
        ans += bt.size() - bt.rank(key(i));
        bt.insert(key(i));
    }

    for (u32 i = k - 1; i < n; ++i) {
        ans += bt.size() - bt.rank(key(i));
        bt.insert(key(i));

        std::cout << ans << ' ';

        ans -= bt.rank(key(i - k + 1));
        bt.erase(key(i - k + 1), 1);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
