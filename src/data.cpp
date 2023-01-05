#include "data.h"

int Data::read_data() {
    std::ifstream input_data_file(this->m_filename.c_str());
    if (!input_data_file) {
        std::cout << "The data file does not exist!" << std::endl;
        exit(-1);
    }

    std::string line;
    bool ready_to_read_customer_demand = false;
    while (std::getline(input_data_file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line[1] == 'L') {
            std::vector<std::string> result;
            boost::split(result, line, boost::is_any_of(">"), boost::token_compress_on);
            boost::trim(result[1]);
            std::vector<std::string> result1;
            boost::split(result1, result[1], boost::is_any_of(","), boost::token_compress_on);
            for (uint i = 0; i < result1.size(); i++) {
                this->m_length_available_lumber.push_back(boost::lexical_cast<double>(result1[i]));
            }
        }
        if (line[1] == 'T') {
            std::vector<std::string> result;
            boost::split(result, line, boost::is_any_of(">"), boost::token_compress_on);
            boost::trim(result[1]);
            std::vector<std::string> result1;
            boost::split(result1, result[1], boost::is_any_of(","), boost::token_compress_on);
            for (uint i = 0; i < result1.size(); i++) {
                this->m_cost.push_back(boost::lexical_cast<double>(result1[i]));
            }
        }
        if (line[1] == 'C') {
            ready_to_read_customer_demand = true;
            continue;
        }
        if (ready_to_read_customer_demand) {
            std::vector<std::string> result;
            boost::split(result, line, boost::is_any_of(","), boost::token_compress_on);
            std::vector<double> temp;
            for (uint i = 0; i < result.size(); i++) {
                temp.push_back(boost::lexical_cast<double>(result[i]));
            }
            this->m_customer_need.push_back(temp);
        }
    }

    input_data_file.close();
    return 1;
}