
#include "src/connector/context.h"
#include "src/connector/worker.h"

int main() {
    connector::context c1;
    connector::worker w1(c1);
        
    while (true) {}
    
    return 0;
}
