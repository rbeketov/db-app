#pragma once

#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <map>

#define DATABASE_ORDERS "../LibraryTxt/Orders.txt"
#define DATABASE_ORDER_DETAILS "../LibraryTxt/OrderDetails.txt"
#define DATABASE_CUSTOMERS "../LibraryTxt/Customers.txt"

#define LENGTH 24
#define ERROR_TAG -1
#define SUCCESSFUL_TAG 0
#define SCREEN_SIZE 80

enum DBType
{
    NoType,
    Int32,
    Double,
    String,
    Date,
    Bool,
};

enum StatusEx
{
    ADD,
    SUB,
    MUL,
    DIV,
    MORE,
    LESS,
    EQUAL,
};

namespace dbms
{
    // ====================DBDate======================
    class DBDateKB
    {
    private:
        int day;
        int month;
        int year;
        static const int arrDays[13];
        friend std::ostream &operator<<(std::ostream &out, DBDateKB &date);

    public:
        DBDateKB(std::string date);
        DBDateKB(int d, int m, int y);
        DBDateKB(const DBDateKB &val);
        DBDateKB() : day(0), month(0), year(0){};
        DBDateKB(DBDateKB &dat) : day(dat.day), month(dat.month), year(dat.year) {}
        int GetDay();
        int GetMonth();
        int GetYear();
        bool IsLeapYear(int year);
        int GetDaysInMonth(int month, int year);
        int DaysInCurYear();
        DBDateKB &operator=(DBDateKB &date);
        bool operator==(DBDateKB &date);
        bool operator!=(DBDateKB &date);
        bool operator<(DBDateKB &date);
        bool operator>(DBDateKB &date);
        bool operator<=(DBDateKB &date);
        bool operator>=(DBDateKB &date);
        DBDateKB &operator+=(int days);
        DBDateKB &operator-=(int days);
        size_t operator-(DBDateKB &date);
        size_t dateToYlian();
    };

    std::string DateToStr(DBDateKB &date);
} // end of namespace dbms

namespace types
{

    std::string statusToStr(StatusEx status);

    class SQLValue
    {
    public:
        virtual void GetValue(int &val) = 0;
        virtual void GetValue(double &val) = 0;
        virtual void GetValue(std::string &val) = 0;
        virtual void GetValue(dbms::DBDateKB &val) = 0;
        virtual void GetValue(bool &val) = 0;

        virtual void SetValue(const int &val) = 0;
        virtual void SetValue(const double &val) = 0;
        virtual void SetValue(const std::string &val) = 0;
        virtual void SetValue(dbms::DBDateKB &val) = 0;
        virtual void SetValue(const bool &val) = 0;

        virtual DBType GetType() = 0;
        virtual SQLValue *Execute(StatusEx cond, SQLValue *rvalue) = 0;
    };

    class SQLInt32 : public SQLValue
    {
    private:
        int value = 0;

    public:
        SQLInt32(int value) : value(value) {}
        SQLInt32 &operator=(SQLValue *val);

        void GetValue(int &val) override;
        void GetValue(double &val) override;
        void GetValue(std::string &val) override;
        void GetValue(dbms::DBDateKB &val) override;
        void GetValue(bool &val) override;

        void SetValue(const int &val) override;
        void SetValue(const double &val) override;
        void SetValue(const std::string &val) override;
        void SetValue(dbms::DBDateKB &val) override;
        void SetValue(const bool &val) override;

        DBType GetType() override;
        SQLValue *Execute(StatusEx cond, SQLValue *rvalue) override;
    };

    class SQLDouble : public SQLValue
    {
    private:
        double value = 0;

    public:
        SQLDouble(double value) : value(value) {}
        SQLDouble &operator=(SQLValue *val);

        void GetValue(int &val) override;
        void GetValue(double &val) override;
        void GetValue(std::string &val) override;
        void GetValue(dbms::DBDateKB &val) override;
        void GetValue(bool &val) override;

        void SetValue(const int &val) override;
        void SetValue(const double &val) override;
        void SetValue(const std::string &val) override;
        void SetValue(dbms::DBDateKB &val) override;
        void SetValue(const bool &val) override;

        DBType GetType() override;
        SQLValue *Execute(StatusEx cond, SQLValue *rvalue) override;
    };

    class SQLString : public SQLValue
    {
    private:
        std::string value = 0;

    public:
        SQLString(std::string value) : value(value) {}
        SQLString &operator=(SQLValue *val);

        void GetValue(int &val) override;
        void GetValue(double &val) override;
        void GetValue(std::string &val) override;
        void GetValue(dbms::DBDateKB &val) override;
        void GetValue(bool &val) override;

