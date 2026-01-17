#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/mo_order.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> p(n);
    for (auto &&[x, y] : p) std::cin >> x >> y, --x;

    for (u32 k : mld::mo_order<u32>(p, n)) std::cout << k + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
