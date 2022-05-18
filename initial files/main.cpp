#include "DBLib.h"

enum menuStatus {
	READDB = 1,
	PRINTDB,
	WRITEDB,
	CREATEBINDB,
	READNPRINT,
	TEST = 8,
	EXIT = 10,
};


int menu() {
	std::cout << "================= ����� ���� ===================\n";
	std::cout << "\t1 - ������ �� �� �����\n";
	std::cout << "\t2 - ������ ��\n";
	std::cout << "\t3 - ������ �� � ����\n";
	std::cout << "\t4 - �������� �������� ������ ��������� ��\n";
	std::cout << "\t5 - ���������� � ������ �������� ������ ��\n";
	std::cout << "\t8 � ������������ ��������������� �������\n";
	std::cout << "\t10 - �����\n"; 
	int choice;
	std::cout << "�������� ��������\n";
	std::cin >> choice;
	while (std::cin.fail()) {
		std::cout << "������ �����. ��������� ����\n";	
		std::cin.clear();
		std::cin.ignore(10,'\n');
		std::cin >> choice;
	}
	return choice;
}

void testing(std::string tableName) {

	dbmsLibKB::DBTableTxt db;
	std::cout << "���������� � ������ ������� Students" << std::endl;
	db.ReadDBTable("..\\LibraryTxt\\" + tableName + ".txt");
	
	db.PrintTable(80);
	/*std::cout << "C������s IU5-22B" << std::endl;
	db.SelfRows("Group", dbmsLibKB::Condition::Equal, dbmsLibKB::GetValue("IU5-22B", "Group", db.GetHeader()))->PrintTable(80);
	std::vector<int> indexes(db.IndexOfRecord(dbmsLibKB::GetValue("IU5-22B", "Group", db.GetHeader()), "Group"));
	std::cout << "������ ����� ��������� IU5-22B � �������� �������" << std::endl;
	for (size_t i = 0; i < indexes.size(); i++) {
		std::cout << indexes[i] << std::endl;
	}
	std::cout << std::endl;*/
	dbmsLibKB::Row student = db.CreateRow();
	db.AddRow(student);
	db.PrintTable(80);
	//db.WriteDBTable("..\\LibraryTxt\\" + tableName + ".txt");
}

int main() {
	system("chcp 1251>nul");
	//cout<<"������� ��� ��\n"; //"LibraryTxt" 
	//��� �������� ������������ ��� ������� ��������� ������������
	std::string dbName("LibraryTxt");
	//cin>>dbName;
	std::cout << dbName << std::endl;
	//string tabName("Students");
	//cin>>tabName;
	//	cout<<"������� ������ ������ � ��������\n";
	int screenWidth=78;
	//	cin>>screenWidth;
	dbmsLibKB::DBTableTxt testdb;
	dbmsLibKB::DBTableSet db(dbName);	
	while(true){
		switch(menu()){
			case READDB:
				db.ReadDB();
				break;
			case PRINTDB: 
				db.PrintDB(screenWidth);
				break;
			case WRITEDB: 
				db.WriteDB();
				break;
			case CREATEBINDB:
				if (dbmsLibKB::CreateBinaryDB() == ERROR_TAG) {
					puts("������ �������� �������� ��");
					break;
				}
				puts("�������� �� �������");
				break;
			case READNPRINT:
				testdb.ReadTableBin("..\\LibraryBin\\Abonements.bin");
				testdb.PrintTable(80);
				break;
			case TEST: 
				testing("Abonements");
				break;
			case EXIT: 
				return 0;  // ���������� ������
			default:
				std::cout << "������������ ��������. ��������� �����\n"; 
				break;
		}
	}
	return 0;
}


