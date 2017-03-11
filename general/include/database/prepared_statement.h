#ifndef general_database_mysql_prepared_statements_h
#define general_database_mysql_prepared_statements_h

#include <vector>
#include <mysql.h>
#include <include/database/connection.h>

namespace database {
    namespace mysql {
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // abstract basic query
        class prepared_statement {
        public:
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            prepared_statement();
            ~prepared_statement();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - connects the query to an open connection
            // Parameter:
            // - conn: a valid connection
            // Return:
            // - true  | on success
            // - false | on any error
            bool prepare(connection& conn);
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - executes the query
            // - the query and all necessary parameters/results have to be set first
            // Return:
            // - true  | on success
            // - false | on any error
            bool execute();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - stores the next result-row into the result-columns
            // Return:
            // - true  | on success
            // - false | on any error
            bool fetchRow();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - returns the length of valid bytes in the result-column specified by the paramID
            // Parameter:
            // - paramID: the ascending column number (starting with 0)
            // Return:
            // - number of valid bytes in the result-column
            long unsigned int getParamResultLength(const int paramID);
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - returns the current error message from the internal MySQL-Connector (--> mysql_stmt_error())
            // Return:
            // - new String containing the error message
            std::string getErrorMsg();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        protected:
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // holds the information for a single columns inside a single row from a load query
            struct result_param_t {
                long unsigned int length;
                my_bool isNULL = false;
                my_bool useError = false;
            };
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - sets n paramT-instances as a single parameter for store queries
            // Parameters:
            // - paramID: the ascending parameter (starting with 0)
            // - mysqlType: the SQL-Type of the column
            // - input: pointer to a valid instance of paramT
            // - n: number of paramT-instances at input
            // - isNULL: see MYSQL_BIND.is_null
            // - isUnsigned: see MYSQL_BIND.is_unsigned
            // - useError: see MYSQL_BIND.error
            // Return:
            // - true  | on success
            // - false | on any error
            template <typename paramT>
            bool setParam(const unsigned int paramID, enum enum_field_types mysqlType, const paramT* input, const int n = 1,  my_bool isNULL = false, my_bool isUnsigned = false, my_bool useError = false) {
                isBinded_ = false;

                if (paramID < params_.size()) {
                    paramInfo_[paramID].isNULL = isNULL;
                    paramInfo_[paramID].useError = useError;

                    params_[paramID].buffer_type = mysqlType;
                    params_[paramID].buffer = (char*) input;
                    params_[paramID].buffer_length = n * sizeof(paramT);
                    params_[paramID].length = &(paramInfo_[paramID].length);
                    params_[paramID].is_null = &(paramInfo_[paramID].isNULL);
                    params_[paramID].is_unsigned = isUnsigned;
                    params_[paramID].error = &(paramInfo_[paramID].useError);

                    return true;
                }

                return false;
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - sets n paramT-instances as a single result-column buffer for load queries
            // Parameters:
            // - paramID: the ascending column number (starting with 0)
            // - mysqlType: the SQL-Type of the column
            // - input: pointer to a valid instance of paramT
            // - n: number of paramT-instances at input
            // - isNULL: see MYSQL_BIND.is_null
            // - isUnsigned: see MYSQL_BIND.is_unsigned
            // - useError: see MYSQL_BIND.error
            // Return:
            // - true  | on success
            // - false | on any error
            template <typename paramT>
            bool setResult(const unsigned int paramID, enum enum_field_types mysqlType, const paramT* input, const int n = 1,  my_bool isNULL = false, my_bool isUnsigned = false, my_bool useError = false) {
                isBinded_ = false;

                if (paramID < results_.size()) {
                    resultInfo_[paramID].isNULL = isNULL;
                    resultInfo_[paramID].useError = useError;

                    results_[paramID].buffer_type = mysqlType;
                    results_[paramID].buffer = (char*) input;
                    results_[paramID].buffer_length = n * sizeof(paramT);
                    results_[paramID].length = &(resultInfo_[paramID].length);
                    results_[paramID].is_null = &(resultInfo_[paramID].isNULL);
                    results_[paramID].is_unsigned = isUnsigned;
                    results_[paramID].error = &(resultInfo_[paramID].useError);

                    return true;
                }

                return false;
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - sets a new query
            // Parameter:
            // - query: a valid MySQL-query-string
            // Return:
            // - true  | on success
            // - false | on any error
            bool setQuery(const std::string query);
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - binds the parameters to the prepared statement
            // Return:
            // - true  | on success
            // - false | on any error
            bool bindParam();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Description:
            // - binds the result-columns to the prepared statement
            // Return:
            // - true  | on success
            // - false | on any error
            bool bindResult();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // is the current query binded?
            bool isBinded_;
            // internal representation of the prepared statement
            MYSQL_STMT* statement_;
            // container holding all parameters/result-columns
            std::vector<MYSQL_BIND> params_;

            std::vector<MYSQL_BIND> results_;

            // container holding all parameter-informations
            std::vector<result_param_t> paramInfo_;
            // container holding all result-informations
            std::vector<result_param_t> resultInfo_;
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        };
    }
}

#endif



