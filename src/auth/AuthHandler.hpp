#ifndef JWT_AUTHHANDLER_HPP
#define JWT_AUTHHANDLER_HPP

#include "JWT.hpp"

class AuthHandler : public oatpp::web::server::handler::BearerAuthorizationHandler {
private:
  std::shared_ptr<JWT> m_jwt;
public:

  AuthHandler(const std::shared_ptr<JWT>& jwt);

  /* override 重载验证函数 */
  std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& token) override;

};

#endif //JWT_AUTHHANDLER_HPP