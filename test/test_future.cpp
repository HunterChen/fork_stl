#include <bind.hpp>
#include <thread.hpp>
#include <future.hpp>
#include <gtest/gtest.h>

TEST(TestFuture, test__assoc_state){
    xusd::__assoc_state<int> state;
    state.set_value(10);
    EXPECT_TRUE(state.__has_value());
    EXPECT_TRUE(state.__is_ready());
    EXPECT_EQ(state.move(), 10);

}
TEST(TestFuture, testFuture){
    auto s = new xusd::__assoc_state<int>();
    xusd::future<int> f1{std::shared_ptr<xusd::__assoc_state<int> >(s)};
    s->set_value(11);
    EXPECT_TRUE(f1.valid());
    EXPECT_EQ(f1.get(), 11);
    EXPECT_FALSE(f1.valid());

    /////////////////////////////////////////////////////
    auto s2 = new xusd::__assoc_state<int&>();
    xusd::future<int&> f2{std::shared_ptr<xusd::__assoc_state<int&> >(s2)};

    int value = 13;
    s2->set_value(value);
    EXPECT_TRUE(f2.valid());
    int& ret = f2.get();
    EXPECT_EQ(ret, 13);
    EXPECT_FALSE(f2.valid());
    EXPECT_EQ(&ret, &value);

    /////////////////////////////////////////////////////
    auto s3 = new xusd::__assoc_state_base();
    xusd::future<void> f3{std::shared_ptr<xusd::__assoc_state_base >(s3)};
    EXPECT_TRUE(f3.valid())<<"xxx";
    s3->set_value();

    f3.get();
    EXPECT_FALSE(f3.valid());

    /////////////////////////////////////////////////////
    auto s4 = new xusd::__assoc_state<int>();
    xusd::future<int> f4{std::shared_ptr<xusd::__assoc_state<int> >(s4)};
    s4->set_value(11);
    EXPECT_TRUE(f4.valid());
    xusd::shared_future<int> fs4(std::move(f4));
    EXPECT_FALSE(f4.valid());
    EXPECT_TRUE(fs4.valid());
    EXPECT_EQ(fs4.get(), 11);
    EXPECT_TRUE(fs4.valid());
}

TEST(TestPromise, testPromise){
    xusd::promise<int> p1;
    p1.set_value(3);
    xusd::future<int> f1 = p1.get_future();
    EXPECT_TRUE(f1.valid());
    EXPECT_EQ(f1.get(), 3);

    auto promise = xusd::promise<std::string>();
    auto producer = xusd::thread([&] { promise.set_value("Hello World"); });
    auto future = promise.get_future();

    EXPECT_EQ(future.get(), "Hello World");
    producer.join();


    auto pro1 = xusd::promise<int>();
    auto fut1 = pro1.get_future();
    xusd::thread([](xusd::promise<int>&& p){ p.set_value_at_thread_exit(7);}, std::move(pro1)).detach();

    EXPECT_EQ(fut1.get(), 7);
}

int testAsyncXXX(int a, int b){
   return a+b; 
}
TEST(TestAsync, testAsync){

    auto f = xusd::async(testAsyncXXX, 1, 2);
    EXPECT_EQ(f.get(), 3);
}

TEST(TestPackagedTask, testPackagedTask){
    xusd::packaged_task<int(int,int)> f([](int x, int y){ return x*y; });
    auto fut = f.get_future();
    xusd::thread(std::move(f), 3, 7).detach();
    EXPECT_EQ(3*7, fut.get());
}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
