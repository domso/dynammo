#ifndef gameServer_region_context_h
#define gameServer_region_context_h

#include <unordered_set>
#include "src/util/lock_ref.h"
#include "src/region/data.h"
#include "src/authentication/types.h"
#include "src/user/controller.h"

namespace region {
    class context : public util::locked_ref_item {
    public:
        context(const uint64_t id, user::controller& userCtrl);
        context(const context&) = delete;
        ~context();
              
        void add_user(const authentication::accountID_t accountID);
        void remove_user(const authentication::accountID_t accountID);
        
    private:        
        void load();
        void save();
        void update();
        
        region::data m_regionData;        
        uint64_t m_id;
        user::controller& m_userCtrl;        
        
        std::unordered_set<authentication::accountID_t> m_activeUsers;
    };     
}

#endif
