#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/affine_sum.h"
#include "lib/prelude.h"
#include "lib/sparse_lazy_segment_tree.h"
#include "lib/managers/reusing.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 25> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    mld::sparse_lazy_segment_tree<u32, mld::actions::affine_sum<Z>, mld::pmr::managers::reusing<>>
        st(0, n, pa);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 l, r, b, c;
            std::cin >> l >> r >> b >> c;

            st.apply(l, r, {b, c});
        } else {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << st.fold(l, r).val().val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
