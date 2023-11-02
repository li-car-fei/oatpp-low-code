
#ifndef CRUD_USERSERVICE_HPP
#define CRUD_USERSERVICE_HPP

#include "db/Db.hpp"
#include "dto/PageDto.hpp"
#include "dto/StatusDto.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"

#include <iostream>

class UserService {
private:
  typedef oatpp::web::protocol::http::Status Status;
private:
/**
 * 不能从 server 环境中找到 数据库client了，因为有一个统一的对象 Db对象进行管理！！！
*/
  /* OATPP_COMPONENT(std::shared_ptr<Db>, m_database); // Inject database component */

  std::shared_ptr<Db> m_database;

public:

  UserService(std::shared_ptr<Db> database) : m_database(database) {};

  /**
   * user 表的实际操作函数 service
  */

  oatpp::Object<UserDto> createUser(const oatpp::Object<UserDto>& dto);
  oatpp::Object<UserDto> updateUser(const oatpp::Object<UserDto>& dto);
  oatpp::Object<UserDto> getUserById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);
  oatpp::Object<PageDto<oatpp::Object<UserDto>>> getAllUsers(const oatpp::UInt32& offset, const oatpp::UInt32& limit);
  oatpp::Object<AllDataDto<oatpp::Object<UserDto>>> getAllUsersNolimit();
  oatpp::Object<StatusDto> deleteUserById(const oatpp::Int32& id);

};

#endif //CRUD_USERSERVICE_HPP
