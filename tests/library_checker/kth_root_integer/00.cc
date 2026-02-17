#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/kth_integral_root.h"

void solve() {
    u64 n;
    u32 k;
    std::cin >> n >> k;

    std::cout << mld::kth_integral_root(n, k) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
