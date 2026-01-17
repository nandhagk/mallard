#include <bits/stdc++.h>

#include "lib/math.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n), b(n), c(n);
    for (auto &&x : a) std::cin >> x;

    b[0] = a[0];
    for (u32 i = 1; i < n; ++i) b[i] = mld::gcd(b[i - 1], a[i]);

    c[n - 1] = a[n - 1];
    for (u32 i = n - 1; i--;) c[i] = mld::gcd(c[i + 1], a[i]);

    u64 z = std::reduce(c.begin(), c.end(), u64{0}) - c[n - 1], y = 0;
    u64 ans = z + y;
    for (u32 i = n - 2; i > 0; --i) {
        z -= c[i], y += b[i];
        ans = std::min(ans, z + y);
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
