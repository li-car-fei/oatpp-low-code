
#include "UserService.hpp"

/**
 * User 表的具体的操作函数
 * 内部会调用 m_database 即数据库连接client的函数执行；而该client对应的类中，是通过调用 Executor 执行SQL语句完成对应工作的；
*/

/**
 *  OATPP_ASSERT_HTTP 是宏定义
 * 
 * #define OATPP_ASSERT_HTTP(COND, STATUS, MESSAGE) \
    if(!(COND)) { throw oatpp::web::protocol::http::HttpError(STATUS, MESSAGE); }

 * 先判定 第一个参数是否成功，如果不成功，则抛出 HttpError，会被全局的 ErrorHandler 捕获进行处理；
 * 本项目定义的 ErrorHandler 处理程序，接收三个参数，（当前Http请求的Status，错误信息message，请求头信息Header）
*/

oatpp::Object<UserDto> UserService::createUser(const oatpp::Object<UserDto>& dto) {

  auto dbResult = m_database->userDb->createUser(dto);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  auto userId = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());

  return getUserById((v_int32) userId);

}

oatpp::Object<UserDto> UserService::updateUser(const oatpp::Object<UserDto>& dto) {

  auto dbResult = m_database->userDb->updateUser(dto);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  return getUserById(dto->id);

}

oatpp::Object<UserDto> UserService::getUserById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

  auto dbResult = m_database->userDb->getUserById(id, connection);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");

  auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
  OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

  return result[0];

}

oatpp::Object<PageDto<oatpp::Object<UserDto>>> UserService::getAllUsers(const oatpp::UInt32& offset, const oatpp::UInt32& limit) {

  oatpp::UInt32 countToFetch = limit;

  if(limit > 10) {
    countToFetch = 10;
  }

  auto dbResult = m_database->userDb->getAllUsers(offset, countToFetch);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  /**
   * 得到一个数组的 User 数据
  */
  auto items = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();

  /**
   * 需要一个 pageDto 构建返回数据格式
  */
  auto page = PageDto<oatpp::Object<UserDto>>::createShared();
  page->offset = offset;
  page->limit = countToFetch;
  page->count = items->size();
  page->items = items;

  return page;

}

oatpp::Object<AllDataDto<oatpp::Object<UserDto>>> UserService::getAllUsersNolimit() {

  auto dbResult = m_database->userDb->getAllUsersNoLimit();
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  auto items = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();

  auto allData = AllDataDto<oatpp::Object<UserDto>>::createShared();
  allData->count = items->size();
  allData->data = items;

  return allData;
}


oatpp::Object<StatusDto> UserService::deleteUserById(const oatpp::Int32& userId) {

  auto dbResult = m_database->userDb->deleteUserById(userId);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  auto status = StatusDto::createShared();
  status->status = "OK";
  status->code = 200;
  status->message = "User was successfully deleted";
  
  return status;
}