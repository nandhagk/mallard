#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/functional_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::functional_graph fg(std::views::iota(u32{0}, n) |
                             std::views::transform([](auto) {
                                 u32 t;
                                 std::cin >> t;
                                 return --t;
                             }));

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        std::cout << i32(fg.dist(--u, --v).value_or(-1)) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
