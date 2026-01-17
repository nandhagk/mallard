#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/managers/reusing.h"
#include "lib/ordered_set.h"
#include "lib/prelude.h"
#include "ska_sort.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    std::vector<std::pair<u32, u32>> p;
    if (k > 1) p.reserve(k - 1);
    for (u32 i = 0; i < k - 1; ++i) p.emplace_back(a[i], i);
    ska::ska_sort(p.begin(), p.end());

    mld::ordered_set<std::pair<u32, u32>, mld::pmr::managers::reusing<>, 64> os(p, pa);

    for (u32 i = k - 1; i < n; ++i) {
        os.insert({a[i], i});
        std::cout << os.kth((k - 1) / 2).first << ' ';
        os.erase({a[i - k + 1], i - k + 1});
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
