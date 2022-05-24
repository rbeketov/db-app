#include "utils.h"
#include "exceptions.h"
#include "mcustomers.h"


dbms::DBTableTxt* dateRange(std::string fileName, types::SQLDate& start, types::SQLDate& end) {
    dbms::DBTableTxt table;
    table.ReadDBTable(fileName);
    dbms::DBTableTxt* chosen= table.SelfRows("OrderDate", LESS, &end);
    chosen = chosen->SelfRows("OrderDate", MORE, &start);
    return chosen;
}

std::unordered_map<std::string, std::vector<types::SQLValue*> > customersOrders(dbms::DBTableTxt* orders) {
    std::unordered_map<std::string, std::vector<types::SQLValue*> > result;
    result.clear();
    std::string CID;
    for (size_t i = 0; i < orders->GetData()->size(); i++) {
       (*orders->GetData())[i]["CustomerID"]->GetValue(CID); // get CID
       result[CID].push_back((*orders->GetData())[i]["OrderID"]); // add OID to CID
    }
    return result;
}

std::unordered_map<std::string, double > customersCash(std::unordered_map<std::string, std::vector<types::SQLValue*> >& customersOrders) {
    std::unordered_map<std::string, double > customersCash;
    customersCash.clear();

    dbms::DBTableTxt orderDetails;
    orderDetails.ReadDBTable(DATABASE_ORDER_DETAILS);
    
    std::vector<int> numOfRow; // rows with appropriate OID

    types::SQLDouble bufDouble(0);
    types::SQLDouble bufRes(0);
    double result;
 
    for (auto it = customersOrders.begin(); it != customersOrders.end(); it++) {
        numOfRow.clear();
        bufRes = new types::SQLDouble(0);

        for (size_t i = 0; i < it->second.size(); i++) {
            numOfRow = orderDetails.IndexOfRecord(it->second[i], "OrderID");

            for (size_t j = 0; j < numOfRow.size(); j++) {
                bufDouble = orderDetails[numOfRow[j]]["Quantity"]->Execute(MUL, orderDetails[numOfRow[j]]["UnitPriceOut"]);
                bufRes = bufRes.Execute(ADD, &bufDouble);
            }
        }
        
        bufRes.GetValue(result);
        customersCash[it->first] = result;
    }
    return customersCash;
}

std::string findMainCustomers(std::unordered_map<std::string, double >& customersCash) {
    std::string result;
    double max_ = 0;
    
    for (auto it = customersCash.begin(); it != customersCash.end(); it++) {
        if ( it->second > max_) {
            max_ = it->second;
            result = it->first;
        }
    }
    return result;
}

std::pair<std::string, std::string> customerInfo(std::string& cid) {
    std::pair<std::string, std::string> result;

    dbms::DBTableTxt customers;
    customers.ReadDBTable(DATABASE_CUSTOMERS);

    std::vector<int> resultIndex;

    types::SQLString cidSQL(cid);
    resultIndex = customers.IndexOfRecord(&cidSQL, "CustomerID");
    customers[resultIndex[0]]["ContactName"]->GetValue(result.first);
    result.second = cid;

    return result;
}