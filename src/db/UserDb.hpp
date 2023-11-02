
#ifndef CRUD_USERDB_HPP
#define CRUD_USERDB_HPP

#include "dto/UserDto.hpp"
#include "oatpp-sqlite/orm.hpp"

#include <iostream>

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

/**
 * UserDb client definitions.
 */
class UserDb : public oatpp::orm::DbClient {
public:

  /**
   * 构造 UserDb 数据库clent
   * 需要 传入 Executor 执行SQL语句的执行器
  */
  UserDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {}

  QUERY(createUser,
        "INSERT INTO AppUser"
        "(username, email, password, role) VALUES "
        "(:user.username, :user.email, :user.password, :user.role);",
        PARAM(oatpp::Object<UserDto>, user))

  QUERY(updateUser,
        "UPDATE AppUser "
        "SET "
        " username=:user.username, "
        " email=:user.email, "
        " password=:user.password, "
        " role=:user.role "
        "WHERE "
        " id=:user.id;",
        PARAM(oatpp::Object<UserDto>, user))

  QUERY(getUserById,
        "SELECT * FROM AppUser WHERE id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(getAllUsers,
        "SELECT * FROM AppUser LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit))

  QUERY(getAllUsersNoLimit,
        "SELECT * FROM AppUser;")

  QUERY(deleteUserById,
        "DELETE FROM AppUser WHERE id=:id;",
        PARAM(oatpp::Int32, id))

};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

#endif //CRUD_USERDB_HPP
