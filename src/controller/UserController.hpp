
#ifndef UserController_hpp
#define UserController_hpp

#include "service/UserService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include <iostream>

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

/**
 * User REST controller.
 */
class UserController : public oatpp::web::server::api::ApiController {
public:
  UserController(std::shared_ptr<Db> database,
                 OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper), m_userService{database}          /** 使用{}指定参数，避免和拷贝构造弄混 */
  {}
private:
/**
 * User 表的实际操作 service
*/
  UserService m_userService; // Create user service.
public:

  /**
   * 静态函数：从环境中得到序列化器作为默认参数，函数返回 UserController 的共享指针
  */
  static std::shared_ptr<UserController> createShared(
    std::shared_ptr<Db> database,
    OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
  ){
    return std::make_shared<UserController>(database, objectMapper);
  }

  /**
   * 具体的 http 路由 EndPoints
   * ENDPOINT_INFO 指定接口的各种可能返回状态与返回信息
   * ENDPOINT 执行具体的操作，调用 service 进行；
   * service需要完成对操作各种可能结果的判定，和返回数据的关系对应起来；而service内部有关数据库的操作则封装在另一个类 db client 中
  */
  
  ENDPOINT_INFO(createUser) {
    info->summary = "Create new User";

    info->addConsumes<Object<UserDto>>("application/json");

    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("POST", "users", createUser,
           BODY_DTO(Object<UserDto>, userDto))
  {
    return createDtoResponse(Status::CODE_200, m_userService.createUser(userDto));
  }
  
  
  ENDPOINT_INFO(putUser) {
    info->summary = "Update User by userId";

    info->addConsumes<Object<UserDto>>("application/json");

    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->pathParams["userId"].description = "User Identifier";
  }
  ENDPOINT("PUT", "users/{userId}", putUser,
           PATH(Int32, userId),
           BODY_DTO(Object<UserDto>, userDto))
  {
    userDto->id = userId;
    return createDtoResponse(Status::CODE_200, m_userService.updateUser(userDto));
  }
  
  
  ENDPOINT_INFO(getUserById) {
    info->summary = "Get one User by userId";

    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->pathParams["userId"].description = "User Identifier";
  }
  ENDPOINT("GET", "users/{userId}", getUserById,
           PATH(Int32, userId))
  {
    return createDtoResponse(Status::CODE_200, m_userService.getUserById(userId));
  }
  
  
  ENDPOINT_INFO(getUsers) {
    info->summary = "get all stored users through offset and limit";

    info->addResponse<oatpp::Object<UsersPageDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("GET", "users/offset/{offset}/limit/{limit}", getUsers,
           PATH(UInt32, offset),
           PATH(UInt32, limit))
  {
    return createDtoResponse(Status::CODE_200, m_userService.getAllUsers(offset, limit));
  }

  /**
   * 获取全部用户，不管 offset 和 limit
  */
  ENDPOINT_INFO(getAllUsers) {
    info->summary = "get all users";
    info->addResponse<oatpp::Object<UsersAllDataDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("GET", "usersAll", getAllUsers)
  {
    return createDtoResponse(Status::CODE_200, m_userService.getAllUsersNolimit());
  }
  
  
  ENDPOINT_INFO(deleteUser) {
    info->summary = "Delete User by userId";

    info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->pathParams["userId"].description = "User Identifier";
  }
  ENDPOINT("DELETE", "users/{userId}", deleteUser,
           PATH(Int32, userId))
  {
    return createDtoResponse(Status::CODE_200, m_userService.deleteUserById(userId));
  }

};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif /* UserController_hpp */
