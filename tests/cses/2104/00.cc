#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/keyword_search.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);

    mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>> st(sa);

    u32 k;
    std::cin >> k;

    while (k--) {
        std::string p;
        std::cin >> p;

        auto [l, r] = mld::keyword_search(s, sa, p);
        if (l == r)
            std::cout << -1 << '\n';
        else
            std::cout << st.fold(l, r).val() + 1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
