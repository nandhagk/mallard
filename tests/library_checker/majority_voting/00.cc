#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/boyer_moore.h"
#include "lib/managers/reusing.h"
#include "lib/ordered_set.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "ska_sort.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    std::vector<std::pair<u32, u32>> p;
    p.reserve(n);
    for (u32 i = 0; i < n; ++i) p.emplace_back(a[i], i);
    ska::ska_sort(p.begin(), p.end());

    mld::ordered_set<std::pair<u32, u32>, mld::pmr::managers::reusing<>, 64> os(p, pa);

    using operand = mld::algebra::boyer_moore<u32>;
    mld::segment_tree<operand> st(
        a | std::views::transform([](u32 x) { return operand(x); }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 k, v;
            std::cin >> k >> v;

            os.erase({a[k], k});
            os.insert({a[k] = v, k});

            st.set(k, operand(v));
        } else {
            u32 l, r;
            std::cin >> l >> r;

            auto [u, _] = st.fold(l, r).val();
            u32 v = os.rank({u, r}) - os.rank({u, l});
            if (v * 2 > r - l)
                std::cout << u << '\n';
            else
                std::cout << -1 << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
