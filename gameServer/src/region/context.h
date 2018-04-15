#ifndef gameServer_region_context_h
#define gameServer_region_context_h

#include <unordered_set>
#include "src/util/lock_ref.h"
#include "src/authentication/types.h"
#include "src/region/static_obj.h"
#include "src/region/dynamic_obj.h"
#include "src/region/layer.h"
#include "src/types/game_events.h"

namespace region {
    class context : public util::locked_ref_item {
    public:
        context(const uint64_t id);
        context(const context&) = delete;
        ~context();
              
        void add_user(const authentication::accountID_t accountID);
        const std::unordered_set<authentication::accountID_t>& get_users() const;
        void remove_user(const authentication::accountID_t accountID);     
        
        region::dynamic_obj* action_for_dynamic_object(const uint32_t id, const types::game_events action, const authentication::accountID_t accountID);
        bool update();
        
        region::layer<uint32_t>& get_layer();
        std::vector<region::dynamic_obj>& get_dynamic_obj();
        std::vector<region::static_obj>& get_static_obj();
    private:        
        void load();
        void save();
        
        void insert_new_dynamic_object(const region::dynamic_obj& obj);        
        void insert_new_static_object(const region::static_obj& obj);
        
        uint64_t m_id;            
        std::unordered_set<authentication::accountID_t> m_activeUsers;
        
        region::layer<uint32_t> m_testLayer;
//         std::unordered_map<uint32_t, region::dynamic_obj> m_dynamicObjects;
        
        std::vector<region::dynamic_obj> m_dynamicObjects2;
        
        std::vector<region::static_obj> m_staticObjects;
    };     
}

#endif
