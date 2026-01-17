#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> cnt(n + 1);
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        ++cnt[a];
    }

    std::vector<u32> a(n + 1);
    for (u32 i = 1; i <= n; ++i)
        for (u32 j = i; j <= n; j += i) a[i] += cnt[j];

    std::vector<Z> pw(n + 1);
    pw[0] = 1;
    for (u32 i = 1; i <= n; ++i) pw[i] = pw[i - 1] * 2;

    std::vector<Z> b(n + 1);
    for (u32 i = n; i >= 1; --i) {
        for (u32 j = i; j <= n; j += i) b[i] -= b[j];
        b[i] += pw[a[i]] - 1;
    }

    for (auto &&x : b | std::views::drop(1)) std::cout << x.val() << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
