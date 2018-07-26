#include "src/user/info.h"


user::info::info(const authentication::accountID_t accountID) {
    publicKey.load("keys/" + std::to_string(accountID) + ".pem");
}

user::info::~info() {

}
