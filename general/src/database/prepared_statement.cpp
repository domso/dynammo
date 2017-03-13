#include <include/database/prepared_statement.h>

namespace database {
    namespace mysql {
        
        prepared_statement::prepared_statement() {

        }
        
        prepared_statement::~prepared_statement() {

        }
        
        bool prepared_statement::prepare(connection& conn) {
            bool result;

            if (conn.getNativeHandler() != nullptr) {
                statement_ = mysql_stmt_init(conn.getNativeHandler());
            } else {
                statement_ = nullptr;
            }

            isBinded_ = false;
            return statement_ != nullptr;
        }
        
        bool prepared_statement::execute() {
            return statement_ != nullptr && mysql_stmt_execute(statement_) == 0;
        }
        
        bool prepared_statement::fetchRow() {
            int result = mysql_stmt_fetch(statement_);
            return statement_ != nullptr && result == 0;
        }
        
        long unsigned int prepared_statement::getParamResultLength(const int paramID) {
            if (paramID < resultInfo_.size()) {
                return resultInfo_[paramID].length;
            }

            return 0;
        }
        
        std::string prepared_statement::getErrorMsg() {
            if (statement_ != nullptr) {
                std::string result(mysql_stmt_error(statement_));
                return result;
            }

            return "[FATAL] No statement found!";
        }
        
        bool prepared_statement::setQuery(const std::string query) {
            if (statement_ != nullptr && mysql_stmt_prepare(statement_, query.c_str(), query.length()) == 0) {
                params_.resize(mysql_stmt_param_count(statement_));
                results_.resize(mysql_stmt_field_count(statement_));
                paramInfo_.resize(params_.size());
                resultInfo_.resize(results_.size());
                return true;
            }

            return false;
        }
        
        bool prepared_statement::bindParam() {
            return statement_ != nullptr && params_.size() != 0 && mysql_stmt_bind_param(statement_, params_.data()) == 0;
        }
        
        bool prepared_statement::bindResult() {
            return statement_ != nullptr && results_.size() != 0 && mysql_stmt_bind_result(statement_, results_.data()) == 0;
        }
        
    }
}



