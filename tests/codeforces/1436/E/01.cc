#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/static_mex.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, e.emplace_back(--x, e.size());

    for (u32 i = 0; i <= n; ++i) e.emplace_back(i, n);

    std::ranges::reverse(e);
    mld::static_csr g(n + 1, e);

    static std::array<std::byte, (1 << 28) - (1 << 25)> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    mld::static_mex<mld::pmr::managers::clonable<>> sm(a, pa);
    for (u32 i = 0; i <= n; ++i) {
        u32 prv = 0;
        for (u32 cur : g[i]) {
            if (prv < cur && sm.mex(prv, cur) == i) goto found;
            prv = cur + 1;
        }

        std::cout << i + 1 << '\n';
        return;

    found:;
    }

    std::cout << n + 2 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
