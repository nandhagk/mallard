#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    u64 k;
    std::cin >> n >> k;

    constexpr u32 UNSET = -1;

    std::vector<u32> out(n, UNSET);
    for (u32 i = n; i--;) {
        if (k >= i) {
            k -= i, out[n - i - 1] = i;
            continue;
        }

        out[n - k - 1] = i;
        for (u32 j = 0, l = n - i - 1; l < n; ++l)
            if (out[l] == UNSET) out[l] = j++;

        break;
    }

    for (auto &&x : out) std::cout << x + 1 << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
