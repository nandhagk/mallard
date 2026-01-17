#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/eppstein.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, s, t, k;
    std::cin >> n >> m >> s >> t >> k;

    mld::dynamic_graph<std::pair<u32, u64>> g(n);
    g.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        g.add_edge(u, v, w);
    }

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::eppstein<mld::pmr::managers::clonable<>, decltype(g)> epp(g, s, t, pa);
    for (u32 i = 0; i < k; ++i)
        if (auto ans = epp.next())
            std::cout << *ans << '\n';
        else
            std::cout << -1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
