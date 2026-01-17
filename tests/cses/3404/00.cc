#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lis.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> a(n), b(m);
    for (auto &&x : a) std::cin >> x, --x;
    for (auto &&x : b) std::cin >> x, --x;
    if (n < m) std::swap(n, m), std::swap(a, b);

    std::vector<u32> rnk(n);
    for (u32 i = 0; i < n; ++i) rnk[a[i]] = i;

    auto c = b;
    for (u32 i = 0; i < m; ++i) c[i] = rnk[b[i]];

    auto d = mld::lis(c);
    std::cout << d.size() << '\n';
    for (u32 x : d) std::cout << b[x] + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
