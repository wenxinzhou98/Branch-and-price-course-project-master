#include "cg.h"

Column_Generation::Column_Generation(std::vector<double> length_available_lumber, std::vector<double> cost, 
        std::vector<std::vector<double> > customer_need, bool is_root_node, 
        std::vector<std::vector<int> > parent_node_pattern_set,
        std::vector<int> branching_constraints_index, std::vector<int> branching_constraints_value, 
        std::vector<std::string> branching_constraints_char) {

    m_length_available_lumber = length_available_lumber;
    m_cost = cost;
    m_customer_need = customer_need;
    m_is_root_node = is_root_node;
    if (m_is_root_node == false) {
        m_parent_node_pattern_set = parent_node_pattern_set;
    }

    m_branching_constraints_index = branching_constraints_index;
    m_branching_constraints_value = branching_constraints_value;
    m_branching_constraints_char = branching_constraints_char;
}

void Column_Generation::construct_ini_pat() {
    if (m_is_root_node) {
        uint customer_need_size = m_customer_need[0].size();
        for (uint i = 0; i < customer_need_size; i++) {
            std::vector<int> temp(customer_need_size, 0);
            temp[i] = 1;
            pattern_set.push_back(temp);
        }
    }
    else {
        pattern_set = m_parent_node_pattern_set;
    }
}

double Column_Generation::compute_pattern_cost(const std::vector<int>& pattern) {
    double pattern_cost = 0;
    int used_lumber = 0;
    uint lumber_size = m_length_available_lumber.size();
    uint customer_need_size = pattern.size();
    for (uint i = 0; i < customer_need_size; i++) {
        used_lumber += pattern[i] * (int)m_customer_need[0][i];
    }
    for (uint i = 1; i < lumber_size; i++) {
        if ((double)used_lumber <= m_length_available_lumber[i-1]) {
            pattern_cost = m_cost[i-1];
            break;
        }
        else if ((double)used_lumber > m_length_available_lumber[i-1] && (double)used_lumber <= m_length_available_lumber[i]) {
            pattern_cost = m_cost[i];
            break;
        }
        else {
            //
        }
    }
    return pattern_cost;
}

double Column_Generation::pricing_sp_ip(const std::vector<double>& omega, const int& index,
        std::vector<int>& added_pattern) {
    try {
        GRBEnv env = GRBEnv(true);
        env.set(GRB_IntParam_LogToConsole, 0);
        env.start();
        GRBModel model = GRBModel(env);
        GRBVar* a = 0;
        a = model.addVars(omega.size(), GRB_INTEGER);
        GRBLinExpr obj = 0;
        for (uint i = 0; i < omega.size(); i++) {
            obj += omega[i] * a[i];
        }
        obj -= m_cost[index];
        model.setObjective(obj, GRB_MAXIMIZE);
        GRBLinExpr con = 0;
        for (uint i = 0; i < m_customer_need[0].size(); i++) {
            con += m_customer_need[0][i] * a[i];
        }
        model.addConstr(con <= m_length_available_lumber[index]);
        model.optimize();
        for (uint j = 0; j < omega.size(); j++) {
            added_pattern.push_back(a[j].get(GRB_DoubleAttr_X));
        }
        delete[] a;
        return model.get(GRB_DoubleAttr_ObjVal);
    } catch (GRBException e) {
        std::cout << "Error code = " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
        exit(-1);
    } catch (...) {
        std::cout << "Exception during optimization" << std::endl;
        exit(-1);
    }
}

double Column_Generation::pricing_sp_dp(const std::vector<double>& omega, const int& index, 
        std::vector<int>& added_pattern) {

    
}

