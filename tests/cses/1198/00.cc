#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/eppstein.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    mld::dynamic_graph<std::pair<u32, u64>> g(n);
    g.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        g.add_edge(--u, --v, w);
    }

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::eppstein<mld::pmr::managers::reusing<>, decltype(g)> epp(g, 0, n - 1, pa);
    while (k--) std::cout << *epp.next() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
