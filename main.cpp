#include "src/bb.h"
#include "src/cg.h"
#include "src/data.h"
#include "src/node.h"
#include "src/utils.hpp"

int main(int argc, char** argv) {
    std::string data_filename = "Data/test.txt";
    Data* data_ptr = new Data();
    Data data = *data_ptr;
    data.m_filename = data_filename;
    int status = data.read_data();
    std::vector<double> length_available_lumber = data.m_length_available_lumber;
    std::vector<double> cost = data.m_cost;
    std::vector<std::vector<double> > customer_need = data.m_customer_need;
    delete data_ptr;

    /*
    BB* bp_ptr = new BB(length_available_lumber, cost, customer_need);
    BB bp = *bp_ptr;
    bp.branch_and_price();
    delete bp_ptr;
    */
    
    
    bool is_root_node = true;
    std::vector<std::vector<int> > parent_node_pattern_set;
    std::vector<int> branching_constraints_index;
    std::vector<int> branching_constraints_value;
    std::vector<std::string> branching_constraints_char;
    Column_Generation* cg1_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, branching_constraints_index, branching_constraints_value, branching_constraints_char);
    Column_Generation cg1 = *cg1_ptr;
    double obj_val = cg1.solve_RMP();
    std::cout << "Objective value = " << obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg1.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg1.m_x_sol[i] << std::endl;
    }
    delete cg1_ptr;
    std::cout << std::endl;

    // branching
    bool indicator = true;
    int first_non_integral_index;
    for (uint i = 0; i < cg1.m_x_sol.size(); i++) {
        if (cg1.m_x_sol[i] - (int)cg1.m_x_sol[i] > 1e-3) {
            indicator = false;
            first_non_integral_index = i;
            break;
        }
    }
    std::cout << "left child: x[" << first_non_integral_index << "] <= " << (int)cg1.m_x_sol[first_non_integral_index] << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg1.pattern_set;
    std::vector<int> left_branching_constraints_index;
    std::vector<int> left_branching_constraints_value;
    std::vector<std::string> left_branching_constraints_char;
    left_branching_constraints_index = cg1.m_branching_constraints_index;
    left_branching_constraints_index.push_back(first_non_integral_index);
    left_branching_constraints_value = cg1.m_branching_constraints_value;
    left_branching_constraints_value.push_back((int)cg1.m_x_sol[first_non_integral_index]);
    left_branching_constraints_char = cg1.m_branching_constraints_char;
    left_branching_constraints_char.push_back("leq");
    Column_Generation* cg2_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, left_branching_constraints_index, left_branching_constraints_value, left_branching_constraints_char);
    Column_Generation cg2 = *cg2_ptr;
    double left_obj_val = cg2.solve_RMP();
    std::cout << "Objective value = " << left_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg2.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg2.m_x_sol[i] << std::endl;
    }
    delete cg2_ptr;
    std::cout << std::endl;
    std::cout << "right child: x[" << first_non_integral_index << "] >= " << (int)cg1.m_x_sol[first_non_integral_index]+1 << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg1.pattern_set;
    std::vector<int> right_branching_constraints_index;
    std::vector<int> right_branching_constraints_value;
    std::vector<std::string> right_branching_constraints_char;
    right_branching_constraints_index = cg1.m_branching_constraints_index;
    right_branching_constraints_index.push_back(first_non_integral_index);
    right_branching_constraints_value = cg1.m_branching_constraints_value;
    right_branching_constraints_value.push_back((int)cg1.m_x_sol[first_non_integral_index]+1);
    right_branching_constraints_char = cg1.m_branching_constraints_char;
    right_branching_constraints_char.push_back("geq");
    Column_Generation* cg3_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, right_branching_constraints_index, right_branching_constraints_value, right_branching_constraints_char);
    Column_Generation cg3 = *cg3_ptr;
    double right_obj_val = cg3.solve_RMP();
    std::cout << "Objective value = " << right_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg3.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg3.m_x_sol[i] << std::endl;
    }
    delete cg3_ptr;

    std::cout << std::endl;
    if (left_obj_val < right_obj_val) {
        std::cout << "Branching from left child: x[" << first_non_integral_index << "] <= " << (int)cg1.m_x_sol[first_non_integral_index] << std::endl;
    }
    else {
        std::cout << "Branching from right child: x[" << first_non_integral_index << "] >= " << (int)cg1.m_x_sol[first_non_integral_index]+1 << std::endl;
    }
    indicator = true;
    for (uint i = 0; i < cg2.m_x_sol.size(); i++) {
        if (cg2.m_x_sol[i] - (int)cg2.m_x_sol[i] > 1e-3) {
            indicator = false;
            first_non_integral_index = i;
            break;
        }
    }
    std::cout << std::endl;
    std::cout << "left child: x[" << first_non_integral_index << "] <= " << (int)cg2.m_x_sol[first_non_integral_index] << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg2.pattern_set;
    left_branching_constraints_index = cg2.m_branching_constraints_index;
    left_branching_constraints_index.push_back(first_non_integral_index);
    left_branching_constraints_value = cg2.m_branching_constraints_value;
    left_branching_constraints_value.push_back((int)cg2.m_x_sol[first_non_integral_index]);
    left_branching_constraints_char = cg2.m_branching_constraints_char;
    left_branching_constraints_char.push_back("leq");
    Column_Generation* cg4_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, left_branching_constraints_index, left_branching_constraints_value, left_branching_constraints_char);
    Column_Generation cg4 = *cg4_ptr;
    left_obj_val = cg4.solve_RMP();
    std::cout << "Objective value = " << left_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg4.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg4.m_x_sol[i] << std::endl;
    }
    delete cg4_ptr;
    std::cout << std::endl;
    std::cout << "right child: x[" << first_non_integral_index << "] >= " << (int)cg2.m_x_sol[first_non_integral_index]+1 << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg2.pattern_set;
    right_branching_constraints_index = cg2.m_branching_constraints_index;
    right_branching_constraints_index.push_back(first_non_integral_index);
    right_branching_constraints_value = cg2.m_branching_constraints_value;
    right_branching_constraints_value.push_back((int)cg2.m_x_sol[first_non_integral_index]+1);
    right_branching_constraints_char = cg2.m_branching_constraints_char;
    right_branching_constraints_char.push_back("geq");
    Column_Generation* cg5_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, right_branching_constraints_index, right_branching_constraints_value, right_branching_constraints_char);
    Column_Generation cg5 = *cg5_ptr;
    right_obj_val = cg5.solve_RMP();
    std::cout << "Objective value = " << right_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg5.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg5.m_x_sol[i] << std::endl;
    }
    delete cg5_ptr;
    std::cout << std::endl;
    if (left_obj_val < right_obj_val) {
        std::cout << "Branching from left child: x[" << first_non_integral_index << "] <= " << (int)cg4.m_x_sol[first_non_integral_index] << std::endl;
    }
    else {
        std::cout << "Branching from right child: x[" << first_non_integral_index << "] >= " << (int)cg4.m_x_sol[first_non_integral_index]+1 << std::endl;
    }
    indicator = true;
    for (uint i = 0; i < cg4.m_x_sol.size(); i++) {
        if (cg4.m_x_sol[i] - (int)cg4.m_x_sol[i] > 1e-3) {
            indicator = false;
            first_non_integral_index = i;
            break;
        }
    }
    std::cout << std::endl;
    std::cout << "left child: x[" << first_non_integral_index << "] <= " << (int)cg4.m_x_sol[first_non_integral_index] << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg4.pattern_set;
    left_branching_constraints_index = cg4.m_branching_constraints_index;
    left_branching_constraints_index.push_back(first_non_integral_index);
    left_branching_constraints_value = cg4.m_branching_constraints_value;
    left_branching_constraints_value.push_back((int)cg4.m_x_sol[first_non_integral_index]);
    left_branching_constraints_char = cg4.m_branching_constraints_char;
    left_branching_constraints_char.push_back("leq");
    Column_Generation* cg6_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, left_branching_constraints_index, left_branching_constraints_value, left_branching_constraints_char);
    Column_Generation cg6 = *cg6_ptr;
    left_obj_val = cg6.solve_RMP();
    std::cout << "Objective value = " << left_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg6.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg6.m_x_sol[i] << std::endl;
    }
    delete cg6_ptr;
    std::cout << std::endl;
    std::cout << "right child: x[" << first_non_integral_index << "] >= " << (int)cg4.m_x_sol[first_non_integral_index]+1 << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg4.pattern_set;
    right_branching_constraints_index = cg4.m_branching_constraints_index;
    right_branching_constraints_index.push_back(first_non_integral_index);
    right_branching_constraints_value = cg4.m_branching_constraints_value;
    right_branching_constraints_value.push_back((int)cg4.m_x_sol[first_non_integral_index]+1);
    right_branching_constraints_char = cg4.m_branching_constraints_char;
    right_branching_constraints_char.push_back("geq");
    Column_Generation* cg7_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, right_branching_constraints_index, right_branching_constraints_value, right_branching_constraints_char);
    Column_Generation cg7 = *cg7_ptr;
    right_obj_val = cg7.solve_RMP();
    std::cout << "Objective value = " << right_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg7.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg7.m_x_sol[i] << std::endl;
    }
    delete cg7_ptr;
    std::cout << std::endl;
    if (left_obj_val < right_obj_val) {
        std::cout << "Branching from left child: x[" << first_non_integral_index << "] <= " << (int)cg7.m_x_sol[first_non_integral_index] << std::endl;
    }
    else {
        std::cout << "Branching from right child: x[" << first_non_integral_index << "] >= " << (int)cg7.m_x_sol[first_non_integral_index]+1 << std::endl;
    }
    indicator = true;
    for (uint i = 0; i < cg7.m_x_sol.size(); i++) {
        if (cg7.m_x_sol[i] - (int)cg7.m_x_sol[i] > 1e-3) {
            indicator = false;
            first_non_integral_index = i;
            break;
        }
    }
    std::cout << "left child: x[" << first_non_integral_index << "] <= " << (int)cg7.m_x_sol[first_non_integral_index] << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg7.pattern_set;
    left_branching_constraints_index = cg7.m_branching_constraints_index;
    left_branching_constraints_index.push_back(first_non_integral_index);
    left_branching_constraints_value = cg7.m_branching_constraints_value;
    left_branching_constraints_value.push_back((int)cg7.m_x_sol[first_non_integral_index]);
    left_branching_constraints_char = cg7.m_branching_constraints_char;
    left_branching_constraints_char.push_back("leq");
    Column_Generation* cg8_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, left_branching_constraints_index, left_branching_constraints_value, left_branching_constraints_char);
    Column_Generation cg8 = *cg6_ptr;
    left_obj_val = cg8.solve_RMP();
    std::cout << "Objective value = " << left_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg8.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg8.m_x_sol[i] << std::endl;
    }
    delete cg8_ptr;
    std::cout << std::endl;
    std::cout << "right child: x[" << first_non_integral_index << "] >= " << (int)cg7.m_x_sol[first_non_integral_index]+1 << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg7.pattern_set;
    right_branching_constraints_index = cg7.m_branching_constraints_index;
    right_branching_constraints_index.push_back(first_non_integral_index);
    right_branching_constraints_value = cg7.m_branching_constraints_value;
    right_branching_constraints_value.push_back((int)cg7.m_x_sol[first_non_integral_index]+1);
    right_branching_constraints_char = cg7.m_branching_constraints_char;
    right_branching_constraints_char.push_back("geq");
    Column_Generation* cg9_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, right_branching_constraints_index, right_branching_constraints_value, right_branching_constraints_char);
    Column_Generation cg9 = *cg9_ptr;
    right_obj_val = cg9.solve_RMP();
    std::cout << "Objective value = " << right_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg9.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg9.m_x_sol[i] << std::endl;
    }
    delete cg9_ptr;
    if (left_obj_val < right_obj_val) {
        std::cout << "Branching from left child: x[" << first_non_integral_index << "] <= " << (int)cg8.m_x_sol[first_non_integral_index] << std::endl;
    }
    else {
        std::cout << "Branching from right child: x[" << first_non_integral_index << "] >= " << (int)cg8.m_x_sol[first_non_integral_index]+1 << std::endl;
    }
    indicator = true;
    for (uint i = 0; i < cg8.m_x_sol.size(); i++) {
        if (cg8.m_x_sol[i] - (int)cg8.m_x_sol[i] > 1e-3) {
            indicator = false;
            first_non_integral_index = i;
            break;
        }
    }
    std::cout << "left child: x[" << first_non_integral_index << "] <= " << (int)cg8.m_x_sol[first_non_integral_index] << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg8.pattern_set;
    left_branching_constraints_index = cg8.m_branching_constraints_index;
    left_branching_constraints_index.push_back(first_non_integral_index);
    left_branching_constraints_value = cg8.m_branching_constraints_value;
    left_branching_constraints_value.push_back((int)cg8.m_x_sol[first_non_integral_index]);
    left_branching_constraints_char = cg8.m_branching_constraints_char;
    left_branching_constraints_char.push_back("leq");
    Column_Generation* cg10_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, left_branching_constraints_index, left_branching_constraints_value, left_branching_constraints_char);
    Column_Generation cg10 = *cg6_ptr;
    left_obj_val = cg10.solve_RMP();
    std::cout << "Objective value = " << left_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg10.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg10.m_x_sol[i] << std::endl;
    }
    delete cg10_ptr;
    std::cout << std::endl;
    std::cout << "right child: x[" << first_non_integral_index << "] >= " << (int)cg8.m_x_sol[first_non_integral_index]+1 << std::endl;
    is_root_node = false;
    parent_node_pattern_set = cg8.pattern_set;
    right_branching_constraints_index = cg8.m_branching_constraints_index;
    right_branching_constraints_index.push_back(first_non_integral_index);
    right_branching_constraints_value = cg8.m_branching_constraints_value;
    right_branching_constraints_value.push_back((int)cg8.m_x_sol[first_non_integral_index]+1);
    right_branching_constraints_char = cg8.m_branching_constraints_char;
    right_branching_constraints_char.push_back("geq");
    Column_Generation* cg11_ptr = new Column_Generation(length_available_lumber, cost, customer_need, is_root_node, parent_node_pattern_set, right_branching_constraints_index, right_branching_constraints_value, right_branching_constraints_char);
    Column_Generation cg11 = *cg11_ptr;
    right_obj_val = cg11.solve_RMP();
    std::cout << "Objective value = " << right_obj_val << std::endl;
    std::cout << "x = " << std::endl;
    for (uint i = 0; i < cg11.m_x_sol.size(); i++) {
        std::cout << "  x[" << i << "] = " << cg11.m_x_sol[i] << std::endl;
    }
    delete cg11_ptr;

    return 0;
}