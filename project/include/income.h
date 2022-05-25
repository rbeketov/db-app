#pragma once

#include <string>
#include <unordered_map>
#include "mcustomers.h"

double companyIncome(std::string companyName, types::SQLDate &start, types::SQLDate &end);
std::vector<types::SQLValue *> companyCustomers(std::string companyName);