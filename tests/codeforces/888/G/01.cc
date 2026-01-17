#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binary_trie.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

// TODO: Why is this so slow?
void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::union_find uf(n);
    u32 z = std::bit_width(n);

    static std::array<std::byte, (1 << 28) - (1 << 25)> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    mld::binary_trie<u64, mld::pmr::managers::reusing<>, 3> bt(pa);
    auto key = [&](u32 u) { return u64(a[u]) << z | u; };
    auto idx = [&](u32 k) { return k & ((1 << z) - 1); };

    for (u32 u = 0; u < n; ++u) bt.insert(key(u));

    std::vector<u64> d;
    std::vector<std::pair<u32, u32>> e, f;
    d.reserve(n), e.reserve(n), f.reserve(n);

    u64 ans = 0;
    while (uf.ccs() != 1) {
        e.clear(), f.clear();
        for (u32 u = 0; u < n; ++u) e.emplace_back(uf.find(u), u);

        mld::static_csr g(n, e);
        for (auto &&cc : g) {
            if (cc.empty()) continue;

            for (u32 u : cc) bt.erase(key(u));

            d.clear();
            for (u32 u : cc) d.push_back(bt.xor_min(key(u)) ^ key(u));
            u32 k = std::ranges::min_element(d) - d.begin();
            f.emplace_back(cc[k], idx(d[k] ^ key(cc[k])));

            for (u32 u : cc) bt.insert(key(u));
        }

        for (auto &&[u, v] : f)
            if (uf.merge(u, v)) ans += a[u] ^ a[v];
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
