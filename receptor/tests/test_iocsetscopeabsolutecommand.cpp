#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iocsetscopeabsolutecommand.h>
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

TEST(IocSetScopeAbsoluteCommand, ChangeScopeToRoot)
{
    IScopeMockPtr rootScope = make_shared<IScopeMock>();
    string targetScopeName = "/";
    IScopeChangerMockPtr scopeChanger = make_shared<IScopeChangerMock>();
    IocSetScopeAbsoluteCommand cmd(rootScope, scopeChanger, targetScopeName);

    EXPECT_CALL(*scopeChanger, Change(static_pointer_cast<IScope>(rootScope)));

    EXPECT_NO_THROW(cmd.execute());
}

TEST(IocSetScopeAbsoluteCommand, ChangeScopeToSubscope)
{
    IScopeMockPtr rootScope = make_shared<IScopeMock>();
    IScopeMockPtr scope1 = make_shared<IScopeMock>();
    IScopeMockPtr scope1_2 = make_shared<IScopeMock>();
    string targetScopeName = "/scope1/scope1_2";
    IScopeChangerMockPtr scopeChanger = make_shared<IScopeChangerMock>();
    IocSetScopeAbsoluteCommand cmd(rootScope, scopeChanger, targetScopeName);

    EXPECT_CALL(*rootScope, FindChild("scope1"))
        .WillOnce(
            Return(static_pointer_cast<IScope>(scope1))
        );

    EXPECT_CALL(*scope1, FindChild("scope1_2"))
        .WillOnce(
            Return(static_pointer_cast<IScope>(scope1_2))
        );

    EXPECT_CALL(*scopeChanger, Change(static_pointer_cast<IScope>(scope1_2)));

    EXPECT_NO_THROW(cmd.execute());
}

TEST(IocSetScopeAbsoluteCommand, WrongScopeName)
{
    IScopeMockPtr rootScope = make_shared<IScopeMock>();
    IScopeMockPtr scope1 = make_shared<IScopeMock>();
    string targetScopeName = "/scope1/scope1_3";
    IScopeChangerMockPtr scopeChanger = make_shared<IScopeChangerMock>();
    IocSetScopeAbsoluteCommand cmd(rootScope, scopeChanger, targetScopeName);

    EXPECT_CALL(*rootScope, FindChild("scope1"))
        .WillOnce(
            Return(static_pointer_cast<IScope>(scope1))
        );

    EXPECT_CALL(*scope1, FindChild("scope1_3"))
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
