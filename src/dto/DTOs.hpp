#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

/**
 * OATPP_CODEGEN_BEGIN 与 OATPP_CODEGEN_END 是宏定义，借助code-gen宏完成一些快速定义
*/

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 * 
 *  请求处理相关数据的定义
 */
class MyDto : public oatpp::DTO {

  DTO_INIT(MyDto, DTO)
  
  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
