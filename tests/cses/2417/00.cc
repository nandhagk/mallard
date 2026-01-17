#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

static const constexpr u32 N = 1'000'000;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> cnt(N + 1);
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        ++cnt[a];
    }

    std::vector<u64> as(N + 1);
    for (u32 i = 1; i <= N; ++i)
        for (u32 j = i; j <= N; j += i) as[i] += cnt[j];

    std::vector<u64> bs(N + 1);
    for (u32 i = N; i >= 1; --i) {
        for (u32 j = i; j <= N; j += i) bs[i] -= bs[j];
        bs[i] += as[i] * (as[i] - 1) / 2;
    }

    std::cout << bs[1] << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
