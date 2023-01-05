#ifndef CG_H_
#define CG_H_

#include <vector>
#include "/opt/gurobi952/linux64/include/gurobi_c++.h"
#include <string.h>
#include <float.h>

class Column_Generation {
public:
    std::vector<double> m_x_sol;
    std::vector<double> m_length_available_lumber;
    std::vector<double> m_cost;
    std::vector<std::vector<double> > m_customer_need;
    std::vector<std::vector<int> > pattern_set;
    bool m_is_root_node; // this parameter has the same meaning as the one in Node class
    std::vector<std::vector<int> > m_parent_node_pattern_set;
    int m_status; // 1, solved successfully; 0, otherwise
    std::vector<int> m_branching_constraints_index;
    std::vector<int> m_branching_constraints_value;
    std::vector<std::string> m_branching_constraints_char;
public:
    Column_Generation(std::vector<double> length_available_lumber, std::vector<double> cost, std::vector<std::vector<double> > customer_need, bool is_root_node, std::vector<std::vector<int> > parent_node_pattern_set, std::vector<int> branching_constraints_index, std::vector<int> branching_constraints_value, std::vector<std::string> branching_constraints_char);
    void construct_ini_pat(); // 0-th element of pattern: number of demand 0; 1...1; 2...2
    double compute_pattern_cost(const std::vector<int>& pattern);
    double solve_RMP();
    double pricing_sp_dp(const std::vector<double>& omega, const int& index, std::vector<int>& added_pattern);  // using DP to solve pricing subproblem
    double pricing_sp_ip(const std::vector<double>& omega, const int& index, std::vector<int>& added_pattern);  // using IP to solve pricing subproblem
};

#endif /*CG_H_*/