#include "AuthService.hpp"

/* 注册 */
oatpp::Object<AuthDto> AuthService::signUp(const oatpp::Object<SignUpDto>& dto) {

  auto user = UserDto::createShared();
  user->id = nullptr;
  user->userName = dto->userName;
  user->email = dto->email;
  user->password = dto->password;
  user->role = nullptr;

  auto dbResult = m_database->userDb->createUser(user);
  if(!dbResult->isSuccess()) {
    OATPP_LOGE("AuthService", "DB-Error: '%s'", dbResult->getErrorMessage()->c_str());
  }
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_401, "Unauthorized");

  auto result = dbResult->fetch<oatpp::Vector<oatpp::Vector<oatpp::String>>>();
  OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_401, "Unauthorized")

  auto newUserId = result[0][0];

  /* 根据 userId 得到 JWT */
  auto payload = std::make_shared<JWT::Payload>();
  payload->userId_ = newUserId;

  auto auth = AuthDto::createShared();
  auth->token = m_jwt->createToken(payload);

  return auth;

}

/* 登录 */
oatpp::Object<AuthDto> AuthService::signIn(const oatpp::Object<SignInDto>& dto) {

  auto dbResult = m_database->userDb->authenticateUser(dto->userName, dto->password);
  if(!dbResult->isSuccess()) {
    OATPP_LOGE("AuthService", "DB-Error: '%s'", dbResult->getErrorMessage()->c_str());
  }
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_401, "Unauthorized")

  auto result = dbResult->fetch<oatpp::Vector<oatpp::Vector<oatpp::String>>>();
  OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_401, "Unauthorized")

  auto userId = result[0][0];

  /* 根据 userId 得到 JWT */
  auto payload = std::make_shared<JWT::Payload>();
  payload->userId_ = userId;

  auto auth = AuthDto::createShared();
  auth->token = m_jwt->createToken(payload);

  return auth;

}