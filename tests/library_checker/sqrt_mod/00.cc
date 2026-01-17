#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/cipolla.h"
#include "lib/dynamic_montgomery_modint.h"
#include "lib/prelude.h"

using Z = mld::dynamic_montgomery_modint_32<-1>;

void solve() {
    u32 y, p;
    std::cin >> y >> p;

    if (y == 0) {
        std::cout << y << '\n';
    } else if (p == 2) {
        std::cout << 1 << '\n';
    } else {
        Z::set_mod(p);
        if (auto r = mld::cipolla(Z(y)))
            std::cout << r->val() << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
