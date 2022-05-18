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


namespace dbmsLibKB {
//====================����� DBDate======================
 class DBDateKB {	
 private:
	int day;
	int month;
	int year;
	static const int arrDays[13];
	friend std::string DateToStr(DBDateKB& date);
	friend std::ostream& operator<<(std::ostream& out,DBDateKB& date);
 public:
	DBDateKB(std::string date); // ������ ������: dd.mm.yyyy
	DBDateKB(int d, int m, int y);
	DBDateKB():day(0), month(0), year(0){};  // ����������� �� ���������
	DBDateKB(DBDateKB& dat): day(dat.day), month(dat.month), year(dat.year){}
	int GetDay();
	int GetMonth();
	int GetYear();
	bool IsLeapYear (int year);  // ��� ����������?
	int GetDaysInMonth(int month,int year);  // ���������� ���� � ������
	int DaysInCurYear();  // ���������� ���� �� ������ ���� �� ������� ����
	bool operator==(DBDateKB& date);
	bool operator!=(DBDateKB& date);
	bool operator<(DBDateKB& date); 
	bool operator>(DBDateKB& date);
	bool operator<=(DBDateKB& date);
	bool operator>=(DBDateKB& date);
	DBDateKB& operator+=(int days);  // ���������� � ������� ���� days ����
	DBDateKB& operator-=(int days);  // �������� �� ������� ���� days ����
	int operator-(DBDateKB& date);	//���������� ���� ����� ������� ����� � date
									//���� ������� ���� > date, ��������� < 0.
};

//DBType-������������ ����� ����� ������ ��:
enum DBType {
	NoType,
	Int32,
	Double,
	String,
	Date,
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

//ColumnDesc - �������� ������ � ������� ������� 
//������ ���� string ����� ���������� ����� (length) ������ ������ �������, 
//�� � ������ �������� �� ������������ ������ ����� ����������

struct ColumnDesc { 
	char colName[LENGTH];  // ��� �������
	DBType colType;  // ��� ������ � ������� �������
	int length;  // ������������ ����� ��������, ���������� 
	// ��� ������������� ������ � �������
};

struct Strip {  // ������ ���������� �������
	~Strip() { delete[] fieldWidth; }
	int nField;  // ����� ����� 
	int* fieldWidth;  // ������ ����� � ������ (������)
};

typedef std::map<std::string, void*> Row;
typedef std::map<std::string, ColumnDesc> Header;

std::string GetTabNameFromPath(std::string path);
std::string ignoreBlanc(const std::string str);
void* GetValue(std::string value, std::string columnName, Header hdr);
void* SetValue(std::string value, std::string columnName, Header hdr);
bool comparator(DBType type, void *obj1, Condition condition, void *obj);
int GetLength(ColumnDesc colDesc);

//-----------------����� DBDate----------------------------
class DBTable {
public:

	// DBType GetType(char* columnName);

	virtual ~DBTable(){};//����������� ����������
	virtual Header& GetHeader() = 0;
	virtual std::vector<int> IndexOfRecord(void* keyValue, std::string keyColumnName)=0;


	//�� �������� ����� ���������� 
	//���������� ����� (������) ������ (������) � data.
	// keyValue-�������� ����� (��������� �� string, int ��� DBDate)
	// keyColumnName-��� ��������� ������� (��������� ��� ��������� ����)
	//� ������� ����� ���� ��������� ������� � ���������� ��������� ���������� �����.

	// virtual Row GetRow(int index)=0;
	virtual Row operator[](int index) = 0;
	virtual std::string valueToString(Row& row, std::string columnName) = 0;
	// virtual int GetSize()=0;
	//virtual void SetHeader(Header& hdr)=0;
	virtual void ReadDBTable(std::string fileName) = 0;//fileName="..\\"+dbName+"\\"+tableName+".bin"|".txt"											
	virtual void WriteDBTable(std::string fileName) = 0;
	// virtual void WriteDBTableBin(std::string fileName) = 0;
	//virtual void SetTableName(std::string tName)=0;
	//virtual void SetPrimaryKey(std::string key)=0;
	virtual std::string GetTableName()=0;
	//virtual std::string GetPrimaryKey()=0;
	//virtual void SetFileName(std::string path)=0;
	//virtual std::string GetFileName()=0;
	virtual void PrintTable(int screenWidth) = 0;
	virtual Row CreateRow() = 0;
	virtual void AddRow(Row row, int index) = 0;
	virtual DBTable* SelfRows(std::string colName,Condition cond, void* value) = 0;
};



//===============class DBTableTxt=============================

std::string DBTypeToString(DBType type);
DBType stringToDBType(std::string type);

class DBTableTxt: public DBTable {
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
		void CreateTableMaket(Strip* &strips,int &nStrip,int screenWidth);
		std::string GetTableName() {return tableName;}
		// std::string GetPrimaryKey() {return primaryKey;}
		Header& GetHeader() { return columnHeaders; }
		// void SetHeader(Header& hdr) { columnHeaders = hdr; }
		// Row GetRow(int index);
		
		

