#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binary_trie.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::binary_trie<u32, mld::pmr::managers::reusing<>, 3> bt(pa);
    for (auto &&x : a) bt.insert(x);

    while (q--) {
        char t;
        std::cin >> t;

        if (t == '!') {
            u32 k, x;
            std::cin >> k >> x;

            bt.erase(a[--k], 1);
            bt.insert(a[k] = x);
        } else {
            u32 x, y;
            std::cin >> x >> y;

            std::cout << bt.rank(y + 1) - bt.rank(x) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
