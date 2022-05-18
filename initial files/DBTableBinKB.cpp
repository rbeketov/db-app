#include "DBLib.h"

namespace dbmsLibKB {

	int CreateBinaryDB() {
		std::string path("..\\LibraryBin\\");
		std::vector<std::string> tablesNames;
		std::ifstream fNames;
		fNames.open(path + "DBTables.txt");

		if (!fNames.is_open()) {
			return ERROR_TAG;		
		}

		tablesNames.clear();
		std::string buffer;
		getline(fNames, buffer);
		size_t posStick;
		while( (posStick = buffer.find('|')) != std::string::npos) {
			std::string res = buffer.substr(0, posStick);
			tablesNames.push_back(res);
			buffer = buffer.substr(++posStick);
		}
		tablesNames.push_back(buffer);

		DBTableTxt table;

		for (size_t i = 0; i < tablesNames.size(); ++i) {
			table.ReadDBTable("..\\LibraryTxt\\" + tablesNames[i] + ".txt");
			table.WriteTableBin(path + tablesNames[i] + ".bin");
		}
		
		return SUCCESSFUL_TAG;
	}



	std::string DBTableBin::valueToString(Row& row, std::string columnName) {
		return columnName;
	}
	void DBTableBin::ReadDBTable(std::string tabName) {
		std::cout << tabName << std::endl;
	}
	void DBTableBin::PrintTable(int screenWidth) {
		std::cout << screenWidth << std::endl;
	}
	void DBTableBin::WriteDBTable(std::string tabName) {
		std::cout << tabName << std::endl;
	}
}