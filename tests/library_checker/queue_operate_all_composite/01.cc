#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine.h"
#include "lib/foldable_deque.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 q;
    std::cin >> q;

    mld::foldable_deque<mld::algebra::affine<Z>> dq(q);
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 a, b;
            std::cin >> a >> b;

            dq.push_back({a, b});
        } else if (t == 1) {
            dq.pop_front();
        } else {
            u32 x;
            std::cin >> x;

            if (dq.empty())
                std::cout << x << '\n';
            else
                std::cout << dq.fold()(x).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
