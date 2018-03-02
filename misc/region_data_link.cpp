#include "src/data/region_data_link.h"

connector::tcp_receiver::target data::region_data_link::configure(data::region_data_link* obj, connector::controller* ctrl) {
    connector::tcp_receiver::target result;
    result.set<region::layer<uint32_t>>(&obj->region0[0]);
    return result;
}

bool data::region_data_link::complete(data::region_data_link* obj, connector::controller* ctrl) {
    obj->currentState.set(message::types::states::recv);
    return true;
}


