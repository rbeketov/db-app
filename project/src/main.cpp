#include "utils.h"
#include "mcustomers.h"
#include "income.h"

enum menuStatus
{
	TEST = 1,
	EXIT = 10,
};

int menu(void);
void test();

int main()
{
	try
	{
		while (true)
		{
			switch (menu())
			{
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
	}
	catch (std::exception &er)
	{
		printf("error: %s\n", er.what());
		// printf("Аварийный выход, внесённые ранее изменения сохранены...");
		return -1;
	}
	catch (...)
	{
		printf("Упс! Что-то пошло не так... :(");
		// printf("Аварийный выход, внесённые ранее изменения сохранены...");
		return -1;
	}
}

int menu()
{
	std::cout << "================= ПРАКТИКА ===================\n";
	std::cout << "\t1 - Тест\n";
	std::cout << "\t10 - Выход\n";
	int choice;
	std::cout << "Введите номер действия для выбора\n";
	std::cin >> choice;
	while (std::cin.fail())
	{
		std::cout << "Некорректный ввод, повторите попытку\n";
		std::cin.clear();
		std::cin.ignore(10, '\n');
		std::cin >> choice;
	}
	return choice;
}

void test()
{
	types::SQLDate start("05.04.2017");
	types::SQLDate end("05.12.2017");

	// num 7
	dbms::DBTableTxt *orders = dateRange(DATABASE_ORDERS, start, end);
	std::unordered_map<std::string, std::vector<types::SQLValue *>> cOrders = customersOrders(orders);
	std::unordered_map<std::string, double> cCash = customersCash(cOrders);
	std::string mainCustomer = findMainCustomers(cCash);
	std::pair<std::string, std::string> resInfo = customerInfo(mainCustomer);
	std::cout << resInfo.first << " " << resInfo.second << std::endl;

	// num 4
	std::cout << companyIncome("Richter_Supermarkt", start, end) << std::endl;
}