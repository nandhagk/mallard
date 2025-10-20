#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/debug.h"
#include "lib/math.h"

void solve() {
    mld::gaussian_integer<i64> a, b;
    std::cin >> a >> b;

    std::cout << mld::gcd(a, b) << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}