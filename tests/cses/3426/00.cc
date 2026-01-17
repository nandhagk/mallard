#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/lxor.h"
#include "lib/dynamic_modint.h"
#include "lib/foldable_queue.h"
#include "lib/prelude.h"

using Z = mld::dynamic_modint_32<-1>;

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    u32 x, a, b, c;
    std::cin >> x >> a >> b >> c;

    mld::foldable_queue<mld::algebra::lxor<u32>> sw(k + 1);

    Z::set_mod(c);
    Z y(x), p(a), q(b);

    u32 ans{};
    for (u32 i = 0; i < n; ++i) {
        sw.push(x);

        y *= p;
        y += q;
        x = y.val();

        if (i >= k - 1) {
            ans ^= sw.fold().val();
            sw.pop();
        }
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
