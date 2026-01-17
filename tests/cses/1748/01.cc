#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> p;
    p.reserve(n);
    for (auto &&x : a) p.emplace_back(x, n - p.size() - 1);
    std::ranges::sort(p);

    mld::fenwick_tree<mld::algebra::sum<Z>> fwt(n);
    for (auto &&[x, j] : p) {
        u32 k = n - j - 1;
        fwt.add(k, fwt.fold(k) + 1);
    }

    std::cout << fwt.fold(n).val().val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
