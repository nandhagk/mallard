#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/erdos_gallai.h"
#include "lib/havel_hakimi.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    if (!mld::erdos_gallai(a)) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    auto e = mld::havel_hakimi(a);

    std::cout << e.size() << '\n';
    for (auto &&[u, v] : e) std::cout << u + 1 << ' ' << v + 1 << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
