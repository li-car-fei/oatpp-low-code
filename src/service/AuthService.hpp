#ifndef JWT_AUTHSERVICE_HPP
#define JWT_AUTHSERVICE_HPP

#include "auth/JWT.hpp"

#include "db/UserDb.hpp"
#include "dto/AuthDto.hpp"
#include "dto/StatusDto.hpp"

#include "BaseService.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"

class AuthService : virtual public BaseService {

private:
  std::shared_ptr<JWT> m_jwt; // JWT component

public:
  /* 构造函数 */
  AuthService(std::shared_ptr<Db> database, std::shared_ptr<JWT> jwt) : BaseService(database), m_jwt(jwt) {};

  oatpp::Object<AuthDto> signUp(const oatpp::Object<SignUpDto>& dto);
  oatpp::Object<AuthDto> signIn(const oatpp::Object<SignInDto>& dto);

};

#endif //JWT_AUTHSERVICE_HPP