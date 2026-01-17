#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/combined.h"
#include "lib/algebra/sum.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"

static constexpr u32 N = 200'000;

void solve() {
    u32 n;
    std::cin >> n;

    using operand =
        mld::algebra::combined<mld::algebra::sum<u32>, mld::algebra::sum<u64>>;
    using tree = mld::dynamic_segment_tree<operand, mld::managers::clonable<>>;

    std::vector<tree> st;
    st.reserve(n + 1);

    st.emplace_back(std::views::iota(u32{0}, N + 2) |
                    std::views::transform([](auto) { return operand(); }));
    for (u32 i = 0; i < n; ++i) {
        u32 x1, x2, y1, a, b, y2;
        std::cin >> x1 >> x2 >> y1 >> a >> b >> y2;

        st.emplace_back(st.back());
        st.back().add(0, {0, y1});
        st.back().add(x1 + 1, {a, u64(b) - y1});
        st.back().add(x2 + 1, {-a, y2 - u64(b)});
    }

    u32 m;
    std::cin >> m;

    for (u64 lst = 0; m--;) {
        u32 l, r, x;
        std::cin >> l >> r >> x;

        x = static_cast<u32>((x + lst) % 1'000'000'000);
        u32 y = std::min(x, N + 1);

        auto &&[a, b] = (st[r].fold(0, y + 1) + (-st[--l].fold(0, y + 1))).val();
        std::cout << (lst = u64(a) * x + b) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
