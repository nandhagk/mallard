#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 26> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    using operand = mld::algebra::affine<Z>;
    mld::dynamic_segment_tree<operand, mld::pmr::managers::reusing<>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a, b;
            std::cin >> a >> b;
            return operand(a, b);
        }),
        pa);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 i, c, d;
            std::cin >> i >> c >> d;

            st.set(i, {c, d});
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
