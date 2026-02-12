#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"
#include "lib/rooted_tree_lca.h"

void solve() {
   u32 n;
   std::cin >> n;

   std::vector<std::pair<u32, u32>> e, f;
   e.reserve(2 * n), f.reserve(n);

   mld::union_find uf(n);
   for (u32 i = 0; i <= n; ++i) {
       u32 u, v;
       std::cin >> u >> v;

       if (uf.merge(--u, --v)) {
           e.emplace_back(u, v);
           e.emplace_back(v, u);
       } else {
           f.emplace_back(u, v);
       }
   }

   if (uf.ccs() != 1) {
       std::cout << 0 << '\n';
       return;
   }

   mld::static_csr g(n, e);
   mld::rooted_tree_lca h(g);

   auto [u, v] = f[0];
   auto [p, q] = f[1];

   u64 ans = u64(h.dist(u, v) + 1) * (h.dist(p, q) + 1);
   if (auto res = h.path_intersection(u, v, p, q)) {
       auto [y, z] = *res;
       u32 x = h.dist(y, z);
       ans -= u64(x) * x;
   }

   std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
