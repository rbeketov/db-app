#pragma once // NOLINT

#include <exception>
#include <string>

#include "utils.h"


namespace types {
class ValueException: public std::exception {
 public:
  explicit ValueException(const std::string& msg)
      : m_msg(msg) {}

  const char* what() const noexcept override {
    return m_msg.c_str();
  }

 private:
  std::string m_msg;
};

class InvalidTypeValue: public ValueException {
 public:
  InvalidTypeValue(DBType type)
      : ValueException("Получен некорректный тип при работе с " + dbms::DBTypeToString(type)) {}
};

class InvalidExecute: public ValueException {
 public:
  InvalidExecute(DBType type, StatusEx status)
      : ValueException("Нельзя выполнить операцию " + types::statusToStr(status) + " c " + dbms::DBTypeToString(type)) {}
  InvalidExecute(DBType orig, DBType accept)
      : ValueException("Подан несовместимый тип " + dbms::DBTypeToString(accept) + " для типа " + dbms::DBTypeToString(orig)) {}
  InvalidExecute(dbms::DBDateKB date, int value)
      : ValueException("Нельзя из " + dbms::DateToStr(date) + " вычесть " + std::to_string(value)) {}
};

class СonversionError: public ValueException {
  public:
   СonversionError()
       : ValueException("Получен некорректный параметр при попытке преобразования") {}
};

class DBTableError: public ValueException {
  public:
    DBTableError(std::string tableName, int codeError)
       : ValueException("Некорректная таблица " + tableName) {}
    DBTableError(std::string fileName)
       : ValueException("Ошибка открытия файла " + fileName ) {}
    DBTableError(std::string fileName, std::string tableName)
       : ValueException("Имя таблицы " + tableName + " не совпадает с название файла по адресу " + fileName ) {}
    DBTableError(dbms::Row& row, std::string columnName)
       : ValueException("Столбца " + columnName + " не существует" ) {}
    DBTableError(int sizeCol)
       : ValueException("Ширина столбца " + std::to_string(sizeCol) + " больше ширины экрана\nТаблицу невозможно распечатать") {}
    DBTableError(dbms::DBDateKB& date)
       : ValueException("Введена некорректная дата") {}
    
};

class NewRowError: public ValueException {
  public:
    NewRowError()
       : ValueException("Некорректно введены данные, несовпадение типов") {}
    NewRowError(int index)
       : ValueException("Введён некорректный индекс при попытке добавления новой строки (" + std::to_string(index) + ")") {}
};


}  // namespace types