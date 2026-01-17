#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/lichao_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> w(n);
    for (auto &&x : w) std::cin >> x;

    mld::segment_tree<mld::algebra::min<u32>> st(w);

    static std::array<std::byte, 1 << 28> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    using line = mld::internal::line<i64>;
    mld::lichao_tree<u32, line, mld::pmr::managers::reusing<>> lch(0, n + 1, pa);

    for (u32 i = 0; i < n; ++i) {
        auto f = [&](auto &&x) { return x >= w[i]; };
        u32 l = i == 0 ? 0 : st.min_left(i, f);
        u32 r = st.max_right(i, f);

        u32 a = l > k ? l - k : 0;
        u32 b = r > k ? r - k : 0;

        i64 z = w[i];
        if (r - l >= k) {
            lch.add(a, l, line(-z, z * i32(l - k)));
            lch.add(l, b, line(0, -z * k));
            lch.add(b, r, line(z, -z * r));
        } else {
            lch.add(a, b, line(-z, z * i32(l - k)));
            lch.add(b, l, line(0, -z * (r - l)));
            lch.add(l, r, line(z, -z * r));
        }
    }

    for (u32 i = 0; i < n - k + 1; ++i) std::cout << u64(-lch.query(i)(i)) << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
