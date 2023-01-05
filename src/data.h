#ifndef DATA_H_
#define DATA_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

class Data {
public:
    std::string m_filename;
    std::vector<double> m_length_available_lumber;
    std::vector<double> m_cost;
    std::vector<std::vector<double> > m_customer_need;
public:
    int read_data();
};

#endif /*DATA_H_*/