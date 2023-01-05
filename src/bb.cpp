#include "bb.h"

BB::BB(std::vector<double> length_available_lumber, std::vector<double> cost, std::vector<std::vector<double> > customer_need) {
    m_length_available_lumber = length_available_lumber;
    m_cost = cost;
    m_customer_need = customer_need;
    
    m_global_lb = 0.;
    m_global_ub = DBL_MAX;
    epsilon = 1e-3;
}

bool BB::check_if_node_integral(const Node& node1, int& first_non_integral_index) {
    if (node1.m_status == 0) {
        return false;
    }
    else {
        bool indicator = true;
        for (int i = 0; i < node1.m_x_sol.size(); i++) {
            if (node1.m_x_sol[i] - (int)node1.m_x_sol[i] > 1e-5) {
                indicator = false;
                first_non_integral_index = i;
                break;
            }
        }
        return indicator;
    }
}

void BB::branch_and_price() {
    bool is_root_node = true;
    std::vector<std::vector<int> > parent_node_pattern_set;
    std::vector<int> branching_constraints_index;
    std::vector<int> branching_constraints_value;
    std::vector<std::string> branching_constraints_char;
    Node* root_node_ptr = new Node(is_root_node, m_length_available_lumber, m_cost, m_customer_need, parent_node_pattern_set, branching_constraints_index, branching_constraints_value, branching_constraints_char);
    Node root_node = *root_node_ptr;
    tree.push(root_node);
    root_node.m_node_depth = 0;
    delete root_node_ptr;
    int iter = 0;
    m_global_lb = root_node.m_obj_val;

    while (tree.size() > 0 && m_global_ub - m_global_lb > epsilon) {
        Node current_node(tree.top());
        bool if_pruned = false;
        int first_non_integral_index = -1;
        double temp_lb = DBL_MAX;

        if (current_node.m_status == 1) {
            bool if_integral_solution = check_if_node_integral(current_node, first_non_integral_index);
            if (if_integral_solution) {
                current_node.m_local_lb = current_node.m_obj_val;
                current_node.m_local_ub = current_node.m_obj_val;
                if (current_node.m_local_ub < m_global_ub) {
                    m_global_ub = current_node.m_local_ub;
                }

                if (current_node.m_local_lb < temp_lb) {
                    temp_lb = current_node.m_local_lb;
                }
            }
            else {
                current_node.m_local_ub = m_global_ub;
                current_node.m_local_lb = current_node.m_obj_val;

                if (current_node.m_local_lb < temp_lb) {
                    temp_lb = current_node.m_local_lb;
                }
            }

            // prune by optimality
            if (if_integral_solution) {
                if_pruned = true;
                tree.pop();
            }

            // prune by bound
            if (if_integral_solution == false && current_node.m_local_lb > m_global_ub) {
                if_pruned = true;
                tree.pop();
            }
        }
        else {
            if_pruned = true;
            tree.pop();
        }

        if (if_pruned == false) {
            tree.pop();
            is_root_node = false;
            parent_node_pattern_set = current_node.current_node_pattern_set;
            // create left child
            std::vector<int> left_branching_constraints_index;
            std::vector<int> left_branching_constraints_value;
            std::vector<std::string> left_branching_constraints_char;
            left_branching_constraints_index = current_node.m_branching_constraints_index;
            left_branching_constraints_index.push_back(first_non_integral_index);
            left_branching_constraints_value = current_node.m_branching_constraints_value;
            left_branching_constraints_value.push_back((int)current_node.m_x_sol[first_non_integral_index]);
            left_branching_constraints_char = current_node.m_branching_constraints_char;
            left_branching_constraints_char.push_back("leq");
            Node* left_child_ptr = new Node(is_root_node, m_length_available_lumber, m_cost, m_customer_need, parent_node_pattern_set, left_branching_constraints_index, left_branching_constraints_value, left_branching_constraints_char);
            Node left_child = *left_child_ptr;
            left_child.m_node_depth = current_node.m_node_depth + 1;
            tree.push(left_child);
            delete left_child_ptr;

            // create right child
            std::vector<int> right_branching_constraints_index;
            std::vector<int> right_branching_constraints_value;
            std::vector<std::string> right_branching_constraints_char;
            right_branching_constraints_index = current_node.m_branching_constraints_index;
            right_branching_constraints_index.push_back(first_non_integral_index);
            right_branching_constraints_value = current_node.m_branching_constraints_value;
            right_branching_constraints_value.push_back((int)current_node.m_x_sol[first_non_integral_index]+1);
            right_branching_constraints_char = current_node.m_branching_constraints_char;
            right_branching_constraints_char.push_back("geq");
            Node* right_child_ptr = new Node(is_root_node, m_length_available_lumber, m_cost, m_customer_need, parent_node_pattern_set, right_branching_constraints_index, right_branching_constraints_value, right_branching_constraints_char);
            Node right_child = *right_child_ptr;
            right_child.m_node_depth = current_node.m_node_depth + 1;
            tree.push(right_child);
            delete right_child_ptr;

            // update global lower bound by exploring all leaves
            temp_lb = std::min(left_child.m_obj_val, right_child.m_obj_val);
            if (temp_lb < m_global_lb) {
                m_global_lb = temp_lb;
            }
        }

        iter++;
    }
}
