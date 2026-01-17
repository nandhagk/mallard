#include <bits/stdc++.h>

#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::map<u32, u32> m;
    for (auto &&x : a) m.emplace_hint(m.end(), m.size(), x);

    auto f = [](u32 x) {
        u32 y = 0;
        for (; x; x /= 10) y += x % 10;
        return y;
    };

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 l, r;
            std::cin >> l >> r;

            for (auto it = m.lower_bound(--l); it != m.end();) {
                auto &&[p, x] = *it;
                if (p >= r) break;

                a[p] = x = f(x);
                if (x <= 9)
                    m.erase(it++);
                else
                    ++it;
            }
        } else {
            u32 p;
            std::cin >> p;

            std::cout << a[--p] << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
