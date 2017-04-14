
#include <thread>
#include <mutex>
#include <condition_variable>


void func(std::condition_variable* con) {
	for (int i = 0; i < 100000000; i++) {

	}
	con->notify_all();	
}

int main() {

	std::mutex m;
	std::unique_lock<std::mutex> ul(m);
	std::condition_variable con;

	std::thread t1(func, &con);

	con.wait(ul);

	return 0;
}
