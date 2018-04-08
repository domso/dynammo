#include "src/user/info.h"

user::info::info() {
    publicKey.load("../keys/public.pem");
}

user::info::~info() {

}
