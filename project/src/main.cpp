#include "utils.h"
#include "service.h"

enum menuStatus {
	PRINT_COMPANY = 1,
	MAIN_CUSTOMERS,
	INCOME_COMPANY,
	TEST = 8,
	EXIT = 10,
};

int menu(void);
void test();

int main () {
	try {
		std::pair<std::string, std::string> mCustomers;
		std::pair<std::string, double> income;
		while(true) {
			switch(menu()) {
				case PRINT_COMPANY:
					printCompany();
					break;
				case MAIN_CUSTOMERS:
					mCustomers = mainCustomers();
					std::cout << "Самый прибыльный клиент за указанный период - " << mCustomers.first
							  << " | " << mCustomers.second << std::endl;
					break;
				case INCOME_COMPANY:
					income = companyIncome();
					std::cout << "Доход " << income.first << " за указанный период времени составил "
							  << income.second << std::endl;
					break;
				case TEST:
					test();
					break;
				case EXIT:
					std::cout << "Завершение работы программы...\n";
					return 0;
				default:
					std::cout << "Выбрано несуществующее действие, повторите попытку\n"; 
				break;
			}
		}
	} catch (std::exception& er) {
		printf("error: %s\n", er.what());
		return -1;
	} catch (...) {
		printf("Упс! Что-то пошло не так... :(");
		return -1;
	}
    
}

int menu() {
    std::cout << "================= Калькулятор дохода ===================\n";
	std::cout << "\t1 - Просмотреть все компании\n";
	std::cout << "\t2 - Определить покупателя, который потратил больше всех денег\n";
	std::cout << "\t3 - Подсчитать доход компании от продажи товаров\n";
	std::cout << "\t8 - Провести тест работоспособности программы\n";
	std::cout << "\t10 - Выход\n";
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

void test() {
	puts("Тест успешный :)");
}