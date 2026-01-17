#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/manacher.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());
    auto [odd, even] = mld::manacher(s);

    std::vector<u32> a(n, n), b(n, n);
    for (u32 i = n; i--;) a[i + odd[i] - 1] = i;
    for (u32 i = n - 1; i; --i) b[i + even[i] - 1] = i - 1;

    for (u32 i = n - 1; i; --i) a[i - 1] = std::min(a[i], a[i - 1]);
    for (u32 i = n - 1; i; --i) b[i - 1] = std::min(b[i], b[i - 1]);
    for (u32 i = 0; i < n; ++i) b[i] = std::min(i, b[i]);

    for (u32 i = 0; i < n; ++i)
        std::cout << std::max(2 * (i - a[i]) + 1, 2 * (i - b[i])) << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
