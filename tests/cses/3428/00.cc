#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lichao_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    using line = mld::internal::line<i64>;
    mld::lichao_tree<u32, line, mld::pmr::managers::reusing<>> lch(0, m + 1, pa);

    while (n--) {
        u32 p, u, q, v;
        std::cin >> p >> u >> q >> v;

        i64 a = u, b = v, s = p, t = q;
        lch.add(p, q + 1, line((a - b) / (t - s), (b * s - a * t) / (t - s)));
    }

    for (u32 i = 0; i <= m; ++i)
        if (auto l = lch.query(i); l != line())
            std::cout << -l(i) << ' ';
        else
            std::cout << -1 << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
