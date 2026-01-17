#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/combined.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/static_modint.h"

using Z = mld::static_modint_64<(u64{1} << 61) - 1>;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::combined<mld::algebra::sum<u64>>;
    mld::scan_table<operand> sp(std::views::iota(u32{0}, n) |
                                std::views::transform([](auto) {
                                    u32 a;
                                    std::cin >> a;
                                    return operand(a);
                                }));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        std::cout << std::get<0>(sp.fold(l, r).val()) << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
