#include "utils.h"
#include "exceptions.h"

namespace dbms {

const int DBDateKB::arrDays[13] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

DBDateKB::DBDateKB(std::string date) {
	if (date.find('.') == std::string::npos || date.find_last_of('.') == std::string::npos) { throw types::DBTableError(*this); };
	day = stoi(date.substr(0, date.find('.')));
	month = stoi(date.substr(date.find('.') + 1, date.find_last_of('.') - date.find('.')));
	year = stoi(date.substr(date.find_last_of('.') + 1));
	if (day < 1 || day > GetDaysInMonth(month, year) || GetDaysInMonth(month, year) == -1) {
		throw types::DBTableError(*this);
	}

}
DBDateKB::DBDateKB(const DBDateKB& val) { 
	day = val.day;
	month = val.month;
	year = val.year;
}


DBDateKB::DBDateKB(int d, int m, int y) {
	if (d < 1 || d > GetDaysInMonth(m, y) || GetDaysInMonth(m, y) == -1) {
		throw types::DBTableError(*this);
		day = 1; month = 1; year = 1;
		
	} 
	day = d;
	month = m;
	year = y;
}
int DBDateKB::GetDay() { return day; }
int DBDateKB::GetMonth() { return month; }
int DBDateKB::GetYear() { return year; }
bool DBDateKB::IsLeapYear (int year) {
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ) {
		return true;
	}
	return false;
}
int DBDateKB::GetDaysInMonth(int month, int year) {
	if (month > 12 || month < 1 || year < 1) { return -1; }
	if (month == 2 && IsLeapYear(year)) {
		return arrDays[month] + 1;
	}
	return arrDays[month];
}
int DBDateKB::DaysInCurYear() {
	int daysCounter = 0;
	for (int i = 1; i <= month; ++i) {
		daysCounter += GetDaysInMonth(i, year);
	}
	daysCounter += day;
	return daysCounter;
};
bool DBDateKB::operator==(DBDateKB& date) {
	if (day == date.GetDay() &&
		month == date.GetMonth() &&
		year == date.GetYear()) {
		return true;
	}
	return false;
}
bool DBDateKB::operator!=(DBDateKB& date) {
	if (!(*this == date)) {
		return true;
	}
	return false;
}
bool DBDateKB::operator<(DBDateKB& date) {
	if (year < date.GetYear()) {
		return true;
	} else if (year == date.GetYear() && month < date.GetMonth()) {
		return true;
	} else if (month == date.GetMonth() && day < date.GetDay()) {
		return true;
	}
	return false;
}
bool DBDateKB::operator>(DBDateKB& date) {
	if (!(*this < date) && (*this != date)) {
		return true;
	}
	return false;
}
bool DBDateKB::operator<=(DBDateKB& date) {
	if ((*this < date) || (*this == date)) {
		return true;
	}
	return false;
}
bool DBDateKB::operator>=(DBDateKB& date) {
	if ((*this > date) || (*this == date)) {
		return true;
	}
	return false;
}
DBDateKB& DBDateKB::operator+=(int days) {
	days += day;
	while (true) {
		days -= GetDaysInMonth(month, year);
		if (days <= 0) {
			day = days + GetDaysInMonth(month, year);
			break;
		}
		++month;
		if (month == 13) {
			month = 1;
			++year;
		}
	}
	return *this;
}

DBDateKB& DBDateKB::operator=(DBDateKB& date) {
	day = date.day;
	month = date.month;
	year = date.year;
	return *this;
}

DBDateKB& DBDateKB::operator-=(int days) {
	DBDateKB minimum(1,1,1);
	if (this->dateToYlian()-days < minimum.dateToYlian()) {
        throw types::InvalidExecute(*this, days);
    }
	
	if (day - days > 0) {
		day -= days;
		return *this;
	}

	while (true) {
		--month;
		if (month == 0) {
			month = 12;
			--year;
		}
		days -= day;
		day = GetDaysInMonth(month,year);
		if (day - days > 0) {
			day -= days;
			break;
		}
	}
	return *this;
}
size_t DBDateKB::operator-(DBDateKB& date) {
	size_t cnt;
	cnt = std::max(this->dateToYlian(),date.dateToYlian()) - std::min(this->dateToYlian(),date.dateToYlian()) ;
	return cnt;
}

std::string DateToStr(DBDateKB& date) {
	return std::to_string(date.GetDay()) + "." + std::to_string(date.GetMonth()) + "." + std::to_string(date.GetYear());
}
std::ostream& operator<<(std::ostream& out, DBDateKB& date) {
	out << date.GetDay() << "." << date.GetMonth() << "." << date.GetYear();
	return out;
}

size_t DBDateKB::dateToYlian() {
	size_t ka = (14 - month)/12;
	size_t ky = year + 4800 - ka;
	size_t km = month + 12*ka - 3;
	size_t jdn = day + (153*km+2)/5 + 365*ky + ky/4 - (ky/100) + ky/400 - 32045;
	return jdn; 
}

}  // end of namespace dbms
