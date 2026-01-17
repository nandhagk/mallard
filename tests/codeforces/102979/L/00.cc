#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/eppstein.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u64> w(n);
    for (auto &&x : w) std::cin >> x;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(5 * n);

    for (u32 i = 0; i < n - 1; ++i) {
        e.emplace_back(3 * i + 0, std::pair<u32, u64>{3 * (i + 1) + 0, w[i + 1]});
        e.emplace_back(3 * i + 0, std::pair<u32, u64>{3 * (i + 1) + 1, 0});

        e.emplace_back(3 * i + 1, std::pair<u32, u64>{3 * (i + 1) + 0, w[i + 1]});
        e.emplace_back(3 * i + 1, std::pair<u32, u64>{3 * (i + 1) + 2, 0});

        e.emplace_back(3 * i + 2, std::pair<u32, u64>{3 * (i + 1) + 0, w[i + 1]});
    }

    u32 s = 3 * n, t = 3 * (n + 1);
    e.emplace_back(3 * (n - 1) + 0, std::pair<u32, u64>{t, 0});
    e.emplace_back(3 * (n - 1) + 1, std::pair<u32, u64>{t, 0});

    e.emplace_back(s, std::pair<u32, u64>{0, w[0]});
    e.emplace_back(s, std::pair<u32, u64>{2, 0});

    mld::static_csr g(t + 1, e);

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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
