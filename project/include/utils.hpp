#pragma once

#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <map>


#define LENGTH  24

#define ERROR_TAG       -1
#define SUCCESSFUL_TAG   0


enum DBType {
	NoType,
	Int32,
	Double,
	String,
	Date,
    Bool,
};

enum StatusEx {
    ADD,
    SUB,
    MUL,
    DIV,
    MORE,
    LESS,
    EQUAL,
};

enum Condition {
	Undefined,
	Equal,
	NotEqual,
	Less,
	Greater,
	LessOrEqual,
	GreaterOrEqual,
};

namespace dbms {

 // ====================DBDate======================
    class DBDateKB {	
    private:
        int day;
        int month;
        int year;
        static const int arrDays[13];
        friend std::ostream& operator<<(std::ostream& out,DBDateKB& date);
    public:
        DBDateKB(std::string date);
        DBDateKB(int d, int m, int y);
        DBDateKB(const DBDateKB& val);
        DBDateKB():day(0), month(0), year(0){};
        DBDateKB(DBDateKB& dat): day(dat.day), month(dat.month), year(dat.year){}
        int GetDay();
        int GetMonth();
        int GetYear();
        bool IsLeapYear (int year);
        int GetDaysInMonth(int month,int year);
        int DaysInCurYear();
        DBDateKB& operator=(DBDateKB& date);
        bool operator==(DBDateKB& date);
        bool operator!=(DBDateKB& date);
        bool operator<(DBDateKB& date); 
        bool operator>(DBDateKB& date);
        bool operator<=(DBDateKB& date);
        bool operator>=(DBDateKB& date);
        DBDateKB& operator+=(int days);
        DBDateKB& operator-=(int days);
        size_t operator-(DBDateKB& date);
        size_t dateToYlian();
    };

    std::string DateToStr(DBDateKB& date);
} // end of namespace dbms



namespace types {

class SQLValue {
    public:
        virtual void GetValue(int& val) = 0;
        virtual void GetValue(double& val) = 0;
        virtual void GetValue(std::string& val) = 0;
        virtual void GetValue(dbms::DBDateKB& val) = 0;
        virtual void GetValue(bool& val) = 0;

        virtual void SetValue(const int& val) = 0;
        virtual void SetValue(const double& val) = 0;
        virtual void SetValue(const std::string& val) = 0;
        virtual void SetValue(dbms::DBDateKB& val) = 0;
        virtual void SetValue(const bool& val) = 0;

        virtual DBType GetType() = 0;
        virtual SQLValue* Execute(StatusEx cond, SQLValue* rvalue) = 0;
};

class SQLInt32: public SQLValue {
    private:
        int value = 0;
    public:
        SQLInt32(int value): value(value) {}
        void GetValue(int& val) override { val = value; }
        void GetValue(double& val) override { throw; }
        void GetValue(std::string& val) override { throw; }
        void GetValue(dbms::DBDateKB& val) override { throw; }
        void GetValue(bool& val) override { throw; }

        void SetValue(const int& val) override { value = val; }
        void SetValue(const double& val) override { throw; }
        void SetValue(const std::string& val) override { throw; }
        void SetValue(dbms::DBDateKB& val) override { throw; }
        void SetValue(const bool& val) override { throw; }

        DBType GetType() override {return DBType::Int32;}
        SQLValue* Execute(StatusEx cond, SQLValue* rvalue) override;
};

class SQLDouble: public SQLValue {
    private:
        double value = 0;
    public:
        SQLDouble(double value): value(value) {}
  
        void GetValue(int& val) override { throw; }
        void GetValue(double& val) override { val = value; }
        void GetValue(std::string& val) override { throw; }
        void GetValue(dbms::DBDateKB& val) override { throw; }
        void GetValue(bool& val) override { throw; }

        void SetValue(const int& val) override { throw; }
        void SetValue(const double& val) override { value = val; }
        void SetValue(const std::string& val) override { throw; }
        void SetValue(dbms::DBDateKB& val) override { throw; }
        void SetValue(const bool& val) override { throw; }

        DBType GetType() override {return DBType::Double;}
        SQLValue* Execute(StatusEx cond, SQLValue* rvalue) override;
};

class SQLString: public SQLValue {
    private:
        std::string value = 0;
    public:
        SQLString(std::string value): value(value) {}
        void GetValue(int& val) override { throw; }
        void GetValue(double& val) override { throw; }
        void GetValue(std::string& val) override { val = value; }
        void GetValue(dbms::DBDateKB& val) override { throw; }
        void GetValue(bool& val) override { throw; }

