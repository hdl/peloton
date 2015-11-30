#include "gtest/gtest.h"

#include "logging/logging_tests_util.h"
#include "backend/common/logger.h"

#include <fstream>

namespace peloton {
namespace test {

//===--------------------------------------------------------------------===//
// Logging Test 
//===--------------------------------------------------------------------===//

std::string aries_log_file_name = "/tmp/aries.log";

/**
 * @brief writing a simple log with multiple threads
 */
TEST(AriesLoggingTest, writing_logfile) {

  std::ifstream log_file(aries_log_file_name.c_str());

  // Reset the log file if exists
  if( log_file.good() ){
    EXPECT_TRUE(std::remove(aries_log_file_name.c_str()) == 0 );
  }
  log_file.close();

  // Prepare a simple log file
  EXPECT_TRUE(LoggingTestsUtil::PrepareLogFile(LOGGING_TYPE_ARIES, aries_log_file_name));

  // Reset data
  LoggingTestsUtil::ResetSystem();
}

/**
 * @brief recovery test
 */
TEST(AriesLoggingTest, recovery) {

  std::ifstream log_file(aries_log_file_name.c_str());

  // Reset the log file if exists
  EXPECT_TRUE(log_file.good());
  log_file.close();

  // Do recovery
  LoggingTestsUtil::CheckRecovery(LOGGING_TYPE_ARIES, aries_log_file_name);
}

}  // End test namespace
}  // End peloton namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  // Setup testing configuration
  peloton::test::LoggingTestsUtil::ParseArguments(argc, argv);

  return RUN_ALL_TESTS();
}