		// ����� ���������� �������:
		// screenWidth - ������ ������ (������� ��������)
		// nStrip - ����� ����� � ���������� (�������� ��������)
		// strips[nStrip] - �������� ����� �������: 
		// ����� �������� � ������ ������� ������� � ������ (�������� ��������)

		// Lab 6
		DBTable* SelfRows(std::string colName, Condition cond, void* value);
		Row operator[](int index) { return data[index]; };
		std::vector<int> IndexOfRecord(void* keyValue, std::string keyColumnName);
		Row CreateRow();
		void AddRow(Row row, int index = -1);


  };
//=====================����� DBTableBin=====================================

int CreateBinaryDB();

class DBTableBin: public DBTable {
private:
	char tabName[LENGTH];
	char primaryKey[LENGTH];//��� ������� �� ���������� ���������� �����
	int nColumn;
	ColumnDesc* header;				
	int nRows; 
	int RowLength();
	int DataBegin();
	int FieldPosition(std::string colName);//�������� ���� colName �� ������ ������ � ������
	int FieldLength(std::string colName);//����� ���� colName � ������
	void ResizeData(int deltaRows);
	char** data;		
	int maxRows;//����� ����� � data � ������ DELTA (��� ���������� �������//????int maxRows()
	//�� ����� ��������������� ������ nRows+DELTA).� ���� �� ������������
	char fileName[80];//� ���� �� ������������. ������������ � ������� ���
	//��������� � �����
	void CreateTableMaket(Strip *&strips,int &nStrips,int screenWidth);
public:
	DBTableBin(){}
	DBTableBin(std::string tableName) { strcpy(tabName ,tableName.c_str()); }
	//DBTableBin(std::string tableName,Header hdr,std::string primKey) {}		
	~DBTableBin(){/*cout<<"~DBTableBin()"<<endl;*/}
	Header& GetHeader() { Header* tmpH = nullptr; return *tmpH; }
	std::vector<int> IndexOfRecord(void* keyPtr,std::string keyName) { std::vector<int> tmpV; return tmpV;}
	//�� �������� ����� ���������� 
	//���������� ����� (������) ������ (������) � data.
	// keyPtr-�������� ����� (��������� �� string, int ��� DBDate)
	// keyName-��� ��������� ������� (��������� ��� ��������� ����)
	//� ������� ����� ���� ��������� ������� � ���������� ��������� ���������� �����.
	// int FieldPosInFile(int rowIndex, std::string columnName);
	// Row GetRow(int index);

	Row operator[](int index) { Row tmp; std::cout << index << std::endl; return tmp; }

	std::string valueToString(Row& row, std::string columnName);
	//int GetSize(){return nRows;}
	//void SetHeader(Header& hdr);
	void ReadDBTable(std::string fileName);//fileName="..\\"+dbName+"\\"+tableName+".bin"											
	void WriteDBTable(std::string fileName);
	//void SetTableName(std::string tName);
	//void SetPrimaryKey(std::string key);
	std::string GetTableName() {return tabName;}
	//std::string GetPrimaryKey(){return primaryKey;}
	void SetFileName(std::string path){
		strcpy(fileName, (path+"//"+tabName+".bin").c_str());
	} 
	std::string GetFileName(){return fileName;}
	void PrintTable(int screenWidth);
	Row CreateRow() { Row tmpR; return tmpR; };
	void AddRow(Row row,int index) { std::cout << "Hello!" << std::endl; }
	DBTable* SelfRows(std::string colName, Condition cond, void* value) { return nullptr; }
};
//==============����� DBTableSet============================
struct Relation{
	std::string relationName;
	std::string parentTable;
	std::string parentPrimaryKey;
	std::string childTable;
	std::string childSecondaryKey;
};
void PrintRow(Row row, DBTable* tab);
//-------------------------����� DBTableSet---------------------
std::string FileType(std::string dbName);
class DBTableSet {
private:
	std::string dbName;
	std::map<std::string, DBTable*> db;
public: 
	DBTableSet() { db.clear(); };
	DBTableSet(std::string name): dbName(name) {}
	int ReadDB();
	void PrintDB(int numcol);
	void WriteDB();
	std::string GetDBName() {return dbName;}
	// DBTable* operator[](std::string tableName); 
	// Relation GetRelation(std::string relationName);
	// Row ParentRow(Relation& relation,Row& childRow);
	// DBTable* ChildRows(Relation& relation,Row& parentRow);
	friend void ReadDBKB(DBTableSet& db1);
	friend void WriteDBKB(DBTableSet& db1);
	friend void PrintDBKB(DBTableSet& db1, int numcol);
};
const int DELTA = 10;
//����� � 10 ����� ��� ����� ������� � �������. 
//���� ����� ������������, �� ��� ��� �������������� ����� ���������
//�� DELTA ������ ������������� ������� char* data[]. 
//const int LENGTH = 24;//����� ����� ������� � ����� �������.


} // end of namespace dbmsLibKB

