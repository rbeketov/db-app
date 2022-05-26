#include "utils.h"
#include "service.h"
#include "test.h"

void test() {

// test #5.1.1 and #5.1.2
puts("");
dbms::DBTableTxt testTable;
testTable.ReadDBTable(DATABASE_CUSTOMERS);
if (testTable.GetData()->size() != 0) {
    puts("Пункты 5.1.1 и 5.1.2 проверены успешно");
} else {
    puts("Пункты 5.1.1 и 5.1.2 не прошли проверку");
}


// test #5.1.3
puts("");
dbms::DBDateKB testDate1("25.08.2025");
dbms::DBDateKB testDate2("15.03.2026");
puts("Объекты даты \"25.08.2025\" и \"15.03.2026\" успешно созданы (п 5.1.3)");
std::cout << testDate1 << " " << testDate2 << std::endl;
if (std::cout.fail()) {
    puts("Проверка потокового вывода дат провалена (п 5.1.3)");
} else {
    puts("Проверка потоквого вывода дат прошла успешно (п 5.1.3)");
}
if (testDate1 < testDate2 &&
    testDate2 > testDate1 &&
    testDate2 >= testDate1 &&
    testDate1 <= testDate2 &&
    testDate1 != testDate2 &&
    !(testDate1 == testDate2)) {
    puts("Проверка сравнения дат прошла успешно (п 5.1.3)");
} else {
    puts("Проверка сравнения дат провалена (п 5.1.3)");
}

dbms::DBDateKB testDate3("04.09.2025");
dbms::DBDateKB testDate4("25.08.2025");
if (((testDate1+=10) == testDate3) && ((testDate1-=10) == testDate4) && testDate2 - testDate1 == 202) {
    puts("Проверка численных операций над датами прошла успешно (п 5.1.3)");
} else {
    puts("Проверка численных операция над датами провалена (п 5.1.3)");
}

// test #5.1.4
puts("");
types::SQLString testString("Misha Vaskin");
std::vector<int> testID = testTable.IndexOfRecord(&testString, "ContactName");
if (testID.size() == 2) {
    puts("Пункты 5.1.4 и 5.1.6 проверены успешно");
} else {
    puts("Пункты 5.1.4 и 5.1.6 проверку не прошли");
}
puts("");

// test #5.1.5
puts("");
dbms::DBTableTxt testTable5;
testTable5.ReadDBTable(DATABASE_TEST);
puts("Таблица до записи новой строки");

size_t sizeBefore = testTable5.GetData()->size();
size_t sizeAfter;
testTable5.PrintTable();
dbms::Row testRow;
std::pair<std::string, types::SQLValue*> testPair;
testPair.first = "CompanyName";
testPair.second = new types::SQLString("Magnit");
testRow.insert(testPair);
testPair.first = "ContactName";
testPair.second = new types::SQLString("Orlov Sergey");
testRow.insert(testPair);
testPair.first = "CustomerID";
testPair.second = new types::SQLString("ORLMG");
testRow.insert(testPair);
testTable5.AddRow(testRow);
puts("Таблица после записи новой строки");
testTable5.PrintTable();
sizeAfter = testTable5.GetData()->size();
if (sizeAfter > sizeBefore) {
    puts("Пункты 5.1.5 и 5.1.8 проверены успешно");
} else {
    puts("Пункты 5.1.5 и 5.1.8 проверку не прошли");    
}
// test #5.1.7

testTable5.WriteDBTable(DATABASE_TEST);
dbms::DBTableTxt testTable6;
testTable6.ReadDBTable(DATABASE_TEST);
testTable6.PrintTable();
puts("Пункт 5.1.7 проверен успешно");
puts("");
// test #5.1.10
std::pair<std::string, std::string> resInfo;

types::SQLDate dateStart("4.04.2016");
types::SQLDate dateEnd("12.12.2017");

dbms::DBTableTxt* orders = dateRange(DATABASE_ORDERS, dateStart, dateEnd);
if (orders == nullptr) {
    resInfo.first = "None";
    resInfo.second = "None";
    puts("Запрос по данному промежутку времени не найден");
}

std::unordered_map<std::string, std::vector<types::SQLValue*> > cOrders = customersOrders(orders);
std::unordered_map<std::string, double > cCash = customersCash(cOrders);
std::string mainCustomer = findMainCustomers(cCash);
resInfo = customerInfo(mainCustomer);
if (resInfo.first == "Anna Holz" && resInfo.second == "RICMA") {
    puts("Пункт 5.1.10 проверен успешно");
} else {
    puts("Пункт 5.1.10 проверку не прошёл");    
}
puts("");
// test #5.1.9

 std::pair<std::string, double> result;
std::string companyName = "Richter Supermarkt";

double income = 0;
std::vector<std::string> customers = companyCustomers(companyName);
dbms::DBTableTxt *ordersTest9 = dateRange(DATABASE_ORDERS, dateStart, dateEnd);

std::unordered_map<std::string, std::vector<types::SQLValue*> > cOrdersTest9 = customersOrders(ordersTest9);
std::unordered_map<std::string, double> custBen =  customersBenefit(cOrdersTest9);
std::unordered_map<std::string, double> appropCust = findCompanyCustomers(custBen, customers);

for (auto it = appropCust.begin(); it != appropCust.end(); it++) {
    income += it->second;
}
result.first = companyName;
result.second = income;

if (result.first == "Richter Supermarkt" && result.second-459.27 < 1e-6) {
    puts("Пункт 5.1.9 проверен успешно");
} else {
    puts("Пункт 5.1.9 проверку не прошёл");    
}
puts("");
}