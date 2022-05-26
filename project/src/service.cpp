#include "utils.h"
#include "exceptions.h"
#include "service.h"

#include <locale>
#include <codecvt>

// support func
dbms::DBTableTxt* dateRange(std::string fileName, types::SQLDate& start, types::SQLDate& end) {
    dbms::DBTableTxt table;
    table.ReadDBTable(fileName);
    dbms::DBTableTxt* chosen= table.SelfRows("OrderDate", LESS, &end);
    chosen = chosen->SelfRows("OrderDate", MORE, &start);
    if (chosen->GetData()->size() == 0) {
        return nullptr;
    }
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
std::pair<types::SQLDate, types::SQLDate> getDate() {
 
    std::wstring letters =
        L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
        L"абвгдеёжщийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
        L",`~/!@#$%^&*()-_+=[]{}|<> ";
    
    std::string start;
    std::string end;
    
    std::wstring bufferCheck;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    puts("Введите период времени. Пример: с 19.04.1958 по 02.06.1958");
    puts("Введите начальную дату:");
    std::cin >> start;
    std::cin.clear();
    std::cin.ignore(10000,'\n');
    bufferCheck = converter.from_bytes(start);
    while (std::cin.fail() || bufferCheck.find_first_of(letters) != std::wstring::npos) {
        puts("Введён некорректный символ, проверьте ввод");
        std::cin >> start;
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        bufferCheck = converter.from_bytes(start);
    }

    puts("Введите конечную дату:");
    std::cin >> end;
    std::cin.clear();
    std::cin.ignore(10000,'\n');
    bufferCheck = converter.from_bytes(end);
    while (std::cin.fail() || bufferCheck.find_first_of(letters) != std::wstring::npos) {
        puts("Введён некорректный символ, проверьте ввод");
        std::cin >> end;
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        bufferCheck = converter.from_bytes(end);
    }
    std::pair<types::SQLDate,types::SQLDate> result{start, end};

    dbms::DBDateKB dateStart(start);
    dbms::DBDateKB dateEnd(end);
    if (dateStart >= dateEnd) {
        throw types::DBTableError(dateStart, dateEnd);
    }
    
    return result;
}

void printCompany() {
    puts("");
    puts("========= Список компаний =========");
    std::set<std::string> namesC;
    dbms::DBTableTxt customers;
    customers.ReadDBTable(DATABASE_CUSTOMERS);
    std::string nameCompany;
    for (size_t i = 0; i < customers.GetData()->size(); i++) {
        customers[i]["CompanyName"]->GetValue(nameCompany);
        namesC.insert(nameCompany);
    }
    size_t counter = 0;
    for (auto it = namesC.begin(); it != namesC.end(); it++) {
        counter++;
        std::cout << counter << " : " << *it << std::endl;
    }
    puts("");
}

// maincustomers
std::pair<std::string, std::string> mainCustomers() {
    std::pair<std::string, std::string> resInfo;

    std::pair<types::SQLDate, types::SQLDate> dates = getDate();
    dbms::DBTableTxt* orders = dateRange(DATABASE_ORDERS, dates.first, dates.second);
    if (orders == nullptr) {
        resInfo.first = "None";
        resInfo.second = "None";
        puts("Запрос по данному промежутку времени не найден");
        return resInfo;
    }

	std::unordered_map<std::string, std::vector<types::SQLValue*> > cOrders = customersOrders(orders);
	std::unordered_map<std::string, double > cCash = customersCash(cOrders);
	std::string mainCustomer = findMainCustomers(cCash);
	resInfo = customerInfo(mainCustomer);
    return resInfo;
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

// income
std::pair<std::string, double> companyIncome() {
    std::pair<std::string, double> result;
    std::string companyName;
    std::cout << "Введите название компании: ";
    std::cin.clear();
    std::cin.ignore(100, '\n');
    getline(std::cin, companyName, '\n');
    double income = 0;
    std::vector<std::string> customers = companyCustomers(companyName);
    if (customers.size() == 0) {
        puts("Компания не найдена");
        puts("Проверьте правильность ввода названия компании");
        result.first = "None";
        result.second = income;
        return result;
    } 

    std::pair<types::SQLDate, types::SQLDate> dates = getDate();
    dbms::DBTableTxt *orders = dateRange(DATABASE_ORDERS, dates.first, dates.second);
    if (orders == nullptr) {
        puts("Запрос по данному промежутку времени не найден");
        result.first = "None";
        result.second = income;
        return result;
    }

    std::unordered_map<std::string, std::vector<types::SQLValue*> > cOrders = customersOrders(orders);
    std::unordered_map<std::string, double> custBen =  customersBenefit(cOrders);
    std::unordered_map<std::string, double> appropCust = findCompanyCustomers(custBen, customers);
    
    for (auto it = appropCust.begin(); it != appropCust.end(); it++) {
        income += it->second;
    }
    result.first = companyName;
    result.second = income;
    return result;
}
std::vector<std::string> companyCustomers(std::string companyName) {
    std::vector<std::string> companyCustomers;
    companyCustomers.clear();

    dbms::DBTableTxt customersTable;
    customersTable.ReadDBTable(DATABASE_CUSTOMERS);

    std::vector<int> CIDsInTable = customersTable.IndexOfRecord(new types::SQLString(companyName), "CompanyName");
    if (CIDsInTable.size() == 0) {
        companyCustomers.clear();
        return companyCustomers;
    }
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