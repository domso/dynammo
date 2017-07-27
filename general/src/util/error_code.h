#ifndef general_util_error_code_h
#define general_util_error_code_h

namespace util {
    //______________________________________________________________________________________________________
    //
    // simple int-based error-code
    //______________________________________________________________________________________________________
    class error_code {
    public:
        error_code();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - sets 'code' as the new internal error-code
        // Parameter:
        // - code: new error-code
        //______________________________________________________________________________________________________
        void set(const int code);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - sets 'code' as the new internal error-code, if 'constraint' equals false
        // Parameter:
        // - code: new error-code
        // - constraint: set-constraint
        // Return:
        // - value of constraint
        //______________________________________________________________________________________________________
        bool setOnError(const int code, const bool constraint = false);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets and deletes the current error
        // Return:
        // - last error
        //______________________________________________________________________________________________________
        int load();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - checks if 'code' equals the internal error-code (see set())
        // Parameter:
        // - code: compare-code
        // Return:
        // - true  | on success
        // - false | on error
        //______________________________________________________________________________________________________
        bool operator==(const int code) const;
        //______________________________________________________________________________________________________
        //
        // Description:
        // - see operator==()
        //______________________________________________________________________________________________________
        bool check(const int code) const;
        //______________________________________________________________________________________________________
        //
        // Description:
        // - deletes the current error
        //______________________________________________________________________________________________________
        void clear();
    private:
        int m_code;
    };

}

#endif
