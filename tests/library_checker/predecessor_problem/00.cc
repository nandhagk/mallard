#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/managers/reusing.h"
#include "lib/ordered_set.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    std::string s;
    std::cin >> s;

    auto r = std::views::iota(u32{0}, n) |
             std::views::filter([&](u32 k) { return s[k] == '1'; });
    mld::ordered_set<u32, mld::pmr::managers::reusing<>, 64> os(
        std::ranges::subrange(r, std::ranges::count(s, '1')), pa);

    while (q--) {
        u32 t, k;
        std::cin >> t >> k;

        if (t == 0) {
            os.insert(k);
        } else if (t == 1) {
            os.erase(k);
        } else if (t == 2) {
            std::cout << os.contains(k) << '\n';
        } else if (t == 3) {
            if (os.contains(k))
                std::cout << k << '\n';
            else if (auto y = os.next(k))
                std::cout << *y << '\n';
            else
                std::cout << -1 << '\n';
        } else { //
            if (os.contains(k))
                std::cout << k << '\n';
            else if (auto y = os.prev(k))
                std::cout << *y << '\n';
            else
                std::cout << -1 << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
