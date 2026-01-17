#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/base.h"
#include "lib/algebra/combined.h"
#include "lib/algebra/lxor.h"
#include "lib/algebra/sum.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_aggregate.h"
#include "lib/static_csr.h"

using room = mld::algebra::combined<mld::algebra::sum<u32>, mld::algebra::sum<u32>>;

struct light : mld::actions::base<mld::algebra::lxor<u32>>::identity {
    using operand = room;
    using operation = mld::algebra::lxor<u32>;

    [[nodiscard]] static operand act(const operation &f, const operand &x) noexcept {
        if (!f) return x;
        auto &&[a, b] = x.val();
        return room(b, a);
    }
};

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::rooted_tree_aggregate<mld::lazy_segment_tree<light>> rag(
        g, a | std::views::transform([](u32 x) { return room(x == 0, x == 1); }));

    u32 q;
    std::cin >> q;

    while (q--) {
        std::string t;
        u32 u;
        std::cin >> t >> u;

        --u;
        if (t == "get")
            std::cout << std::get<1>(rag.fold_subtree(u).val()) << '\n';
        else
            rag.apply_subtree(u, 1);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
