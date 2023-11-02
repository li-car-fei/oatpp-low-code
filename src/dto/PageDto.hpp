
#ifndef CRUD_PAGEDTO_HPP
#define CRUD_PAGEDTO_HPP

#include "UserDto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 按 offset 和 limit 读取数据，数据类型为模板参数 T
*/
template<class T>
class PageDto : public oatpp::DTO {

  DTO_INIT(PageDto, DTO)

  DTO_FIELD(UInt32, offset);
  DTO_FIELD(UInt32, limit);
  DTO_FIELD(UInt32, count);
  DTO_FIELD(Vector<T>, items);

};

class UsersPageDto : public PageDto<oatpp::Object<UserDto>> {

  DTO_INIT(UsersPageDto, PageDto<oatpp::Object<UserDto>>)

};

/**
 * 某张表的全部数据的 Dto
*/
template<class T>
class AllDataDto : public oatpp::DTO {
  DTO_INIT(AllDataDto, DTO)

  DTO_FIELD(UInt32, count);
  DTO_FIELD(Vector<T>, data);
};

class UsersAllDataDto : public AllDataDto<oatpp::Object<UserDto>> {

  DTO_INIT(UsersAllDataDto, AllDataDto<oatpp::Object<UserDto>>)

};

#include OATPP_CODEGEN_END(DTO)

#endif //CRUD_PAGEDTO_HPP
