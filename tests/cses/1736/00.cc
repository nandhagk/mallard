#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_sum.h"
#include "lib/algebra/combined.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

template <typename T>
struct action : mld::actions::base<mld::algebra::sum<T>>::identity {
    using operand = mld::algebra::combined<mld::algebra::sum<T>, mld::algebra::sum<T>>;
    using operation = mld::algebra::sum<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        auto &&[a, b] = x.val();
        return {a, a * f.val() + b};
    }
};

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = action<u64>::operand;
    mld::lazy_segment_tree<action<u64>> st(std::views::iota(u32{0}, n) |
                                           std::views::transform([](u32 k) {
                                               u32 a;
                                               std::cin >> a;
                                               return operand(k + 1, a);
                                           }));
    mld::lazy_segment_tree<mld::actions::add_sum<u64>> sr(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    while (q--) {
        u32 t, a, b;
        std::cin >> t >> a >> b;

        --a;
        if (t == 1)
            st.apply(a, b, 1), sr.apply(a, b, a);
        else
            std::cout << std::get<1>(st.fold(a, b).val()) - sr.fold(a, b).val() << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
