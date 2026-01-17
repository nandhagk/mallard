#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/two_sat.h"

void solve() {
    std::string p, cnf;
    u32 n, m;

    std::cin >> p >> cnf >> n >> m;

    mld::two_sat ts(n);
    ts.reserve(m);

    while (m--) {
        i32 u, v, z;
        std::cin >> u >> v >> z;

        u32 x = std::abs(u) - 1;
        u32 y = std::abs(v) - 1;
        ts.add_clause(x, u > 0, y, v > 0);
    }

    if (auto z = ts.satisfiable()) {
        std::cout << "s SATISFIABLE\n";
        std::cout << 'v';

        auto ans = *z;
        for (u32 i = 0; i < n; ++i) std::cout << ' ' << i32(ans[i] ? i + 1 : -i - 1);
        std::cout << " 0\n";
    } else {
        std::cout << "s UNSATISFIABLE\n";
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
