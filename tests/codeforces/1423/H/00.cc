#include <bits/stdc++.h>

#include "lib/am_tree.h"
#include "lib/erasable_heap.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q, k;
    std::cin >> n >> q >> k;

    mld::pbds::hash_map<std::pair<u32, u32>, u32> end;
    end.resize(q);

    mld::erasable_heap<std::tuple<u32, u32, u32>, std::greater<>> hep;

    mld::am_tree<i32> amt(n);

    mld::offline::dynamic_connectivity<decltype(amt)> dct(n);
    dct.reserve(q);

    u32 d = 0;
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 u, v;
            std::cin >> u >> v;

            --u, --v;
            if (u > v) std::swap(u, v);

            if (end.find({u, v}) != end.end())
                hep.erase(end[{u, v}], u, v);
            else
                dct.link(u, v);

            end[{u, v}] = d + k;
            hep.insert(d + k, u, v);
        } else if (t == 2) {
            u32 u;
            std::cin >> u;

            --u;
            dct.query([u](auto &&am, u32) { std::cout << am.size(u) << '\n'; });
        } else {
            for (++d; !hep.empty() && std::get<0>(hep.top()) == d; hep.pop()) {
                auto [_, u, v] = hep.top();
                dct.cut(u, v);
                end.erase({u, v});
            }
        }
    }

    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
