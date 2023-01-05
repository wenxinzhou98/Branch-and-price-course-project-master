#ifndef BB_H_
#define BB_H_

#include "node.h"
#include "utils.hpp"
#include <queue>

class BB {
public:
    double gap;
    double m_global_lb;
    double m_global_ub;
    double epsilon;
    std::vector<double> m_length_available_lumber;
    std::vector<double> m_cost;
    std::vector<std::vector<double> > m_customer_need;
    std::priority_queue<Node, std::vector<Node>, cmp> tree;
public:
    BB(std::vector<double> length_available_lumber, std::vector<double> cost, std::vector<std::vector<double> > customer_need);
    void branch_and_price();
    bool check_if_node_integral(const Node& node1, int& first_non_integral_index);
};

#endif /*BB_H_*/