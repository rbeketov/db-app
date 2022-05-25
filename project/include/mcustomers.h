#pragma once 

#include <string>
#include <unordered_map>


//
dbms::DBTableTxt* dateRange(std::string fileName, types::SQLDate& start, types::SQLDate& end);
std::unordered_map<std::string, std::vector<types::SQLValue*> > customersOrders(dbms::DBTableTxt* orders);
std::unordered_map<std::string, double > customersCash(std::unordered_map<std::string, std::vector<types::SQLValue*> >& customersOrders);
std::string findMainCustomers(std::unordered_map<std::string, double >& customersCash);
std::pair<std::string, std::string> customerInfo(std::string& cid);

//

std::unordered_map<std::string, double> customersBenefit(std::unordered_map<std::string, std::vector<types::SQLValue*> >& customersOrders);
double companyIncome(std::string companyName, types::SQLDate &start, types::SQLDate &end);
std::vector<std::string> companyCustomers(std::string companyName);
std::unordered_map<std::string, double> findCompanyCustomers(std::unordered_map<std::string, double>& custBen, std::vector<std::string> customers);