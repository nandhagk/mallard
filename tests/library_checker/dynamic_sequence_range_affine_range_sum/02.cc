#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/affine_sum.h"
#include "lib/lazy_dynamic_sequence.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"
#include "lib/splay_tree.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 26> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::lazy_dynamic_sequence<mld::actions::affine_sum<Z>, mld::splay_tree,
                               mld::pmr::managers::reusing<>>
        ds(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
               u32 a;
               std::cin >> a;
               return a;
           }),
           pa);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 k, x;
            std::cin >> k >> x;

            ds.insert(k, x);
        } else if (t == 1) {
            u32 k;
            std::cin >> k;

            ds.erase(k);
        } else if (t == 2) {
            u32 l, r;
            std::cin >> l >> r;

            ds.reverse(l, r);
        } else if (t == 3) {
            u32 l, r, b, c;
            std::cin >> l >> r >> b >> c;

            ds.apply(l, r, {b, c});
        } else {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << ds.fold(l, r).val().val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
