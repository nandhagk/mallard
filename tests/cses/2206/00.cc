#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::segment_tree<mld::algebra::min<u32>> stl(
        std::views::iota(u32{0}, n) |
        std::views::transform([&](u32 i) { return a[i] + n - i - 1; }));

    mld::segment_tree<mld::algebra::min<u32>> str(
        std::views::iota(u32{0}, n) |
        std::views::transform([&](u32 i) { return a[i] + i; }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 k, x;
            std::cin >> k >> x;

            a[--k] = x;
            stl.set(k, a[k] + n - k - 1);
            str.set(k, a[k] + k);
        } else {
            u32 k;
            std::cin >> k;

            u32 out = a[--k];
            if (k > 0) out = std::min(out, stl.fold(0, k).val() - (n - k - 1));
            if (k < n - 1) out = std::min(out, str.fold(k + 1, n).val() - k);
            std::cout << out << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
