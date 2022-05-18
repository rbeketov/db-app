#include "utils.hpp"

namespace dbms {

DBType stringToDBType(std::string type) {
	if (type == "NoType") {
		return NoType;
	}
	if (type == "Int32") {
		return Int32;
	}
	if (type == "Double") {
		return Double;
	}
	if (type == "String") {
		return String;
	}
	if (type == "Date") {
		return Date;
	}
	return NoType;
}

std::string DBTypeToString(DBType type) {
	switch (type) {
		case NoType: return "NoType";
		case Int32: return "Int32";
		case Double: return "Double";
		case String: return "String";
		case Date: return "Date";
		default:
			exit(1);
			break;
	}
}


void DBTableTxt::ReadDBTable(std::string fileName) {
	std::ifstream tabBd(fileName);

	if (!tabBd.is_open()) {
		puts("Ошибка открытия (ReadDBTable)");
		throw;
	}
	std::string buffer;
	getline(tabBd, buffer, '|');

	if (buffer != GetTabNameFromPath(fileName)) {
		tabBd.close();
		puts("Имя таблицы не совпадает с названием файла (ReadDBTable)");
		return;
	}

	this->fileName = fileName;
	tableName = buffer;
	getline(tabBd, buffer);
	primaryKey = buffer;

	getline(tabBd, buffer);

	// header
	size_t posStart = 0;
	size_t posEnd = 0;

	columnHeaders.clear();
	std::pair<std::string, ColumnDesc> tmpData;
	ColumnDesc tmpCD;

	size_t cntRows = 0;
	while(true) {
		if ((posEnd = buffer.find('|', posStart)) == std::string::npos) {
			tabBd.close();
			puts("Некорректная таблица (ReadDBTable)");
			throw;
	    }
		tmpData.first = buffer.substr(posStart, posEnd - posStart);
		strcpy(tmpCD.colName, tmpData.first.c_str());

		posStart = posEnd+1;
		if ((posEnd = buffer.find('|', posStart)) == std::string::npos) {
			tabBd.close();
			puts("Некорректная таблица (ReadDBTable)");
			throw;
		}
		tmpCD.colType = stringToDBType(buffer.substr(posStart, posEnd - posStart));
		posStart = posEnd+1;
		if ((posEnd = buffer.find('|', posStart)) == std::string::npos) {
			tmpCD.length = stoi(buffer.substr(posStart));
			tmpData.second = tmpCD;
			columnHeaders.insert(tmpData);
			cntRows++;
			break;
		}
		tmpCD.length = stoi(buffer.substr(posStart, posEnd - posStart));
		posStart = posEnd+1;
		tmpData.second = tmpCD;
		columnHeaders.insert(tmpData);
		cntRows++;
	}



	// rows 
	std::string tmpValue;
	Row tmpRow = *(new Row());
	std::pair<std::string, types::SQLValue*> tmpPair;
	data.clear();
	while(getline(tabBd, buffer)) {
		posStart = posEnd = 0;
		buffer = ignoreBlanc(buffer);
		for (auto it = columnHeaders.begin(); it != --columnHeaders.end(); ++it) {
			if ((posEnd = buffer.find('|', posStart)) == std::string::npos) {
				tabBd.close();
				puts("Некорректная такблица (ReadDBTable)");
				throw;
			}

			tmpValue = buffer.substr(posStart, posEnd - posStart);
			tmpPair.first = it->first;
			tmpPair.second = GetValue(tmpValue, it->first, columnHeaders);
			tmpRow.insert(tmpPair);

			posStart = posEnd + 1;
		}

		tmpValue = buffer.substr(posStart, posEnd - posStart);
		tmpPair.first = (--columnHeaders.end())->first;
		tmpPair.second = GetValue(tmpValue, (--columnHeaders.end())->first, columnHeaders);
		tmpRow.insert(tmpPair);

		data.push_back(tmpRow);
		tmpRow.clear();
	}
	tabBd.close();



}


void DBTableTxt::PrintTable(int screenWidth) {
	std::cout << '\n' << "Таблица " << tableName << std::endl;
	std::cout << std::setfill('=') << std::setw (screenWidth - 1) << '=' << std::setfill(' ') << std::endl;

	Strip* strips;
	int nStrip;
	Header::iterator headIter;
	Header::iterator contHeaderIter;
	Row::iterator rowIter;
	Row::iterator contRowIter;

	CreateTableMaket(strips, nStrip, screenWidth);

	contHeaderIter = columnHeaders.begin();
	contRowIter = data[0].begin();

	for (int r = 0; r < nStrip; r++) {
		if (r > 0) {
			std::cout << "Продолжение таблицы " << tableName << std::endl;
			std::cout << std::setfill('=') << std::setw (screenWidth - 1) << '=' << std::setfill(' ') << std::endl;
		} 
		headIter = contHeaderIter;
		for (int j = 0; j < strips[r].nField; j++) {
			std::cout << std::setw(strips[r].fieldWidth[j]) << headIter->first;
			headIter++;
		}
		std::cout << std::endl;
		headIter = contHeaderIter;
		for (int j = 0; j < strips[r].nField; j++) {
			std::cout << std::setw(strips[r].fieldWidth[j]) << DBTypeToString(headIter->second.colType);
			headIter++;
		}
		std::cout << '\n' << std::setfill('-') << std::setw (screenWidth - 1) << '-' << std::setfill(' ') << std::endl;
		for (int i = 0; i < data.size(); i++) {
			rowIter = contRowIter;
			for (int j = 0; j < strips[r].nField; j++) {
				std::cout << std::setw(strips[r].fieldWidth[j]) << valueToString(data[i], rowIter->first);
				rowIter++;
			}
			std::cout << std::endl;
		}
		contHeaderIter = headIter;
		contRowIter = rowIter;
		std::cout << std::setfill('=') << std::setw (screenWidth - 1) << '=' << std::setfill(' ') << '\n' << std::endl;
	}
	delete[] strips;
}

void DBTableTxt::WriteDBTable(std::string fileName) {
	std::fstream tabBd(fileName);
	if (!tabBd.is_open()) {
		puts("Ошибка открытия (WriteDBTable)");
		return;
	}

	std::string buffer;
	std::getline(tabBd, buffer, '|');
	if (buffer != GetTabNameFromPath(fileName)) {
		tabBd.close();
		puts("Название таблицы не совпадает с название файла (WriteDBTable)");
		return;
	}

	tabBd.seekg(0);
	tabBd << tableName << '|' << primaryKey << '\n';
	auto iter = columnHeaders.begin();
	auto iterWhile = iter;

	while (++iterWhile != columnHeaders.end())
	{
		tabBd << iter->second.colName << '|' << DBTypeToString(iter->second.colType) << '|' << iter->second.length << '|';
		iter++;
	}
	tabBd << iter->first << '|' << DBTypeToString(iter->second.colType) << '|' << iter->second.length << '\n';

	for (size_t i = 0; i < data.size(); ++i) {
		for (auto it = columnHeaders.begin(); it != --columnHeaders.end(); ++it) {
			tabBd << valueToString(data[i], it->first) << '|';	
		}
		tabBd << valueToString(data[i], (--columnHeaders.end())->first) << '\n';
	}

	tabBd.close();
}

types::SQLValue* GetValue(std::string value, std::string columnName, Header hdr) {
	switch (hdr[columnName].colType) {
		case Int32:	
			return new types::SQLInt32(stoi(value)); 
		case Double: 
			return new types::SQLDouble(stod(value));
		case String:
			return new types::SQLString(value);
		case Date:
			return new types::SQLDate(value);
		default:
			std::cout << "Подан несуществующий тип (GetValue): " << columnName << std::endl;
			return nullptr;
	}
}

std::string DBTableTxt::valueToString(Row& row, std::string columnName) {

	int bufInt;
	double bufDouble;
	dbms::DBDateKB bufDate;
	std::string bufString;
	if (this->columnHeaders.find(columnName) != this->columnHeaders.end()) {
		switch(columnHeaders[columnName].colType) {
			case Int32:
				row[columnName]->GetValue(bufInt);
				return std::to_string(bufInt);
			case Double:
				row[columnName]->GetValue(bufDouble);
				return std::to_string(bufDouble);
			case String:
				row[columnName]->GetValue(bufString);
				return bufString;
			case Date:
				row[columnName]->GetValue(bufDate);
				return dbms::DateToStr(bufDate);
			default: 
				std::cout << std::setw(8) << "NoType" << '\0'; 
				system("pause");
				break;
		}
	}
	puts("Такого название cтолбца не существует (valueToString)");
	return "";
}

std::string ignoreBlanc(const std::string str){		
	std::string bufStr = str;
	int begStr = bufStr.find_first_not_of(' ');
	return bufStr.substr(begStr);
}

std::string GetTabNameFromPath(std::string path) { 
	size_t posPoint = path.find_last_of('.');
	size_t posSlash = path.find_last_of('/');
	return path.substr(posSlash+1, posPoint-posSlash-1);
}

// bool comparator(DBType type, void *obj1, Condition condition, void *obj2) {
// 	switch (type) {
// 		case Int32: switch (condition) {
// 			case Equal: 
// 				return *(int*)obj1 == *(int*)obj2;
// 			case NotEqual:
// 				return *(int*)obj1 != *(int*)obj2;
// 			case Less:
// 				return *(int*)obj1 < *(int*)obj2;
// 			case Greater:
// 				return *(int*)obj1 > *(int*)obj2;
// 			case LessOrEqual:
// 				return *(int*)obj1 <= *(int*)obj2;
// 			case GreaterOrEqual:
// 				return *(int*)obj1 >= *(int*)obj2;
// 			default: 
// 				std::cout << "������������ �������� ���������\n" << std::endl;
// 				return false;
// 			}
// 		case Double: switch (condition) {
// 			case Equal:
// 				return *(double*)obj1 == *(double*)obj2;
// 			case NotEqual:
// 				return *(double*)obj1 != *(double*)obj2;
// 			case Less:
// 				return *(double*)obj1 < *(double*)obj2;
// 			case Greater:
// 				return *(double*)obj1 > *(double*)obj2;
// 			case LessOrEqual:
// 				return *(double*)obj1 <= *(double*)obj2;
// 			case GreaterOrEqual:
// 				return *(double*)obj1 >= *(double*)obj2;
// 			default: 
// 				std::cout << "������������ �������� ���������\n" << std::endl;
// 				return false;
// 			}
// 		case String: switch (condition) {
// 			case Equal:
// 				return ignoreBlanc(*(std::string*)obj1) == ignoreBlanc(*(std::string*)obj2);
// 			case NotEqual:
// 				return ignoreBlanc(*(std::string*)obj1) != ignoreBlanc(*(std::string*)obj2);
// 			case Less:
// 				return ignoreBlanc(*(std::string*)obj1) < ignoreBlanc(*(std::string*)obj2);
// 			case Greater:
// 				return ignoreBlanc(*(std::string*)obj1) > ignoreBlanc(*(std::string*)obj2);
// 			case LessOrEqual:
// 				return ignoreBlanc(*(std::string*)obj1) <= ignoreBlanc(*(std::string*)obj2);
// 			case GreaterOrEqual:
// 				return ignoreBlanc(*(std::string*)obj1)>= ignoreBlanc(*(std::string*)obj2);
// 			default: std::cout << "������������ �������� ���������\n" << std::endl;
// 				return false;
// 			}
// 		case Date: switch (condition){
// 			case Equal:
// 				return *(dbms::DBDateKB*)obj1 == *(dbms::DBDateKB*)obj2;
// 			case NotEqual:
// 				return *(dbms::DBDateKB*)obj1 != *(dbms::DBDateKB*)obj2;
// 			case Less:
// 				return *(dbms::DBDateKB*)obj1 < *(dbms::DBDateKB*)obj2;
// 			case  Greater:
// 				return *(dbms::DBDateKB*)obj1 > *(dbms::DBDateKB*)obj2;
// 			case LessOrEqual:
// 				return *(dbms::DBDateKB*)obj1 <= *(dbms::DBDateKB*)obj2;
// 			case GreaterOrEqual:
// 				return *(dbms::DBDateKB*)obj1 >= *(dbms::DBDateKB*)obj2;
// 			default:
// 				std::cout << "������������ �������� ���������\n" << std::endl;
// 				return false;
// 			 }	
		
// 		default:
// 			std::cout << "������������ ��� ������\n" << std::endl;
// 			return false;
// 	}
// }

void DBTableTxt::CreateTableMaket(Strip *&strips,int &nStrips,int screenWidth) {
	Header::iterator headerIter,contHeaderIter;		
	int nColumn = (int)columnHeaders.size();
	int* fieldW = new int[nColumn];
	headerIter = columnHeaders.begin();
	for(int k = 0; k < nColumn; k++){
		fieldW[k] = headerIter->second.length > headerIter->first.size() ? headerIter->second.length+2 : headerIter->first.size() + 2;
		headerIter++;
	}
	int currCol = 0;
	nStrips = 1;
	int sumWidth = 0;
	int n = 0;
	int buff[40]={0};
	for (currCol = 0; currCol < nColumn; currCol++){
		if (fieldW[currCol] >= screenWidth-1) {
			std::cout << "������ ������� " << currCol << " ������ ������ ������\n";
			std::cout << "������� ������ �����������" << std::endl;
			nStrips = 0;
			return;
		}
		sumWidth += fieldW[currCol];
		if ((sumWidth<screenWidth-1) && (currCol<nColumn-1)) {
			n++;
			continue;
		}
		if ((sumWidth >= screenWidth - 1)) {
			currCol--;				
			buff[nStrips-1] = n;
			nStrips++;
			n = 0;
			sumWidth = 0;
			continue;
		}
		if (currCol == nColumn - 1) {
			n++;
			buff[nStrips-1] = n;
		}			
	}
	strips = new Strip[nStrips];   
	int col = 0;
	for (int i = 0; i < nStrips; i++) {
		strips[i].nField = buff[i];
		strips[i].fieldWidth = new int[strips[i].nField];
		for (int j=0; j < strips[i].nField; j++)
			strips[i].fieldWidth[j] = fieldW[col++];
	}				
}

DBTableTxt::~DBTableTxt() {
// 	std::string type;
// 	for (size_t i = 0; i < data.size(); i++) {
// 		for (auto it = data[i].begin(); it != data[i].end(); ++it) {
// 			type = DBTypeToString(columnHeaders[it->first].colType);
// 			if (type == "Int32") { delete (int*) it->second; } 
// 			else if (type == "Double") { delete (double*) it->second; }
// 			else if (type == "String") { delete (std::string*) it->second; }
// 			else if (type == "DBDate") { delete (dbms::DBDateKB*) it->second; }
// 		}
// 	}
}


DBTableTxt* DBTableTxt::SelfRows(std::string colName, StatusEx cond, types::SQLValue* value) {
	bool bVal;
	std::string tabName = "SR" + GetTableName();
	DBTableTxt* tab = new DBTableTxt(tabName);
	tab->columnHeaders = columnHeaders;
	for (size_t i = 0; i < data.size(); i++) {
		data[i][colName]->Execute(cond, value)->GetValue(bVal);
		if (bVal) {
			tab->data.push_back(data[i]);
		}
	}
	return tab;
}
	
std::vector<int> DBTableTxt::IndexOfRecord(types::SQLValue* keyValue, std::string keyColumnName) {
	std::vector<int> bufOfIndex;
	bufOfIndex.clear();
	bool flag = false;
	for (auto it = columnHeaders.begin(); it != columnHeaders.end(); it++) {
		if (it->first == keyColumnName) {
			flag = true;
			break;
		}
	}
	if (!flag) {
		return bufOfIndex;
	}
	bool bVal;
	for (size_t i = 0; i < data.size(); i++) {
		data[i][keyColumnName]->Execute(StatusEx::EQUAL, keyValue)->GetValue(bVal);
		if (bVal) {
			bufOfIndex.push_back(i);
		}
	}
	return bufOfIndex;
}

Row DBTableTxt::CreateRow() {
	Row row;
	std::pair<std::string, types::SQLValue*> tmpPair;
	std::string tmpValue;
	for (auto it = columnHeaders.begin(); it != columnHeaders.end(); it++) {
		std::cout << "Введите поле \"" + it->first << "\" (максимальная длинна - " << it->second.length << ", тип - " << DBTypeToString(it->second.colType) << ")" << std::endl;
		tmpPair.first = it->first;

		std::cin >> tmpValue;
		while ((tmpValue.length() > it->second.length || std::cin.fail()) && tmpValue != "!q") {
			std::cout << "Слишком большой текст, повторите попытку ввода\n для отмены ввода отправьте !q\n";
			std::cin.clear();
			std::cin.ignore(25,'\n');
			std::cin >> tmpValue;
		}
		if (tmpValue == "!q") {
			row.clear();
			puts("Сброс ввода");
			return row;
		}
		tmpPair.second = GetValue(tmpValue, it->first, columnHeaders);
		row.insert(tmpPair);
	}
	return row;
}

void DBTableTxt::AddRow(Row row, int index) {
	if (index == -1) {
		data.push_back(row);
	} else if (index >= 0) {
		data[index] = row;
	} else {
		puts("Введён некорректный индекс (AddRow)");
	}
}

}  // dbTableKB


