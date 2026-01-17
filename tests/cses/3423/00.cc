#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/sieve.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::sieve s(2 * n + 1);

    std::vector<u32> p(n);
    for (u32 r = n; r > 0; --r) {
        u32 z = 2 * r;
        while (!s.is_prime(z)) --z;

        for (u32 l = z - r; l <= r && l > 0; ++l) p[l - 1] = z - l;
        r = z - r;
    }

    for (u32 i = 0; i < n; ++i) std::cout << i + 1 << ' ';
    std::cout << '\n';

    for (auto &&x : p) std::cout << x << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
