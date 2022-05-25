#include "utils.h"
#include "exceptions.h"
#include "mcustomers.h"

double companyIncome(std::string companyName, types::SQLDate &start, types::SQLDate &end) {
    
    dbms::DBTableTxt *orders = dateRange(DATABASE_ORDERS, start, end);
    std::unordered_map<std::string, std::vector<types::SQLValue*> > cOrders = customersOrders(orders);
    std::unordered_map<std::string, double> custBen =  customersBenefit(cOrders);
    std::vector<std::string> customers = companyCustomers(companyName);
    std::unordered_map<std::string, double> appropCust = findCompanyCustomers(custBen, customers);

    double income = 0;
    for (auto it = appropCust.begin(); it != appropCust.end(); it++) {
        income += it->second;
    }
    return income;
}

std::vector<std::string> companyCustomers(std::string companyName) {
    std::vector<std::string> companyCustomers;
    companyCustomers.clear();

    dbms::DBTableTxt customersTable;
    customersTable.ReadDBTable(DATABASE_CUSTOMERS);
    std::vector<int> CIDsInTable = customersTable.IndexOfRecord(new types::SQLString(companyName), "CompanyName");
    std::string buffer;

    for (size_t i = 0; i < CIDsInTable.size(); i++) {
        customersTable[CIDsInTable[i]]["CustomerID"]->GetValue(buffer);
        companyCustomers.push_back(buffer);
    }

    return companyCustomers;
}

std::unordered_map<std::string, double> customersBenefit(std::unordered_map<std::string, std::vector<types::SQLValue*> >& customersOrders) {
    
    std::unordered_map<std::string, double > customersBenefit;
    customersBenefit.clear();

    dbms::DBTableTxt orderDetails;
    orderDetails.ReadDBTable(DATABASE_ORDER_DETAILS);
    dbms::DBTableTxt products;
    products.ReadDBTable(DATABASE_PRODUCTS);
    
    std::vector<int> oidInOrD; // rows with appropriate OID
    std::vector<int> pidInPrd;
    // types::SQLString* bufPID;
    types::SQLDouble wholesalePrice(0);
    types::SQLDouble outPrice(0);
    types::SQLDouble diff(0);
    types::SQLDouble inter(0);
    types::SQLDouble bufRes(0);
    double result;

    for (auto it = customersOrders.begin(); it != customersOrders.end(); it++) {
        oidInOrD.clear();
        
        bufRes = new types::SQLDouble(0);

        for (size_t i = 0; i < it->second.size(); i++) {
            oidInOrD = orderDetails.IndexOfRecord(it->second[i], "OrderID");
            
            for (size_t j = 0; j < oidInOrD.size(); j++) {
                pidInPrd.clear();
                pidInPrd = products.IndexOfRecord(orderDetails[oidInOrD[j]]["ProductID"], "ProductID");
                
                wholesalePrice = products[pidInPrd[0]]["UnitPrice"];
                outPrice = orderDetails[oidInOrD[j]]["UnitPriceOut"];
                diff = outPrice.Execute(SUB, &wholesalePrice);
                
                inter = orderDetails[oidInOrD[j]]["Quantity"]->Execute(MUL, &diff);


                bufRes = bufRes.Execute(ADD, &inter);
            }
        }
        
        bufRes.GetValue(result);
        customersBenefit[it->first] = result;
    }
    return customersBenefit;
}

std::unordered_map<std::string, double> findCompanyCustomers(std::unordered_map<std::string, double>& custBen, std::vector<std::string> customers) {
    std::unordered_map<std::string, double> appropCust;
    for (size_t i = 0; i < customers.size(); i++) {
        appropCust[customers[i]] = custBen[customers[i]];
    }
    return appropCust;
}

