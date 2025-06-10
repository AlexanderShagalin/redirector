#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iocnewscopecommand.h>

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

TEST(IocNewScopeCommand, SunnyDayTest)
{
    IScopeMockPtr currentScope = make_shared<IScopeMock>();
    string scopeName = "testScope";
    IocNewScopeCommand cmd(currentScope, scopeName);

    EXPECT_CALL(*currentScope, FindChild(scopeName)).WillOnce(Return(nullptr));
    EXPECT_CALL(*currentScope, AddChild(_));

    EXPECT_NO_THROW(cmd.execute());
}

TEST(IocNewScopeCommand, InvalidArguments)
{
    string scopeName = "testScope";
    IocNewScopeCommand cmd(nullptr, scopeName);

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

TEST(IocNewScopeCommand, ScopeAlreadyExists)
{
    IScopeMockPtr currentScope = make_shared<IScopeMock>();
    IScopeMockPtr existingScope = make_shared<IScopeMock>();
    string scopeName = "testScope";
    IocNewScopeCommand cmd(currentScope, scopeName);

    EXPECT_CALL(*currentScope, FindChild(scopeName)).WillOnce(Return(existingScope));

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
