#ifndef gameServer_data_map_map_instance_h
#define gameServer_data_map_map_instance_h

#include <vector>

namespace data {
    namespace map {
        
        typedef int id;
        
        class map_instance {
        public:
            bool init(const id mapID) {
                return true;
            }
            void close() {
                
            }
            //TODO
        };
    }
}

#endif

