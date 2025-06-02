#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iocregistercommand.h>
#include <stdexcept>
#include <iresolvercontainer.h>

using namespace std;
using ::testing::Throw;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::_;
using ::testing::InSequence;

class IScopeMock: public IScope
{
public:
    MOCK_METHOD(string, getName, ());
    MOCK_METHOD(void, AddChild, (IScopePtr));
    MOCK_METHOD(IScopePtr, FindChild, (string));
    MOCK_METHOD(IResolverContainerPtr, getResolver, (string));
    MOCK_METHOD(void, setResolver, (string, IResolverContainerPtr));
};

using IScopeMockPtr = shared_ptr<IScopeMock>;

class IResolverContainerMock: public IResolverContainer
{
public:
};

using IResolverContainerMockPtr = std::shared_ptr<IResolverContainerMock>;
TEST(IocRegisterCommand, SunnyDayTest)
{
    IScopeMockPtr currentScope = make_shared<IScopeMock>();
    IResolverContainerMockPtr container = make_shared<IResolverContainerMock>();
    string dependecyName = "testDependecy";
    IocRegisterCommand cmd(currentScope, dependecyName, container);

    EXPECT_CALL(*currentScope, setResolver(dependecyName, static_pointer_cast<IResolverContainer>(container)));

    EXPECT_NO_THROW(cmd.execute());
}

TEST(IocRegisterCommand, InvalidArguments)
{
    IResolverContainerMockPtr container = make_shared<IResolverContainerMock>();
    string dependecyName = "testDependecy";
    IocRegisterCommand cmd(nullptr, dependecyName, container);

    try
    {
        cmd.execute();
        FAIL();
    }
    catch(std::exception exception)
    {
        SUCCEED();
    }
}

TEST(IocRegisterCommand, DependecyAlreadyExists)
{
    IScopeMockPtr currentScope = make_shared<IScopeMock>();
    IResolverContainerMockPtr container = make_shared<IResolverContainerMock>();
    string dependecyName = "testDependecy";
    IocRegisterCommand cmd(currentScope, dependecyName, container);

    EXPECT_CALL(*currentScope, setResolver(dependecyName, static_pointer_cast<IResolverContainer>(container)))
        .WillOnce(Throw(std::runtime_error("")));

    try
    {
        cmd.execute();
        FAIL();
    }
    catch(std::exception exception)
    {
        SUCCEED();
    }
}
