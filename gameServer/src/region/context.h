#ifndef gameServer_region_context_h
#define gameServer_region_context_h

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
              
        void add_user(const authentication::accountID_t accountID);
        const std::unordered_set<authentication::accountID_t>& get_users() const;
        void remove_user(const authentication::accountID_t accountID);     
               
        bool update();
        
        const std::vector<region::layer<uint32_t>>& get_layers() const;
        
        const region::layer<static_obj>& get_obj_layer(const int level) const;
        
        region::dynamic_obj* get_dynamic_obj(const uint32_t id);        
        const std::unordered_map<uint32_t, region::dynamic_obj>& get_dynamic_objs() const;
        
        std::vector<region::static_obj> get_static_objs() const;
               
        
        uint32_t insert_new_dynamic_object(const region::dynamic_obj& obj);        
        void remove_dynamic_object(const uint32_t id);
        
        void insert_new_static_object(const region::static_obj& obj);
    private:        
        
        void load_layer(region::layer<uint32_t>& layer, const std::string& filename);
        
        void load();
        void save();
        
        uint32_t m_dynIdCounter = 0;
        
        uint32_t m_id;            
        std::unordered_set<authentication::accountID_t> m_activeUsers;
        
        std::vector<region::layer<uint32_t>> m_layers;    
        
        std::vector<region::layer<region::static_obj>> m_staticObjLayers;
        std::unordered_map<uint32_t, region::dynamic_obj> m_dynamicObjects;
    };     
}

#endif