double Column_Generation::solve_RMP() {
    construct_ini_pat();
    uint n_columns = pattern_set.size();
    int iter = 0;

    while (1) {
        // compute restricted master problem
        double* pattern_cost_vector = new double[n_columns]();
        for (uint i = 0; i < n_columns; i++) {
            double pattern_cost = compute_pattern_cost(pattern_set[i]);
            pattern_cost_vector[i] = pattern_cost;
        }
        double* x_lb = new double[n_columns]();
        double* x_ub = new double[n_columns]();
        memset(x_ub, GRB_INFINITY, sizeof(x_ub));
        GRBConstr* constrs = new GRBConstr[m_customer_need[1].size()];
        std::vector<double> x_primal(n_columns, 0.); // primal variables
        std::vector<double> omega(m_customer_need[1].size(), 0.); // dual variables
        double obj_val = 0.;
        try {
            GRBEnv env = GRBEnv(true);
            // env.set("LogFile", "rmp.log");
            env.set(GRB_IntParam_LogToConsole, 0);
            env.start();
            GRBModel model = GRBModel(env);
            GRBVar* x = 0;
            x = model.addVars(n_columns, GRB_CONTINUOUS);
            GRBLinExpr obj = 0;
            for (uint i = 0; i < n_columns; i++) {
                obj += pattern_cost_vector[i] * x[i];
            }
            model.setObjective(obj, GRB_MINIMIZE);
            for (uint i = 0; i < m_customer_need[1].size(); i++) {
                GRBLinExpr cons = 0;
                for (uint j = 0; j < n_columns; j++) {
                    cons += (double)pattern_set[j][i] * x[j];
                }
                constrs[i] = model.addConstr(cons >= m_customer_need[1][i], "Constr_"+i);
            }
            // branching constraints
            for (uint i = 0; i < m_branching_constraints_index.size(); i++) {
                if (m_branching_constraints_char[i] == "geq") {
                    model.addConstr(x[m_branching_constraints_index[i]] >= m_branching_constraints_value[i], "Branching1_"+i);
                }
                if (m_branching_constraints_char[i] == "leq") {
                    model.addConstr(x[m_branching_constraints_index[i]] <= m_branching_constraints_value[i], "Branching2_"+i);
                }
            }
            model.optimize();
            int status = model.get(GRB_IntAttr_Status);
            if (status != 2) {
                m_status = 0;
                return -1;
            }
            for (uint i = 0; i < n_columns; i++) {
                x_primal[i] = x[i].get(GRB_DoubleAttr_X);
            }
            for (uint i = 0; i < m_customer_need[1].size(); i++) {
                // Only the dual variables of the first (m_customer_need[1].size()) constraints are needed.
                omega[i] = constrs[i].get(GRB_DoubleAttr_Pi);
            }
            obj_val = model.get(GRB_DoubleAttr_ObjVal);
            delete[] x;
        } catch (GRBException e) {
            std::cout << "Error code = " << e.getErrorCode() << std::endl;
            std::cout << e.getMessage() << std::endl;
            m_status = 0;
            exit(-1);
        } catch (...) {
            std::cout << "Exception during optimization" << std::endl;
            m_status = 0;
            exit(-1);
        }
        delete[] pattern_cost_vector;
        delete[] x_lb;
        delete[] x_ub;
        delete[] constrs;
        // std::cout << "Iteration " << iter << " RMP solved." << std::endl;

        // Compute new columns
        double maximum_reduced_cost = DBL_MIN;
        std::vector<int> added_pattern;
        for (uint index = 0; index < m_length_available_lumber.size(); index++) {
            std::vector<int> temp_col;
            double temp_reduced_cost = pricing_sp_ip(omega, index, temp_col);
            // std::cout << "  reduced cost = " << temp_reduced_cost << " ";
            if (temp_reduced_cost > 0 && temp_reduced_cost > maximum_reduced_cost) {
                maximum_reduced_cost = temp_reduced_cost;
                added_pattern = temp_col;
            }
        }
        // std::cout << std::endl;

        if (maximum_reduced_cost <= 0.0001) {
            // std::cout << "The column generation subproblem has been solved to optimality. ";
            m_x_sol = x_primal;
            m_status = 1;
            return obj_val;
        }

        pattern_set.push_back(added_pattern);
        n_columns++;
        iter++;
    }
}