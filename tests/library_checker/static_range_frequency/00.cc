#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/managers/reusing.h"
#include "lib/ordered_set.h"
#include "lib/prelude.h"
#include "ska_sort.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    std::vector<std::pair<u32, u32>> p;
    p.reserve(n);
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        p.emplace_back(a, i);
    }
    ska::ska_sort(p.begin(), p.end());

    mld::ordered_set<std::pair<u32, u32>, mld::pmr::managers::reusing<>, 64, 64> os(p,
                                                                                    pa);

    while (q--) {
        u32 l, r, x;
        std::cin >> l >> r >> x;

        std::cout << os.rank({x, r}) - os.rank({x, l}) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
