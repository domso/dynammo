#ifndef gameServer_region_context_h
#define gameServer_region_context_h

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "src/util/lock_ref.h"
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
        const std::vector<uint32_t>& affected_users();
        const std::vector<uint32_t>& changed_dynamic_objects();
        const std::vector<uint32_t>& changed_reliable_dynamic_objects();
        
        const std::unordered_map<uint32_t, region::dynamic_obj>& all_dynamic_objects();
        
        const std::vector<uint32_t>& changed_static_objects();
        const std::unordered_map<uint32_t, region::static_obj>& all_static_objects();
        
        const std::vector<uint32_t>& changed_layers();
        const std::vector<region::layer<uint32_t>>& all_layers();
        
        void commit();
        uint32_t get_id();
    private:       
        void set_all_user_as_affected();
        
        
        void load_layer(region::layer<uint32_t>& layer, const std::string& filename);
        void set_static_obj(const uint32_t x, const uint32_t y, const uint32_t z, const uint32_t type);
        region::static_obj& get_static_obj(const uint32_t x, const uint32_t y, const uint32_t z);
        void load();
        void save();

        uint32_t m_id;            
        
        std::unordered_set<uint32_t> m_activeUsers;
        std::vector<uint32_t> m_affectedUsers;
        
        std::vector<uint32_t> m_changedDynamicObjects;
        std::vector<uint32_t> m_changedDynamicObjectsReliable;
        std::unordered_map<uint32_t, region::dynamic_obj> m_dynamicObjects;
        
        std::vector<uint32_t> m_changedStaticObjects;
        std::unordered_map<uint32_t, region::static_obj> m_staticObjects;
        
        std::vector<uint32_t> m_changedLayers;
        std::vector<region::layer<uint32_t>> m_layers;  
    };     
}

#endif
