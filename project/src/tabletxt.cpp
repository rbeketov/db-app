#include "utils.h"
#include "exceptions.h"

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
	throw types::СonversionError();
}

std::string DBTypeToString(DBType type) {
	switch (type) {
		case NoType: return "NoType";
		case Int32: return "Int32";
		case Double: return "Double";
		case String: return "String";
		case Date: return "Date";
		default:
			throw types::СonversionError();
	}
}

void DBTableTxt::ReadDBTable(std::string fileName) {
	std::ifstream tabBd(fileName);

	if (!tabBd.is_open()) {
		throw types::DBTableError(fileName);
	}
	std::string buffer;
	getline(tabBd, buffer, '|');

	if (buffer != GetTabNameFromPath(fileName)) {
		tabBd.close();
		throw types::DBTableError(fileName, buffer);
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
			throw types::DBTableError(tableName, ERROR_TAG);
	    }
		tmpData.first = buffer.substr(posStart, posEnd - posStart);
		strcpy(tmpCD.colName, tmpData.first.c_str());

		posStart = posEnd+1;
		if ((posEnd = buffer.find('|', posStart)) == std::string::npos) {
			tabBd.close();
			throw types::DBTableError(tableName, ERROR_TAG);
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
				throw types::DBTableError(tableName, ERROR_TAG);
			}

			tmpValue = buffer.substr(posStart, posEnd - posStart);
			tmpValue = ignoreBlanc(tmpValue);
			tmpPair.first = it->first;
			tmpPair.second = GetValue(tmpValue, it->first, columnHeaders);
			tmpRow.insert(tmpPair);

			posStart = posEnd + 1;
		}

		tmpValue = buffer.substr(posStart, posEnd - posStart);
		tmpValue = ignoreBlanc(tmpValue);
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
		throw types::DBTableError(fileName);
	}

	std::string buffer;
	std::getline(tabBd, buffer, '|');
	if (buffer != GetTabNameFromPath(fileName)) {
		tabBd.close();
		throw types::DBTableError(fileName, buffer);
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
			throw types::СonversionError();
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
	throw types::DBTableError(row, columnName);
}

std::string ignoreBlanc(const std::string str){		
	std::string bufStr = str;
	size_t begStr = bufStr.find_first_not_of(' ');
	size_t endStr = bufStr.find_last_not_of(' ');
	bufStr = bufStr.substr(begStr, endStr-begStr+1);
	return bufStr;
	
}

std::string GetTabNameFromPath(std::string path) { 
	size_t posPoint = path.find_last_of('.');
	size_t posSlash = path.find_last_of('/');
	return path.substr(posSlash+1, posPoint-posSlash-1);
}

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
			nStrips = 0;
			throw types::DBTableError(currCol);
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
	std::string tabName = GetTableName();
	DBTableTxt* tab = new DBTableTxt(tabName);
	tab->columnHeaders = columnHeaders;
	for (size_t i = 0; i < data.size(); i++) {
		data[i][colName]->Execute(cond, value)->GetValue(bVal);
		// std::cout << bVal << std::endl;
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
		Row row;
		throw types::DBTableError(row, keyColumnName);
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
	try {
		for (auto it = columnHeaders.begin(); it != columnHeaders.end(); it++) {
			std::cout << "Введите значение поля без пробелов \"" + it->first << "\" (максимальная длинна - " << it->second.length << ", тип - " << DBTypeToString(it->second.colType) << ")" << std::endl;
			tmpPair.first = it->first;

			std::cin >> tmpValue;
			while ((tmpValue.length() > it->second.length || std::cin.fail())  && tmpValue != "!q") {
				std::cout << "Ошибка ввода, повторите попытку ввода\nДля отмены ввода отправьте !q\n";
				std::cin.clear();
				std::cin.ignore(1000,'\n');
				std::cin >> tmpValue;
			}
			if (tmpValue == "!q") {
				row.clear();
				puts("Сброс ввода");
				return row;
			}
			tmpPair.second = GetValue(tmpValue, it->first, columnHeaders);
			row.insert(tmpPair);
			std::cin.clear();
			std::cin.ignore(100,'\n');
		}
	} catch (...) {
		row.clear();
		throw types::NewRowError();
	}
	return row;
}

void DBTableTxt::AddRow(Row row, int index) {
	if (index == -1) {
		data.push_back(row);
	} else if (index >= 0) {
		data[index] = row;
	} else {
		throw types::NewRowError(index);
	}
}

}  // dbTableKB


