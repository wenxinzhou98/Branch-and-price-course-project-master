#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "node.h"

struct cmp {
    bool operator()(const Node& node1, const Node& node2) {
        return node1.m_obj_val > node2.m_obj_val;
    }
};

#endif /*UTILS_HPP_*/