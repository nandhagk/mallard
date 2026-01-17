#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/assign.h"
#include "lib/algebra/affine.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/sparse_lazy_segment_tree.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 30> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    using operand = mld::algebra::affine<Z>;
    using action = mld::actions::assign<operand>;
    mld::sparse_lazy_segment_tree<u32, action, mld::pmr::managers::reusing<>> st(0, n, pa);

    for (u32 i = 0; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        st.set(i, operand(a, b));
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 l, r, a, b;
            std::cin >> l >> r >> a >> b;

            st.apply(l, r, operand(a, b).val());
        } else {
            u32 l, r, x;
            std::cin >> l >> r >> x;

            std::cout << st.fold(l, r)(x).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
