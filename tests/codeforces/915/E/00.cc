#include <bits/stdc++.h>

#include "lib/actions/base.h"
#include "lib/algebra/assign.h"
#include "lib/algebra/combined.h"
#include "lib/algebra/sum.h"
#include "lib/compress.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

struct action : mld::actions::base<mld::algebra::assign<bool>>::identity {
    using operand =
        mld::algebra::combined<mld::algebra::sum<u32>, mld::algebra::sum<u32>>;
    using operation = mld::algebra::assign<bool>;

    [[nodiscard]] static operand act(const operation &f, const operand &x) noexcept {
        if (!f) return x;

        auto [a, b] = x.val();
        return {a, a * f.val().value()};
    }
};

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> p;
    p.reserve(2 * q + 2);

    std::vector<u32> que;
    que.reserve(q);

    for (u32 i = 0; i < q; ++i) {
        u32 l, r, k;
        std::cin >> l >> r >> k;

        p.push_back(--l), p.push_back(r);
        que.push_back(k);
    }

    p.push_back(0), p.push_back(n);
    auto [v, na] = mld::compress(p);

    mld::lazy_segment_tree<action> st(
        std::views::iota(u32{0}, static_cast<u32>(v.size()) - 1) |
        std::views::transform(
            [&](u32 k) { return action::operand(v[k + 1] - v[k], v[k + 1] - v[k]); }));

    for (u32 i = 0; i < q; ++i) {
        u32 l = na[i << 1 | 0], r = na[i << 1 | 1];
        if (que[i] == 1)
            st.apply(l, r, 0);
        else
            st.apply(l, r, 1);

        std::cout << std::get<1>(st.fold().val()) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
