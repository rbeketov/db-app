#include "utils.h"
#include "exceptions.h"


namespace types {

std::string statusToStr(StatusEx status) {
        std::string result;
        switch (status) {
            case ADD:
                result = "сложения";
                break;
            case SUB:
                result = "вычитания";
                break;
            case MUL:
                result = "умножения";
                break;
            case DIV:
                result = "деления";
                break;
            case MORE:
                result = "сравнения (больше)";
                break;
            case LESS:
                result = "сравнения (меньше)";
                break;
            case EQUAL:
                result = "сравнение (равенство)";
                break;
        }
        return result;
    }

void SQLInt32::GetValue(int& val) { val = value; }
void SQLInt32::GetValue(double& val) { throw InvalidTypeValue(GetType()); }
void SQLInt32::GetValue(std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLInt32::GetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLInt32::GetValue(bool& val) { throw InvalidTypeValue(GetType()); }
void SQLInt32::SetValue(const int& val) { value = val; }
void SQLInt32::SetValue(const double& val) { throw InvalidTypeValue(GetType()); }
void SQLInt32::SetValue(const std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLInt32::SetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLInt32::SetValue(const bool& val) { throw InvalidTypeValue(GetType()); }
DBType SQLInt32::GetType() {return DBType::Int32;}
SQLValue* SQLInt32::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::Int32) {
        throw InvalidExecute(GetType(), rvalue->GetType());
    }
    int lv = this->value;
    int rv = 0;
    rvalue->GetValue(rv);
    switch (cond) {
        case ADD:
            return new SQLInt32(lv+rv);
        case SUB:
            return new SQLInt32(lv-rv);
        case MUL:
            return new SQLInt32(lv*rv);
        case DIV:
            return new SQLInt32(lv/rv);
        case EQUAL:
            return new SQLBool(lv == rv);
        case LESS:
            return new SQLBool(lv < rv);
        case MORE:
            return new SQLBool(lv > rv);
        default:
            throw InvalidExecute(GetType(), cond);
    }
}


void SQLDouble::GetValue(int& val) { throw InvalidTypeValue(GetType());  }
void SQLDouble::GetValue(double& val) { val = value; }
void SQLDouble::GetValue(std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLDouble::GetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLDouble::GetValue(bool& val) { throw InvalidTypeValue(GetType()); }
void SQLDouble::SetValue(const int& val) { throw InvalidTypeValue(GetType());  }
void SQLDouble::SetValue(const double& val) { value = val; }
void SQLDouble::SetValue(const std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLDouble::SetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLDouble::SetValue(const bool& val) { throw InvalidTypeValue(GetType()); }
DBType SQLDouble::GetType() {return DBType::Double;}
SQLValue* SQLDouble::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::Double) {
        throw InvalidExecute(GetType(), rvalue->GetType());
    }
    double lv = this->value;
    double rv = 0;
    rvalue->GetValue(rv);
    switch (cond) {
        case ADD:
            return new SQLDouble(lv+rv);
        case SUB:
            return new SQLDouble(lv-rv);
        case MUL:
            return new SQLDouble(lv*rv);
        case DIV:
            return new SQLDouble(lv/rv);
        case EQUAL:
            return new SQLBool(lv == rv);
        case LESS:
            return new SQLBool(lv < rv);
        case MORE:
            return new SQLBool(lv > rv);
        default:
            throw InvalidExecute(GetType(), cond);
    }
}


void SQLString::GetValue(int& val) { throw InvalidTypeValue(GetType());  }
void SQLString::GetValue(double& val) { throw InvalidTypeValue(GetType()); }
void SQLString::GetValue(std::string& val) { val = value; }
void SQLString::GetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLString::GetValue(bool& val) { throw InvalidTypeValue(GetType()); }
void SQLString::SetValue(const int& val) { throw InvalidTypeValue(GetType());  }
void SQLString::SetValue(const double& val) { throw InvalidTypeValue(GetType()); }
void SQLString::SetValue(const std::string& val) { value = val; }
void SQLString::SetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLString::SetValue(const bool& val) { throw InvalidTypeValue(GetType()); }
DBType SQLString::GetType() {return DBType::String;}
SQLValue* SQLString::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::String) {
        throw InvalidExecute(GetType(), rvalue->GetType());
    }
    std::string lv = this->value;
    std::string rv = "";
    bool comp;
    rvalue->GetValue(rv);
    switch (cond) {
        case ADD:
            return new SQLString(lv+rv);
        case EQUAL:
            comp = !(lv.compare(rv));
            return new SQLBool(comp);
        case LESS:
            comp = (lv.compare(rv) == 1);
            return new SQLBool(comp);
        case MORE:
            comp = (lv.compare(rv) == -1);
            return new SQLBool(comp);
        default:
            throw InvalidExecute(GetType(), cond);
    }
}


