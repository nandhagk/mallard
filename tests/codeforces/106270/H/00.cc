#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/sieve.h"

static mld::sieve s;
static constexpr u32 N = 1'000'000;

void solve() {
    u32 n, a, b;
    std::cin >> n >> a >> b;

    std::vector<std::pair<u32, u32>> x;
    x.reserve(n);
    for (u32 i = 0; i < n; ++i) {
        u32 w;
        std::cin >> w;

        u32 u = 0, v = 0;
        for (auto &&[p, c] : s.factorize(w))
            if (p % 2)
                v += p * c;
            else
                u += p * c;

        x.emplace_back(u, v);
    }

    std::sort(x.begin(), x.end(), [](auto &&u, auto &&v) {
        return u.first + v.second < u.second + v.first;
    });

    u64 ans = 0;
    for (u32 i = 0; i < b; ++i) ans += x[i].first;
    for (u32 i = b; i < n - a; ++i) ans += std::min(x[i].first, x[i].second);
    for (u32 i = n - a; i < n; ++i) ans += x[i].second;

    std::cout << ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    s.build(N);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
