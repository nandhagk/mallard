#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/two_sat.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::two_sat ts(m);
    while (n--) {
        u32 x, y;
        char a, b;

        std::cin >> a >> x >> b >> y;

        ts.add_clause(--x, a == '+', --y, b == '+');
    }

    if (auto ans = ts.satisfiable())
        for (auto &&x : *ans) std::cout << (x ? '+' : '-') << ' ';
    else
        std::cout << "IMPOSSIBLE\n";
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
