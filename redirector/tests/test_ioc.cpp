#include <ioc.h>
#include <icommand.h>
#include <resolvercontainer.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <thread>

using namespace std;
using ::testing::Throw;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::_;
using ::testing::InSequence;

TEST(IoC, RegisterDependecy)
{
    IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "/")->execute();
    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "ScopeName",
        make_container(std::function<string()>( [](){ return string("root"); } ))
        )->execute();

    EXPECT_EQ(
        string("root"),
        IoC::Resolve<string>("ScopeName")
    );
}

TEST(IoC, CreateScope)
{
    try {
        IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "/")->execute();
        IoC::Resolve<ICommandPtr>("IoC.Scope.New", "subscope_01")->execute();
        IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "subscope_01")->execute();

        IoC::Resolve<ICommandPtr>(
            "IoC.Register",
            "ScopeName",
            make_container(std::function<string()>( [](){ return string("subscope_01"); } ))
            )->execute();
        
        EXPECT_EQ(
            string("subscope_01"),
            IoC::Resolve<string>("ScopeName")
        );
    }
    catch(std::exception exception)
    {
        string what = exception.what();
        FAIL() << what;
    }
}

TEST(IoC, InheritedDependies)
{
    IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "/")->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "RootScopeName",
        make_container(std::function<string()>( [](){ return string("root"); } ))
        )->execute();

    IoC::Resolve<ICommandPtr>("IoC.Scope.New", "subscope_02")->execute();
    IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "subscope_02")->execute();

    EXPECT_EQ(
        string("root"),
        IoC::Resolve<string>("RootScopeName")
    );
}

TEST(IoC, MultithreadedTest)
{
    IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "/")->execute();
    IoC::Resolve<ICommandPtr>("IoC.Scope.New", "subscope_03")->execute();
    IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "subscope_03")->execute();

    IoC::Resolve<ICommandPtr>(
        "IoC.Register",
        "ScopeName",
        make_container(std::function<string()>( [](){ return string("subscope_03"); } ))
    )->execute();

    std::thread t( []() {
        IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "/")->execute();
        IoC::Resolve<ICommandPtr>("IoC.Scope.New", "subscope_03_thread")->execute();
        IoC::Resolve<ICommandPtr>("IoC.Scope.Current.Set", "subscope_03_thread")->execute();
    
        IoC::Resolve<ICommandPtr>(
            "IoC.Register",
            "ScopeName",
            make_container(std::function<string()>( [](){ return string("subscope_03_thread"); } ))
        )->execute();
    
    } );
    t.join();

    EXPECT_EQ(
        string("subscope_03"),
        IoC::Resolve<string>("ScopeName")
    );    
}
