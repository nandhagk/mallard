#include <bits/stdc++.h>

#include "lib/binary_trie.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> c;
    c.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.push_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::vector<u32> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    for (u32 u : h.tor | std::views::drop(1)) d[u] ^= d[h.par[u]];

    static std::array<std::byte, (1 << 29) - (1 << 26)> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::binary_trie<u32, mld::pmr::managers::reusing<>> bt0(pa), bt1(pa);
    for (u32 u = 0; u < n; ++u)
        if (h.dep[u] % 2)
            bt1.insert(d[u]);
        else
            bt0.insert(d[u]);

    u32 z = 0;
    while (q--) {
        char t;
        std::cin >> t;

        if (t == '^') {
            u32 y;
            std::cin >> y;

            z ^= y;
        } else {
            u32 u, x;
            std::cin >> u >> x;

            u32 a = d[--u] ^ x;

            u32 ans = 0;
            if (h.dep[u] % 2) {
                a ^= z;
                bt1.erase(d[u], 1);

                if (bt0.size()) ans = std::max(ans, a ^ bt0.xor_max(a));
                a ^= z;
                if (bt1.size()) ans = std::max(ans, a ^ bt1.xor_max(a));

                bt1.insert(d[u]);
            } else {
                bt0.erase(d[u], 1);

                if (bt0.size()) ans = std::max(ans, a ^ bt0.xor_max(a));
                a ^= z;
                if (bt1.size()) ans = std::max(ans, a ^ bt1.xor_max(a));

                bt0.insert(d[u]);
            }

            std::cout << ans << ' ';
        }
    }

    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
