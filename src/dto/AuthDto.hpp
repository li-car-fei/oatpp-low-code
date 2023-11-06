#ifndef JWT_AUTHDTO_HPP
#define JWT_AUTHDTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/* JWT token */
class AuthDto : public oatpp::DTO {
  
  DTO_INIT(AuthDto, DTO)

  DTO_FIELD(String, token);

};

/* Sign In */
class SignInDto : public oatpp::DTO {

  DTO_INIT(SignInDto, DTO)

  DTO_FIELD(String, userName, "username");
  DTO_FIELD(String, password, "password");

};

/* Sign Up */
class SignUpDto : public oatpp::DTO {

  DTO_INIT(SignUpDto, DTO)

  DTO_FIELD(String, userName, "username");
  DTO_FIELD(String, email, "email");
  DTO_FIELD(String, password, "password");

};


#include OATPP_CODEGEN_END(DTO)

#endif /* JWT_AUTHDTO_HPP */