void SQLDate::GetValue(int& val) { throw InvalidTypeValue(GetType());  }
void SQLDate::GetValue(double& val) { throw InvalidTypeValue(GetType()); }
void SQLDate::GetValue(std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLDate::GetValue(dbms::DBDateKB& val) { val = value; }
void SQLDate::GetValue(bool& val) { throw InvalidTypeValue(GetType()); }
void SQLDate::SetValue(const int& val) { throw InvalidTypeValue(GetType());  }
void SQLDate::SetValue(const double& val) { throw InvalidTypeValue(GetType()); }
void SQLDate::SetValue(const std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLDate::SetValue(dbms::DBDateKB& val) { value = val; }
void SQLDate::SetValue(const bool& val) { throw InvalidTypeValue(GetType()); }
DBType SQLDate::GetType() {return DBType::Date;}
SQLValue* SQLDate::Execute(StatusEx cond, SQLValue* rvalue) {
    dbms::DBDateKB lv = this->value;
    int rv;
    dbms::DBDateKB daterv;
    dbms::DBDateKB minimum(1,1,1);
    switch (cond) {
        case ADD:
            if (rvalue->GetType() != DBType::Int32) {
                throw InvalidExecute(GetType(), rvalue->GetType());
            }
            rvalue->GetValue(rv);
            lv+=rv;
            return new SQLDate(lv);
        case SUB:
            if (rvalue->GetType() != DBType::Int32) {
                throw InvalidExecute(GetType(), rvalue->GetType());
            }
            rvalue->GetValue(rv);
            if (lv.dateToYlian()-rv < minimum.dateToYlian()) {
                throw InvalidExecute(lv, rv);
            }
            lv-=rv;
            return new SQLDate(lv);
        case EQUAL:
            if (rvalue->GetType() != DBType::Date) {
                throw InvalidExecute(GetType(), rvalue->GetType());
            }
            rvalue->GetValue(daterv);
            return new SQLBool(lv==daterv);
        case LESS:
            if (rvalue->GetType() != DBType::Date) {
                throw InvalidExecute(GetType(), rvalue->GetType());
            }
            rvalue->GetValue(daterv);
            return new SQLBool(lv<daterv);
        case MORE:
            if (rvalue->GetType() != DBType::Date) {
                throw InvalidExecute(GetType(), rvalue->GetType());
            }
            rvalue->GetValue(daterv);
            return new SQLBool(lv>daterv);
        default:
            throw InvalidExecute(GetType(), cond);
    }
}


void SQLBool::GetValue(int& val) { throw InvalidTypeValue(GetType());  }
void SQLBool::GetValue(double& val) { throw InvalidTypeValue(GetType()); }
void SQLBool::GetValue(std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLBool::GetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLBool::GetValue(bool& val) { val = value; }
void SQLBool::SetValue(const int& val) { throw InvalidTypeValue(GetType());  }
void SQLBool::SetValue(const double& val) { throw InvalidTypeValue(GetType()); }
void SQLBool::SetValue(const std::string& val) { throw InvalidTypeValue(GetType()); }
void SQLBool::SetValue(dbms::DBDateKB& val) { throw InvalidTypeValue(GetType()); }
void SQLBool::SetValue(const bool& val) { value = val; }
DBType SQLBool::GetType() {return DBType::Bool;}
SQLValue* SQLBool::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::Bool) {
        throw InvalidExecute(GetType(), rvalue->GetType());
    }
    bool lv = this->value;
    bool rv;
    rvalue->GetValue(rv);
    switch (cond) {
        case EQUAL:
            return new SQLDouble(lv==rv);
        default:
            throw InvalidExecute(GetType(), cond);
    }
}

}  // end of namespace types
