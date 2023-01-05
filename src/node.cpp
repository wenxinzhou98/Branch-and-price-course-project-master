#include "node.h"

Node::Node(bool is_root_node, std::vector<double> length_available_lumber, std::vector<double> cost, 
        std::vector<std::vector<double> > customer_need, std::vector<std::vector<int> > parent_node_pattern_set,
        std::vector<int> branching_constraints_index, std::vector<int> branching_constraints_value, 
        std::vector<std::string> branching_constraints_char) {

    m_is_root_node = is_root_node;
    m_length_available_lumber = length_available_lumber;
    m_cost = cost;
    m_customer_need = customer_need;
    m_branching_constraints_index = branching_constraints_index;
    m_branching_constraints_value = branching_constraints_value;
    m_branching_constraints_char = branching_constraints_char;

    if (m_is_root_node == false) {
        m_parent_node_pattern_set = parent_node_pattern_set;
    }

    // Solve linear relaxation model of current node
    Column_Generation* cg_ptr = new Column_Generation(m_length_available_lumber, m_cost, m_customer_need, m_is_root_node, m_parent_node_pattern_set, m_branching_constraints_index, m_branching_constraints_value, m_branching_constraints_char);
    Column_Generation cg = *cg_ptr;
    m_obj_val = cg.solve_RMP();
    if (cg.m_status == 1) {
        m_status = 1;
        m_x_sol = cg.m_x_sol;
        current_node_pattern_set = cg.pattern_set;
    }
    else {
        m_status = 0;
        m_obj_val = DBL_MAX; // put this node at the bottom of the tree (priority queue).
    }
    
    delete cg_ptr;
}