#include "DBLib.h"

namespace dbmsLibKB {

std::string FileType(std::string dbName) {
	return '.' + dbName.substr(dbName.size() - 3);
}
 
// ������ ���� ������

 
int DBTableSet::ReadDB() {
	std::vector<std::string> strArray;
	db.clear();
	std::ifstream in;
	std::string path = "..\\" + dbName + "\\";
	in.open(path+"DBTables.txt");
	if (in.is_open()) {
 
		// ���������� ������� ���� ������
		std::string str;
		std::getline(in, str);
		std::string res;
		while (str.find('|') != std::string::npos) {
			res = str.substr(0, str.find_first_of('|'));
			strArray.push_back(res);
			str = str.substr(str.find_first_of('|') + 1);
		}
		strArray.push_back(str);
 
		// ���������� ������ � map � ������ ������������
		std::string fileType = FileType(dbName);
 		for(size_t i = 0; i < strArray.size(); i++){
			if (fileType==".Txt") {
				db[strArray[i]] = new DBTableTxt(strArray[i]);
			} else {
				if (fileType==".Bin") {
					db[strArray[i]] = new DBTableBin(strArray[i]);
				} else {
					std::cout << "ReadDB5: ������ ����� ��" << std::endl;
					return -1;
				}
			}
			db[strArray[i]]->ReadDBTable(path+strArray[i]+fileType);
		}
	} else {
		std::cout << "������ �������� DBTables.txt" << std::endl;
	}
	return 0;
}

void DBTableSet::WriteDB() {
	std::string path = "..\\" + dbName + "\\";
	int i = 0;
	for (auto iter = db.begin(); iter != db.end(); iter++) {
		iter->second->WriteDBTable(path+iter->first+".txt");
		++i;
	}
}

void DBTableSet::PrintDB(int numcol) {
	for (auto iter = db.begin(); iter != db.end(); iter++) {
		iter -> second -> PrintTable(numcol);
	}
}

}