        void SetValue(const int &val) override;
        void SetValue(const double &val) override;
        void SetValue(const std::string &val) override;
        void SetValue(dbms::DBDateKB &val) override;
        void SetValue(const bool &val) override;

        DBType GetType() override;
        SQLValue *Execute(StatusEx cond, SQLValue *rvalue) override;
    };

    class SQLDate : public SQLValue
    {
    private:
        dbms::DBDateKB value;

    public:
        SQLDate(std::string value) : value(value) {}
        SQLDate(dbms::DBDateKB val) { value = val; }
        SQLDate &operator=(SQLValue *val);

        void GetValue(int &val) override;
        void GetValue(double &val) override;
        void GetValue(std::string &val) override;
        void GetValue(dbms::DBDateKB &val) override;
        void GetValue(bool &val) override;

        void SetValue(const int &val) override;
        void SetValue(const double &val) override;
        void SetValue(const std::string &val) override;
        void SetValue(dbms::DBDateKB &val) override;
        void SetValue(const bool &val) override;

        DBType GetType() override;
        SQLValue *Execute(StatusEx cond, SQLValue *rvalue) override;
    };

    class SQLBool : public SQLValue
    {
    private:
        bool value;

    public:
        SQLBool(bool value) : value(value) {}
        SQLBool &operator=(SQLValue *val);

        void GetValue(int &val) override;
        void GetValue(double &val) override;
        void GetValue(std::string &val) override;
        void GetValue(dbms::DBDateKB &val) override;
        void GetValue(bool &val) override;

        void SetValue(const int &val) override;
        void SetValue(const double &val) override;
        void SetValue(const std::string &val) override;
        void SetValue(dbms::DBDateKB &val) override;
        void SetValue(const bool &val) override;

        DBType GetType() override;
        SQLValue *Execute(StatusEx cond, SQLValue *rvalue) override;
    };

} // end of namespace types

namespace dbms
{

    struct ColumnDesc
    {
        char colName[LENGTH];
        DBType colType;
        int length;
    };

    struct Strip
    {
        ~Strip() { delete[] fieldWidth; }
        int nField;
        int *fieldWidth;
    };

    typedef std::map<std::string, types::SQLValue *> Row;
    typedef std::map<std::string, ColumnDesc> Header;

    std::string GetTabNameFromPath(std::string path);
    std::string ignoreBlanc(const std::string str);
    types::SQLValue *GetValue(std::string value, std::string columnName, Header hdr);
    // bool comparator(DBType type, void *obj1, Condition condition, void *obj);

    std::string DBTypeToString(DBType type);
    DBType stringToDBType(std::string type);

    // ============= DBTableTxt ===============
    class DBTableTxt
    {
    private:
        Header columnHeaders;
        std::string tableName;
        std::string primaryKey;
        std::vector<Row> data;
        std::string fileName;

    public:
        DBTableTxt() {}
        DBTableTxt(std::string tabName) : tableName(tabName) {}
        DBTableTxt(std::string tabName, Header hdr, std::string primKey) : columnHeaders(hdr), tableName(tabName), primaryKey(primKey) {}
        ~DBTableTxt();

        std::string valueToString(Row &row, std::string columnName);
        void ReadDBTable(std::string fileName); // tabName=path+tableName
        void PrintTable(int screenWidth = SCREEN_SIZE);
        void WriteDBTable(std::string fileName); //  tabName=path+tableName
        void WriteTableBin(std::string fileName);
        void ReadTableBin(std::string fileName);
        // int GetSize();
        // DBType GetType(char* columnName);

        // void SetFileName(std::string path) { fileName = path; }
        // void SetTableName(std::string tName) { tableName = tName; }
        // void SetPrimaryKey(std::string key) { primaryKey = key; }
        // std::string GetFileName() {return fileName;}
        void CreateTableMaket(Strip *&strips, int &nStrip, int screenWidth);
        std::string GetTableName() { return tableName; }
        // std::string GetPrimaryKey() {return primaryKey;}
        Header &GetHeader() { return columnHeaders; }
        std::vector<Row> *GetData() { return &data; }
        // void SetHeader(Header& hdr) { columnHeaders = hdr; }
        // Row GetRow(int index);

        DBTableTxt *SelfRows(std::string colName, StatusEx cond, types::SQLValue *value);
        Row operator[](int index) { return data[index]; };
        std::vector<int> IndexOfRecord(types::SQLValue *keyValue, std::string keyColumnName);
        Row CreateRow();
        void AddRow(Row row, int index = -1);
    };

} // end of namespace dbms
