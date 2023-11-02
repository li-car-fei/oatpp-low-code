#include "AppComponent.hpp"

#include "controller/MyController.hpp"
#include "controller/StaticController.hpp"

#include "oatpp-swagger/Controller.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>

/**
 * OATPP_COMPONENT 是一个宏定义：
 *                 注入组件；创建类型为type，变量名为name的变量，并将已经注册的组件赋值给它（都是共享指针）；
*/

void run() {

  /* Register Components in scope of run() method */
  AppComponent components;
  
  /* 拿到 路由器组件，通过 router 变量名可访问 */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints docEndpoints;
  
  /* 数据表相关的 Controller */
  /* 需要将 components.databaseComponent.m_database 传到MyController的新建，进一步传到 Service的新建 */
  MyController::createShared(components.databaseComponent.m_database)->setRouter(docEndpoints, router);
  /* docEndpoints.append(router->addController(UserController::createShared(components.databaseComponent.m_database))->getEndpoints()); */

  /* 将 Controller 中的路由信息，添加到 swagger 中 */
  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));

  /* 静态页面 Controller 配置 */
  router->addController(StaticController::createShared());


  /* 拿到 HTTP 请求处理器组件 */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* 拿到 TCP连接组件 */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* 通过 TCP连接组件 和 HTTP请求连接处理器 构建服务器 */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Print info about server port */
  OATPP_LOGD("Server", "Running on port %s...", connectionProvider->getProperty("port").toString()->c_str());

  /* Run server */
  server.run();

  /* stop db connection pool */
  OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
  dbConnectionProvider->stop();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {

  oatpp::base::Environment::init();

  run();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  return 0;
}
