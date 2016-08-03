//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// delete_test.cpp
//
// Identification: test/executor/delete_test.cpp
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//


#include <cstdio>

#include "common/harness.h"
#include "common/logger.h"
#include "common/statement.h"
#include "catalog/bootstrapper.h"
#include "catalog/catalog.h"
#include "planner/create_plan.h"
#include "planner/insert_plan.h"
#include "planner/delete_plan.h"
#include "planner/update_plan.h"
#include "executor/create_executor.h"
#include "executor/insert_executor.h"
#include "executor/delete_executor.h"
#include "executor/update_executor.h"
#include "executor/plan_executor.h"
#include "parser/parser.h"
#include "optimizer/simple_optimizer.h"


#include "gtest/gtest.h"

namespace peloton {
namespace test {

//===--------------------------------------------------------------------===//
// Catalog Tests
//===--------------------------------------------------------------------===//

class UpdateTests : public PelotonTest {};

TEST_F(UpdateTests, Updating) {

  LOG_INFO("Bootstrapping...");
  catalog::Bootstrapper::bootstrap();
  catalog::Bootstrapper::global_catalog->CreateDatabase(DEFAULT_DB_NAME);
  LOG_INFO("Bootstrapping completed!");


  auto &txn_manager = concurrency::TransactionManagerFactory::GetInstance();

  txn_manager.BeginTransaction();
  LOG_INFO("Creating table");
  LOG_INFO("Query: CREATE TABLE department_table(dept_id INT PRIMARY KEY, dept_name TEXT);");
  std::unique_ptr<Statement> statement;
  statement.reset(new Statement("CREATE","CREATE TABLE department_table(dept_id INT PRIMARY KEY, dept_name TEXT);"));
  auto& peloton_parser = parser::Parser::GetInstance();
  LOG_INFO("Building parse tree...");
  auto create_stmt = peloton_parser.BuildParseTree("CREATE TABLE department_table(dept_id INT PRIMARY KEY, dept_name TEXT);");
  LOG_INFO("Building parse tree completed!");
  LOG_INFO("Building plan tree...");
  statement->SetPlanTree(optimizer::SimpleOptimizer::BuildPelotonPlanTree(create_stmt));
  LOG_INFO("Building plan tree completed!");
  std::vector<Value> params;
  std::vector<ResultType> result;
  bridge::PlanExecutor::PrintPlan(statement->GetPlanTree().get(), "Plan");
  LOG_INFO("Executing plan...");
  bridge::peloton_status status = bridge::PlanExecutor::ExecutePlan(statement->GetPlanTree().get(), params, result);
  LOG_INFO("Statement executed. Result: %d", status.m_result);
  LOG_INFO("Table Created");
  txn_manager.CommitTransaction();

  // Inserting a tuple end-to-end
  txn_manager.BeginTransaction();
  LOG_INFO("Inserting a tuple...");
  LOG_INFO("Query: INSERT INTO department_table(dept_id,dept_name) VALUES (1,'hello_1');");
  statement.reset(new Statement("INSERT", "INSERT INTO department_table(dept_id,dept_name) VALUES (1,'hello_1');"));
  LOG_INFO("Building parse tree...");
  auto insert_stmt = peloton_parser.BuildParseTree("INSERT INTO department_table(dept_id,dept_name) VALUES (1,'hello_1');");
  LOG_INFO("Building parse tree completed!");
  LOG_INFO("Building plan tree...");
  statement->SetPlanTree(optimizer::SimpleOptimizer::BuildPelotonPlanTree(insert_stmt));
  LOG_INFO("Building plan tree completed!");
  bridge::PlanExecutor::PrintPlan(statement->GetPlanTree().get(), "Plan");
  LOG_INFO("Executing plan...");
  status = bridge::PlanExecutor::ExecutePlan(statement->GetPlanTree().get(), params, result);
  LOG_INFO("Statement executed. Result: %d", status.m_result);
  LOG_INFO("Tuple inserted!");
  txn_manager.CommitTransaction();

  // Now Updating end-to-end
  txn_manager.BeginTransaction();
  LOG_INFO("Updating a tuple...");
  LOG_INFO("Query: UPDATE department_table SET dept_name = 'CS' WHERE dept_id = 1");
  statement.reset(new Statement("UPDATE", "UPDATE department_table SET dept_name = 'CS' WHERE dept_id = 1"));
  LOG_INFO("Building parse tree...");
  auto update_stmt = peloton_parser.BuildParseTree("UPDATE department_table SET dept_name = 'CS' WHERE dept_id = 1");
  LOG_INFO("Building parse tree completed!");
  LOG_INFO("Building plan tree...");
  statement->SetPlanTree(optimizer::SimpleOptimizer::BuildPelotonPlanTree(update_stmt));
  LOG_INFO("Building plan tree completed!");
  bridge::PlanExecutor::PrintPlan(statement->GetPlanTree().get(), "Plan");
  LOG_INFO("Executing plan...");
  status = bridge::PlanExecutor::ExecutePlan(statement->GetPlanTree().get(), params, result);
  LOG_INFO("Statement executed. Result: %d", status.m_result);
  LOG_INFO("Tuple Updated!");
  txn_manager.CommitTransaction();

  txn_manager.BeginTransaction();
  LOG_INFO("Deleting a tuple...");
  LOG_INFO("Query: DELETE FROM department_table WHERE dept_name = 'CS'");
  statement.reset(new Statement("DELETE", "DELETE FROM department_table WHERE dept_name = 'CS'"));
  LOG_INFO("Building parse tree...");
  auto delete_stmt = peloton_parser.BuildParseTree("DELETE FROM department_table WHERE dept_name = 'CS'");
  LOG_INFO("Building parse tree completed!");
  LOG_INFO("Building plan tree...");
  statement->SetPlanTree(optimizer::SimpleOptimizer::BuildPelotonPlanTree(delete_stmt));
  LOG_INFO("Building plan tree completed!");
  bridge::PlanExecutor::PrintPlan(statement->GetPlanTree().get(), "Plan");
  LOG_INFO("Executing plan...");
  status = bridge::PlanExecutor::ExecutePlan(statement->GetPlanTree().get(), params, result);
  LOG_INFO("Statement executed. Result: %d", status.m_result);
  LOG_INFO("Tuple deleted!");
  txn_manager.CommitTransaction();
}

}  // End test namespace
}  // End peloton namespace

