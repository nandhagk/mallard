#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lichao_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using line = mld::internal::line<i64>;
    mld::lichao_tree<i32, line, mld::managers::reusing<>> lch(-(1 << 30), (1 << 30));

    while (n--) {
        i32 a;
        i64 b;
        std::cin >> a >> b;
        lch.add(line(a, b));
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            i32 a;
            i64 b;
            std::cin >> a >> b;
            lch.add(line(a, b));
        } else {
            i32 k;
            std::cin >> k;

            std::cout << lch.query(k)(k) << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
