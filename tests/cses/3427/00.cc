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
        u32 u, v;
        std::cin >> u >> v;

        i32 a = u, b = v;
        lch.add(line((a - b) / i32(m), -a));
    }

    for (u32 i = 0; i <= m; ++i) std::cout << u64(-lch.query(i)(i)) << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
