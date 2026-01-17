#include <bits/stdc++.h>

#include "lib/algebra/lor.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

static constexpr u32 N = 1'000'000;
static constexpr u32 K = std::bit_width(N);

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::segment_tree<mld::algebra::lor<u32>> st(a);

    std::vector<bool> sen(std::bit_ceil(N));

    constexpr u32 UNSET = -1;
    std::array<u32, K> prv{};
    prv.fill(UNSET);

    for (u32 i = 0; i < n; ++i) {
        for (u32 j : prv)
            if (j != UNSET) sen[st.fold(j, i + 1).val()] = 1;

        sen[a[i]] = 1;
        for (u32 j = 0; j < K; ++j)
            if ((a[i] >> j) & 1) prv[j] = i;
    }

    std::cout << std::reduce(sen.begin(), sen.end(), u32{0}) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
