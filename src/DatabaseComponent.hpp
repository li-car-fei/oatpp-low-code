#ifndef CRUD_DATABASECOMPONENT_HPP
#define CRUD_DATABASECOMPONENT_HPP

#include "db/Db.hpp"

#include <iostream>

class DatabaseComponent {
public:

/**
   * 总的数据库客户端管理对象
  */
  std::shared_ptr<Db> m_database;

  /**
   * 构建 m_database 时需要从环境中找到一些参数
  */
  std::shared_ptr<Db> make_m_database();

  /**
  * 自定义一个构造函数，给 Db 设定初值
  * 需要在构造函数内部执行，
  * 不然成员 dbConnectionProvider 还没创建，即无法从环境中 找到 dbConnectionProvider !!! !!!
  */
  DatabaseComponent() {
    m_database = make_m_database();
  };

  

    
  /**
   * Create database connection provider component：数据库连接池 , 赋予变量名 dbConnectionProvider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider)([] {

    /* Create database-specific ConnectionProvider */
    auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(DATABASE_FILE);

    /* Create database-specific ConnectionPool */
    return oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
                                                       10 /* max-connections */,
                                                       std::chrono::seconds(5) /* connection TTL */);

  }());

  /**
   * /**
   * Create database client
   *
   * OATPP_CREATE_COMPONENT(std::shared_ptr<UserDb>, userDb)([] {

   * 从环境找到数据库连接池
   * OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, connectionProvider);

   * 用 数据库连接池 初始化 Executor
   * auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

   * 用 Executor 初始化 UserDb 数据库客户端
   * return std::make_shared<UserDb>(executor);

   * }());
  */

};

/**
 * make_m_database() 函数内部，找到所需参数，再构建一个共享指针返回
*/
std::shared_ptr<Db> DatabaseComponent::make_m_database(){
  std::cout << "starting making m_database" << std::endl;

  /* 从环境找到数据库连接池 */
  OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);

  /* 用 数据库连接池 初始化 Executor */
  auto executor = std::make_shared<oatpp::sqlite::Executor>(dbConnectionProvider);

  return std::make_shared<Db>(executor);
}

#endif //CRUD_DATABASECOMPONENT_HPP