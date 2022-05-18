#include "utils.hpp"

#define DATABASE_PATH_STUDENTS     "../LibraryTxt/Students.txt"
#define DATABASE_PATH_BOOKS        "../LibraryTxt/Books.txt"
#define DATABASE_PATH_ABONEMENTS   "../LibraryTxt/Abonements.txt"

int menu(void);

int main () {
    dbms::DBTableTxt A;
    A.ReadDBTable(DATABASE_PATH_STUDENTS);
    types::SQLString B("IU5-22B");
    //
    A.AddRow(A.CreateRow());
    // A.SelfRows("Group", EQUAL, &B)->PrintTable(80);
    A.PrintTable(80);
    
    return 0;
}

int menu() {
    std::cout << "================= ПРАКТИКА ===================\n";
	std::cout << "\t1 - Привет\n";
	int choice;
	std::cout << "Введите номер действия для выбора\n";
	std::cin >> choice;
	while (std::cin.fail()) {
		std::cout << "Некорректный ввод, повторите попытку\n";	
		std::cin.clear();
		std::cin.ignore(10,'\n');
		std::cin >> choice;
	}
	return choice;
}