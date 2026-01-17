#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ntt.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());

    u32 m = std::bit_ceil(2 * n - 1);
    std::vector<Z> a(m), b(m);
    for (u32 i = 0; i < n; ++i)
        if (s[i] == '1') a[i] += 1, b[n - i - 1] += 1;

    mld::ntt<false>(a), mld::ntt<false>(b);
    for (u32 i = 0; i < m; ++i) a[i] *= b[i];
    mld::ntt<true>(a);

    for (u32 i = n; i < 2 * n - 1; ++i) std::cout << a[i].val() << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
