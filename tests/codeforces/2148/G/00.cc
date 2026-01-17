#include <bits/stdc++.h>

#include "lib/algebra/max.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/static_csr.h"

static constexpr u32 N = 200'000;
static mld::static_csr<u32> z;

void solve() {
    u32 n;
    std::cin >> n;

    mld::segment_tree<mld::algebra::max<u32>> st(
        std::views::iota(u32{0}, n + 1) |
        std::views::transform([](auto) { return 0; }));

    u32 g = 0;
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;

        u32 h = std::gcd(g, a);
        if (g != 0) {
            for (u32 f : z[g])
                if (h % f) st.set(f, i);
        }

        g = h;
        for (u32 f : z[a])
            if (g % f) st.set(f, st.get(f).val() + 1);

        std::cout << st.fold().val() << ' ';
    }

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::pair<u32, u32>> e;
    e.reserve(std::bit_width(N) * N);

    for (u32 i = 2; i <= N; ++i)
        for (u32 j = i; j <= N; j += i) e.emplace_back(j, i);

    z.build(N + 1, e);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
