#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lis.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    auto l = mld::lis(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    std::cout << l.size() << '\n';
    for (auto &&x : l) std::cout << x << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
