#ifndef NODE_H_
#define NODE_H_

#include "cg.h"

class Node {
public:
    bool m_is_root_node; // 1, if this node is a root node; 0, otherwise
    double m_local_lb;
    double m_local_ub;
    double m_obj_val;
    int m_status; // 1, RMP solved successfully; 0, otherwise
    int m_node_depth;
    std::vector<double> m_length_available_lumber;
    std::vector<double> m_cost;
    std::vector<std::vector<double> > m_customer_need;
    std::vector<double> m_x_sol;
    std::vector<std::vector<int> > m_parent_node_pattern_set;
    std::vector<std::vector<int> > current_node_pattern_set;
    std::vector<int> m_branching_constraints_index;
    std::vector<int> m_branching_constraints_value;
    std::vector<std::string> m_branching_constraints_char;
public:
    Node(bool is_root_node, std::vector<double> length_available_lumber, std::vector<double> cost, std::vector<std::vector<double> > customer_need, std::vector<std::vector<int> > parent_node_pattern_set, std::vector<int> branching_constraints_index, std::vector<int> branching_constraints_value, std::vector<std::string> branching_constraints_char);
};

#endif /*NODE_H_*/