#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
#include <memory>

#include "macrocommand.h"

using ::testing::Throw;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::_;

class CommandMock: public ICommand
{
public:
    MOCK_METHOD(void, execute, ());
};

ACTION(ThrowException)
{
    throw std::exception("command exception");
}

TEST(MacroCommand, executed)
{
    auto mo = std::make_shared<CommandMock>();
    MacroCommand m({mo, mo, mo});

    EXPECT_CALL(*mo, execute()).Times(3);

    m.execute();
}

TEST(MacroCommand, exception)
{
    auto mo = std::make_shared<CommandMock>();
    MacroCommand m({mo, mo, mo});
    
    EXPECT_CALL(*mo, execute()).Times(1).WillRepeatedly(Throw(ThrowException()));

    EXPECT_ANY_THROW(m.execute());
}
