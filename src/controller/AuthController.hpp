#ifndef JWT_AUTHCONTROLLER_HPP
#define JWT_AUTHCONTROLLER_HPP

#include "service/AuthService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

/**
 * User REST controller.
 */
class AuthController : public oatpp::web::server::api::ApiController {
public:
  AuthController(
    std::shared_ptr<Db> database,
    std::shared_ptr<JWT> jwt,
    OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper),
    m_authService(database, jwt)
  {}

private:
  AuthService m_authService; // auth service.

public:

  static std::shared_ptr<AuthController> createShared(
    std::shared_ptr<Db> database,
    std::shared_ptr<JWT> jwt,
    OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
  ){
    return std::make_shared<AuthController>(database, jwt, objectMapper);
  }

  ENDPOINT_INFO(signUp) {
    info->summary = "Sign up";

    info->addConsumes<Object<SignUpDto>> ("application/json");

    info->addResponse<Object<AuthDto>> (Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("POST", "users/signup", signUp,
           BODY_DTO(Object<SignUpDto>, dto))
  {
    return createDtoResponse(Status::CODE_200, m_authService.signUp(dto));
  }

  ENDPOINT_INFO(signIn) {
    info->summary = "Sign in";

    info->addConsumes <Object<SignInDto>> ("application/json");

    info->addResponse <Object<AuthDto>> (Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("POST", "users/signin", signIn,
           BODY_DTO(Object<SignInDto>, dto))
  {
    return createDtoResponse(Status::CODE_200, m_authService.signIn(dto));
  }

};

#include OATPP_CODEGEN_BEGIN(ApiController) //<- End Codegen

#endif // JWT_AUTHCONTROLLER_HPP