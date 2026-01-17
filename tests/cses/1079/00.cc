#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binomial.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint1000000007;

static mld::binomial<Z> bin;
static constexpr u32 N = 1'000'000;

void solve() {
    u32 a, b;
    std::cin >> a >> b;

    std::cout << bin.comb(a, b).val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    bin.build(N);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
