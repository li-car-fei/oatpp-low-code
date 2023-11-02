#ifndef MyController_hpp
#define MyController_hpp

#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/Server.hpp"

#include "db/Db.hpp"
#include "UserController.hpp"
#include "ProjectController.hpp"
#include "ComponentController.hpp"
#include "UploadController.hpp"


/**
 * 通过一个 MyController 将所有的 数据库相关的 Controller 包含起来，
 * 统一完成 原本在App.cpp 中的 路由的配置
*/

class MyController {
  public:
  /* 构造函数 ： 一个database对象构建所有 Controller */
    MyController(std::shared_ptr<Db> database) : 
      userController(UserController::createShared(database)),
      projectController(ProjectController::createShared(database)),
      componentController(ComponentController::createShared(database)),
      childComponentController(ChildComponentController::createShared(database)),
      uploadController(UploadController::createShared())
    {};


  /* 静态函数，MyController 只需要一个实例即可 */
    static std::shared_ptr<MyController> createShared(
      std::shared_ptr<Db> database)
    {
      return std::make_shared<MyController>(database);
    }

  /* 通过 MyController 完成 所有 子Controller 的路由配置 */
  void setRouter(
    oatpp::web::server::api::Endpoints& docEndpoints,
    std::shared_ptr<oatpp::web::server::HttpRouter> router)
  {
    docEndpoints.append(router->addController(userController)->getEndpoints());
    docEndpoints.append(router->addController(projectController)->getEndpoints());
    docEndpoints.append(router->addController(componentController)->getEndpoints());
    docEndpoints.append(router->addController(childComponentController)->getEndpoints());
    docEndpoints.append(router->addController(uploadController)->getEndpoints());
  };


  /* 所有的 Controller */
    std::shared_ptr<UserController> userController;
    std::shared_ptr<ProjectController> projectController;
    std::shared_ptr<ComponentController> componentController;
    std::shared_ptr<ChildComponentController> childComponentController;
    std::shared_ptr<UploadController> uploadController;
};

#endif /* MyController_hpp */
