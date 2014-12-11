#include <multiset.h>

bool comparing::operator ()(const float& left, const float& right) const {
    return left > right;
}

