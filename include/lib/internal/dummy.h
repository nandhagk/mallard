#ifndef MALLARD_INTERNAL_DUMMY_H
#define MALLARD_INTERNAL_DUMMY_H 1

namespace mld::internal {
// To prevent duplicate base type
template <int>
struct dummy {};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_DUMMY_H
