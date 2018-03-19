#include "src/util/signals.h"
#include <signal.h>
#include <unistd.h>
void util::signals::default_handler(const int sig) {
}

void util::signals::wait_for_sigint() {
    struct sigaction action;
    action.sa_handler = &util::signals::default_handler;
    
    sigaction(SIGINT, &action, nullptr);
    pause();
}

