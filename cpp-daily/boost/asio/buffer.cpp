#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <vector>
#include <iostream>

void Print(boost::system::error_code ec) {
    std::cout << "Hello, world" << std::endl;
}

void PrintNumber(boost::system::error_code ec,
                 boost::asio::deadline_timer* timer,
                 int* count) {
    if (*count < 10) {
        std::cout << *count << std::endl;
        ++(*count);

        timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));
//        timer->async_wait(boost::bind())
    }
}

void printArray(int array[], int size) {
    for (int index = 0; index < size; index++) {
        std::cout << array[index] << std::endl;
    }
}

void bindFunction() {
    int array[] = {0, 1, 2, 3, 4, 5, 6};
    int len = sizeof(array) / sizeof(array[0]);
    boost::bind(printArray, _1, _2)(array, len);

    std::cout << "======" << std::endl;

    boost::function<void(int[], int)> fn = boost::bind(printArray, _1, _2);
    fn(array, len);
}

class MyThread {
public:
    explicit MyThread(int thread_id): _thread_id(thread_id) {}

    virtual void run() {
        std::cout << "thread is running, id is " << _thread_id << std::endl;
    }

private:
    int _thread_id;
};

class CallWrapper {
public:
    typedef boost::function<void ()> CallbackFunc;

    CallWrapper() = default;

    static void run(const CallbackFunc &fn) {
        std::cout << __FUNCTION__ << __LINE__ << std::endl;
        fn();
    }
};

void testBindCallbackFunc() {
    MyThread thread(100);
    CallWrapper::run(boost::bind(&MyThread::run, thread));
}

void testAsioContext() {
    boost::asio::io_context io_context;
    boost::asio::deadline_timer timer(io_context, boost::posix_time::seconds(3));

    timer.async_wait(&Print);

    io_context.run();
}

void Foo(int a, int b) {
    std::cout << "make-pair: a=(" << a << "), b=(" << b << ")" << std::endl;
}

template<typename InputIt, typename UnaryFunction>
UnaryFunction my_for_each(InputIt first, InputIt last, UnaryFunction f) {
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}

template<typename A1>
class MyList1 {

};

template<typename A1, typename A2>
class MyList2 {

};

template <typename F, typename A1, typename A2>
my_bind_t<F, MyList2> my_bind(F f, A1 a1, A2 a2) {
    MyList2 list(a1, a2);
    return my_bind_t<F, MyList2>(f, list);
}

template <typename F, typename L>
class my_bind_t {
    F f_;
    L l_;

public:
    my_bind_t(F f, L l) : f_(f), l_(l){

    }

    template<typename A> operator()(A a) {

    }

    template<typename A1, typename A2> operator()(A1 a1, A2 a2) {

    }
};

void testBindImplement() {
    std::vector<int> vec;
    vec.reserve(10);
    for (int index = 0; index < 10; index++) {
        vec.push_back(index);
    }
    int b = 10;
    my_for_each(vec.begin(), vec.end(), boost::bind(Foo, _1, b));
}

int main() {
//    testAsioContext();

//    bindFunction();
//    testBindCallbackFunc();
    testBindImplement();
    return 0;
}