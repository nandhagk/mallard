#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/functional_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::functional_graph fg(std::views::iota(u32{0}, n) |
                             std::views::transform([](auto) {
                                 u32 t;
                                 std::cin >> t;
                                 return --t;
                             }));

    for (u32 u = 0; u < n; ++u) std::cout << fg.min_cycle(u) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
