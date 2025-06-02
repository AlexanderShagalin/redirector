#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iocsetscoperelativecommand.h>
#include <memory>

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

class IScopeChangerMock: public IScopeChanger
{
public:
    MOCK_METHOD(void, Change, (IScopePtr));
};

using IScopeChangerMockPtr = std::shared_ptr<IScopeChangerMock>;

TEST(IocSetScopeRelativeCommand, SunnyDayTest)
{
    IScopeMockPtr currentScope = make_shared<IScopeMock>();
    IScopeMockPtr targetScope = make_shared<IScopeMock>();
    string targetScopeName = "TestScope";
    IScopeChangerMockPtr scopeChanger = make_shared<IScopeChangerMock>();

    IocSetScopeRelativeCommand cmd(currentScope, scopeChanger, targetScopeName);

    EXPECT_CALL(*currentScope, FindChild(targetScopeName))
        .WillOnce(
            Return(static_pointer_cast<IScope>(targetScope))
        );

    EXPECT_CALL(*scopeChanger, Change(static_pointer_cast<IScope>(targetScope)));

    EXPECT_NO_THROW(cmd.execute());
}

TEST(IocSetScopeRelativeCommand, WrongScopeNameTest)
{
    IScopeMockPtr currentScope = make_shared<IScopeMock>();
    IScopeMockPtr targetScope = make_shared<IScopeMock>();
    string targetScopeName = "TestScope";
    IScopeChangerMockPtr scopeChanger = make_shared<IScopeChangerMock>();

    IocSetScopeRelativeCommand cmd(currentScope, scopeChanger, targetScopeName);

    EXPECT_CALL(*currentScope, FindChild(targetScopeName))
        .WillOnce(Return(nullptr));

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
