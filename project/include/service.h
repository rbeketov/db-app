#pragma once 

#include <string>
#include <unordered_map>
#include <set>


// supp func
dbms::DBTableTxt* dateRange(std::string fileName, types::SQLDate& start, types::SQLDate& end);
std::unordered_map<std::string, std::vector<types::SQLValue*> > customersOrders(dbms::DBTableTxt* orders);
std::pair<types::SQLDate,types::SQLDate> getDate();
void printCompany();

// maincustomers
std::pair<std::string, std::string> mainCustomers();
std::unordered_map<std::string, double > customersCash(std::unordered_map<std::string, std::vector<types::SQLValue*> >& customersOrders);
std::string findMainCustomers(std::unordered_map<std::string, double >& customersCash);
std::pair<std::string, std::string> customerInfo(std::string& cid);

// income
std::pair<std::string, double> companyIncome();
std::unordered_map<std::string, double> customersBenefit(std::unordered_map<std::string, std::vector<types::SQLValue*> >& customersOrders);
std::vector<std::string> companyCustomers(std::string companyName);
std::unordered_map<std::string, double> findCompanyCustomers(std::unordered_map<std::string, double>& custBen, std::vector<std::string> customers);