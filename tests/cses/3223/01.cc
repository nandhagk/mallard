#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/managers/reusing.h"
#include "lib/ordered_set.h"
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

    mld::ordered_set<std::pair<u32, u32>, mld::pmr::managers::reusing<>, 64> os(pa);

    u64 ans = 0;
    for (u32 i = 0; i < k - 1; ++i) {
        std::pair<u32, u32> b{a[i], i};
        ans += os.size() - os.rank(b);
        os.insert(b);
    }

    for (u32 i = k - 1; i < n; ++i) {
        std::pair<u32, u32> b{a[i], i};
        ans += os.size() - os.rank(b);
        os.insert(b);

        std::cout << ans << ' ';

        b = {a[i - k + 1], i - k + 1};
        ans -= os.rank(b);
        os.erase(b);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
