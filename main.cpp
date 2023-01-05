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

    return 0;
}
