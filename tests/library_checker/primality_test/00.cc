#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/miller_rabin.h"
#include "lib/prelude.h"

void solve() {
    u64 n;
    std::cin >> n;

    std::cout << (mld::miller_rabin(n) ? "Yes\n" : "No\n");
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
