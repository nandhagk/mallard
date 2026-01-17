#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lis.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::cout << mld::lis(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                              u32 a;
                              std::cin >> a;
                              return a;
                          }))
                     .size();
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
