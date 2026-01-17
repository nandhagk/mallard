#include <bits/stdc++.h>

#include "lib/lichao_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    static std::array<std::byte, (1 << 30) - (1 << 25)> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    using line = mld::internal::line<f80>;

    i64 lo = -1e14 - 5, hi = 1e14 + 5;
    mld::lichao_tree<i64, line, mld::pmr::managers::reusing<>> lch(lo, hi, pa);

    while (n--) {
        i64 x, v;
        std::cin >> x >> v;

        f80 m = f80(1) / v;
        lch.add(lo, x, line(-m, x * m));
        lch.add(x, hi, line(m, -x * m));
    }

    u32 q;
    std::cin >> q;

    while (q--) {
        char t;
        std::cin >> t;

        if (t == '+') {
            i64 x, v;
            std::cin >> x >> v;

            f80 m = f80(1) / v;
            lch.add(lo, x, line(-m, x * m));
            lch.add(x, hi, line(m, -x * m));
        } else {
            i64 y;
            std::cin >> y;

            std::cout << lch.query(y)(y) << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    std::cout << std::fixed;
    std::cout << std::setprecision(16);
    while (t--) solve();
}
