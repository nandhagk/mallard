#include <bits/stdc++.h>

#pragma GCC target("popcnt")

#include "blazingio.min.h"
#include "lib/actions/base.h"
#include "lib/algebra/lor.h"
#include "lib/algebra/sum.h"
#include "lib/dynamic_modint.h"
#include "lib/lazy_segment_tree.h"
#include "lib/rooted_tree_aggregate.h"
#include "lib/sieve.h"
#include "lib/static_csr.h"
#include <tr2/dynamic_bitset>

static constexpr u32 N = 1024;
using bitset = std::bitset<N>;

static bitset sft;
using Z = mld::dynamic_modint_32<-1>;

struct action : mld::actions::base<mld::algebra::sum<Z>>::identity {
    using operand = mld::algebra::lor<bitset>;
    using operation = mld::algebra::sum<Z>;

    [[nodiscard]] static operand act(const operation &f, const operand &x) {
        if (!f) return x;
        return ((x.val() << (f.val().val()) | x.val() >> (Z::mod() - f.val().val())) &
                sft);
    }
};

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    Z::set_mod(m);
    for (u32 i = 0; i < m; ++i) sft.set(i);

    mld::sieve s(m - 1);

    bitset msk;
    for (u32 p : s.pri) msk.set(p);

    std::vector<Z> a(n);
    for (auto &&x : a) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_aggregate<mld::lazy_segment_tree<action>> rag(
        g, a | std::views::transform([](Z x) {
               bitset y;
               y.set(x.val());
               return y;
           }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t, u;
        std::cin >> t >> u;

        --u;
        if (t == 1) {
            u32 x;
            std::cin >> x;

            rag.apply_subtree(u, x);
        } else {
            std::cout << (rag.fold_subtree(u).val() & msk).count() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