        void SetValue(const int& val) override { throw; }
        void SetValue(const double& val) override { throw; }
        void SetValue(const std::string& val) override { value = val; }
        void SetValue(dbms::DBDateKB& val) override { throw; }
        void SetValue(const bool& val) override { throw; }

        DBType GetType() override {return DBType::String;}
        SQLValue* Execute(StatusEx cond, SQLValue* rvalue) override;
};

class SQLDate: public SQLValue {
    private:
        dbms::DBDateKB value;
    public:
        SQLDate(std::string value): value(value) {}
        SQLDate(dbms::DBDateKB val) { value = val; }
        void GetValue(int& val) override { throw; }
        void GetValue(double& val) override { throw; }
        void GetValue(std::string& val) override { throw; }
        void GetValue(dbms::DBDateKB& val) override { val = value; }
        void GetValue(bool& val) override { throw; }

        void SetValue(const int& val) override { throw; }
        void SetValue(const double& val) override { throw; }
        void SetValue(const std::string& val) override { throw; }
        void SetValue(dbms::DBDateKB& val) override { value = val; }
        void SetValue(const bool& val) override { throw; }

        DBType GetType() override {return DBType::Date;}
        SQLValue* Execute(StatusEx cond, SQLValue* rvalue) override;
};

class SQLBool: public SQLValue {
    private:
        bool value;
    public:
        SQLBool(bool value): value(value) {}
        void GetValue(int& val) override { throw; }
        void GetValue(double& val) override { throw; }
        void GetValue(std::string& val) override { throw; }
        void GetValue(dbms::DBDateKB& val) override { throw; }
        void GetValue(bool& val) override { val = value; }

        void SetValue(const int& val) override { throw; }
        void SetValue(const double& val) override { throw; }
        void SetValue(const std::string& val) override { throw; }
        void SetValue(dbms::DBDateKB& val) override { throw; }
        void SetValue(const bool& val) override { value = val; }

        DBType GetType() override {return DBType::Bool;}
        SQLValue* Execute(StatusEx cond, SQLValue* rvalue) override;
};


}  // end of namespace types


namespace dbms {
    
    struct ColumnDesc { 
        char colName[LENGTH];
        DBType colType;
        int length;
    };

    struct Strip {
        ~Strip() { delete[] fieldWidth; }
        int nField;
        int* fieldWidth;
    };

    typedef std::map<std::string, types::SQLValue*> Row;
    typedef std::map<std::string, ColumnDesc> Header;

    std::string GetTabNameFromPath(std::string path);
    std::string ignoreBlanc(const std::string str);
    types::SQLValue* GetValue(std::string value, std::string columnName, Header hdr);
    // bool comparator(DBType type, void *obj1, Condition condition, void *obj);

    std::string DBTypeToString(DBType type);
    DBType stringToDBType(std::string type);

    // ============= DBTableTxt ===============
    class DBTableTxt {
        private:
            Header columnHeaders;
            std::string tableName;
            std::string primaryKey; 	
            std::vector<Row> data;
            std::string fileName;
        public:
            DBTableTxt() {}
            DBTableTxt(std::string tabName): tableName(tabName) {}
            DBTableTxt(std::string tabName, Header hdr, std::string primKey): columnHeaders(hdr), tableName(tabName), primaryKey(primKey) {}	
            ~DBTableTxt();
            
            std::string valueToString(Row& row, std::string columnName);
            void ReadDBTable(std::string fileName); // tabName=path+tableName
            void PrintTable(int screenWidth);
            void WriteDBTable(std::string fileName); //  tabName=path+tableName
            void WriteTableBin(std::string fileName);
            void ReadTableBin(std::string fileName);
            // int GetSize();
            // DBType GetType(char* columnName);
            
            // void SetFileName(std::string path) { fileName = path; }
            // void SetTableName(std::string tName) { tableName = tName; }
            // void SetPrimaryKey(std::string key) { primaryKey = key; }
            // std::string GetFileName() {return fileName;}
            void CreateTableMaket(Strip* &strips, int &nStrip, int screenWidth);
            std::string GetTableName() {return tableName;}
            // std::string GetPrimaryKey() {return primaryKey;}
            Header& GetHeader() { return columnHeaders; }
            // void SetHeader(Header& hdr) { columnHeaders = hdr; }
            // Row GetRow(int index);
            
        
            DBTableTxt* SelfRows(std::string colName, StatusEx cond, types::SQLValue* value);
            Row operator[](int index) { return data[index]; };
            std::vector<int> IndexOfRecord(types::SQLValue* keyValue, std::string keyColumnName);
            Row CreateRow();
            void AddRow(Row row, int index = -1);
    };


}  // end of namespace dbms

