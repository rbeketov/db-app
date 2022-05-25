#include "utils.h"
#include "exceptions.h"
#include "income.h"

double companyIncome(std::string companyName, types::SQLDate &start, types::SQLDate &end)
{
    double income = 999;
    dbms::DBTableTxt *orders = dateRange(DATABASE_ORDERS, start, end);
    std::vector<types::SQLValue *> customers = companyCustomers(companyName);

    return income;
}

std::vector<types::SQLValue *> companyCustomers(std::string companyName)
{
    std::vector<types::SQLValue *> companyCustomers;
    dbms::DBTableTxt customersTable;
    customersTable.ReadDBTable(DATABASE_CUSTOMERS);

    std::vector<int> CIDsInTable = customersTable.IndexOfRecord(new types::SQLString(companyName), "CompanyName");
    for (size_t i = 0; i < CIDsInTable.size(); i++)
    {
        companyCustomers.push_back(customersTable[CIDsInTable[i]]["CustomerID"]);
    }

    return companyCustomers;
}
