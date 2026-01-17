#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/affine_sum.h"
#include "lib/dynamic_lazy_segment_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 27> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    using action = mld::actions::affine_sum<Z>;
    mld::dynamic_lazy_segment_tree<action, mld::pmr::managers::reusing<>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a;
            std::cin >> a;
            return action::operand(a);
        }),
        pa);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 l, r, a, b;
            std::cin >> l >> r >> a >> b;

            st.apply(l, r, {a, b});
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
