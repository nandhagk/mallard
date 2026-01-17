#ifndef MALLARD_FOLDABLE_DEQUE_H
#define MALLARD_FOLDABLE_DEQUE_H 1

#include <cassert>
#include <vector>

#include "lib/algebra/internal/concepts.h"

namespace mld {
template <algebra::internal::semigroup Semigroup>
struct foldable_deque {
    using operand = Semigroup;

    using size_type = u32;
    using container_type = std::vector<operand>;

private:
    container_type vl, vr, al, ar;

public:
    constexpr foldable_deque() noexcept = default;

    explicit constexpr foldable_deque(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        clear();
        vl.reserve(n);
        vr.reserve(n);
        al.reserve(n);
        ar.reserve(n);
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return vl.empty() && vr.empty();
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return static_cast<size_type>(vl.size() + vr.size());
    }

    constexpr void clear() noexcept {
        vl.clear(), vr.clear();
        al.clear(), ar.clear();
    }

    constexpr void push_back(const operand &x) noexcept {
        vr.push_back(x);
        ar.push_back(ar.empty() ? x : ar.back() + x);
    }

    constexpr void push_front(const operand &x) noexcept {
        vl.push_back(x);
        al.push_back(al.empty() ? x : x + al.back());
    }

    constexpr void pop_front() noexcept {
        assert(!empty());

        if (al.empty()) {
            size_type len = ar.size() / 2;
            vl.insert(vl.end(), vr.rbegin() + len, vr.rend());
            vr.erase(vr.begin(), vr.end() - len);
            al = vl, ar = vr;
            for (size_type i = 1; i < al.size(); ++i) al[i] = al[i] + al[i - 1];
            for (size_type i = 1; i < al.size(); ++i) ar[i] = ar[i - 1] + ar[i];
        }

        vl.pop_back();
        al.pop_back();
    }

    constexpr void pop_back() noexcept {
        assert(!empty());

        if (ar.empty()) {
            size_type len = al.size() / 2;
            vr.insert(vr.end(), vl.rbegin() + len, vl.rend());
            vl.erase(vl.begin(), vl.end() - len);
            al = vl, ar = vr;
            for (size_type i = 1; i < al.size(); ++i) al[i] = al[i] + al[i - 1];
            for (size_type i = 1; i < al.size(); ++i) ar[i] = ar[i - 1] + ar[i];
        }

        vr.pop_back();
        ar.pop_back();
    }

    [[nodiscard]] constexpr operand fold() const noexcept {
        assert(!empty());

        if (vl.empty()) return ar.back();
        if (vr.empty()) return al.back();
        return al.back() + ar.back();
    }
};
} // namespace mld

#endif // MALLARD_FOLDABLE_DEQUE_H
