#include "utils.hpp"

namespace types {

SQLValue* SQLInt32::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::Int32) {
        throw;
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
            throw;
    }
}

SQLValue* SQLDouble::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::Double) {
        throw;
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
            throw;
    }
}

SQLValue* SQLString::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::String) {
        throw;
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
            // std::cout << comp << std::endl;
            return new SQLBool(comp);
        case LESS:
            comp = (lv.compare(rv) == 1);
            return new SQLBool(comp);
        case MORE:
            comp = (lv.compare(rv) == -1);
            return new SQLBool(comp);
        default:
            throw;
    }
}

SQLValue* SQLDate::Execute(StatusEx cond, SQLValue* rvalue) {
    dbms::DBDateKB lv = this->value;
    int rv;
    dbms::DBDateKB daterv;
    dbms::DBDateKB minimum(1,1,1);
    switch (cond) {
        case ADD:
            if (rvalue->GetType() != DBType::Int32) {
                throw;
            }
            rvalue->GetValue(rv);
            lv+=rv;
            return new SQLDate(lv);
        case SUB:
            if (rvalue->GetType() != DBType::Int32) {
                throw;
            }
            rvalue->GetValue(rv);
            if (lv.dateToYlian()-rv < minimum.dateToYlian()) {
                throw;
            }
            lv-=rv;
            return new SQLDate(lv);
        case EQUAL:
            if (rvalue->GetType() != DBType::Date) {
                throw;
            }
            rvalue->GetValue(daterv);
            return new SQLBool(lv==daterv);
        case LESS:
            if (rvalue->GetType() != DBType::Date) {
                throw;
            }
            rvalue->GetValue(daterv);
            return new SQLBool(lv<daterv);
        case MORE:
            if (rvalue->GetType() != DBType::Date) {
                throw;
            }
            rvalue->GetValue(daterv);
            return new SQLBool(lv>daterv);
        default:
            throw;
    }
}

SQLValue* SQLBool::Execute(StatusEx cond, SQLValue* rvalue) {
    if (rvalue->GetType() != DBType::Bool) {
        throw;
    }
    bool lv = this->value;
    bool rv;
    rvalue->GetValue(rv);
    switch (cond) {
        case EQUAL:
            return new SQLDouble(lv==rv);
        default:
            throw;
    }
}

}  // end of namespace types
