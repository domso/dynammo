#ifndef gameServer_region_context_h
#define gameServer_region_context_h

#include <set>
#include <unordered_set>
#include <unordered_map>

#include "src/util/lock_ref.h"
#include "src/authentication/types.h"
#include "src/region/static_obj.h"
#include "src/region/dynamic_obj.h"
#include "src/region/layer.h"
#include "src/types/game_events.h"

namespace region {
    class context : public util::locked_ref_item {
    public:
        context(const uint32_t id);
        context(const context&) = delete;
        ~context();
        bool update();
        bool action(const uint32_t accountID, const uint32_t sessionID, const types::game_events event);        
        void remove_user(const uint32_t sessionID);
        
        const std::unordered_set<uint32_t>& all_users();
        const std::unordered_set<uint32_t>& affected_users();
        const std::unordered_set<uint32_t>& changed_dynamic_objects();
        const std::unordered_map<uint32_t, region::dynamic_obj>& all_dynamic_objects();
        
        const std::unordered_set<uint32_t>& changed_static_objects();
        const std::unordered_map<uint32_t, region::static_obj>& all_static_objects();
        
        const std::set<uint32_t>& changed_layers();
        const std::vector<region::layer<uint32_t>>& all_layers();
        
        void commit();
    private:       
        void load_layer(region::layer<uint32_t>& layer, const std::string& filename);
        
        void load();
        void save();

        uint32_t m_id;            
        
        std::unordered_set<uint32_t> m_activeUsers;
        std::unordered_set<uint32_t> m_affectedUsers;
        std::unordered_set<uint32_t> m_changedDynamicObjects;
        std::unordered_map<uint32_t, region::dynamic_obj> m_dynamicObjects;
        
        std::unordered_set<uint32_t> m_changedStaticObjects;
        std::unordered_map<uint32_t, region::static_obj> m_staticObjects;
        
        std::set<uint32_t> m_changedLayers;
        std::vector<region::layer<uint32_t>> m_layers;  
    };     
}

#endif
