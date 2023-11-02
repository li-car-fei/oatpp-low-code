#ifndef ProjectDto_hpp
#define ProjectDto_hpp

#include "ComponentRecordDto.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * project 项目的 DTO
*/
class ProjectDto : public oatpp::DTO {
  
  DTO_INIT(ProjectDto, DTO)

  DTO_FIELD(Int32, project_id);
  DTO_FIELD(Int32, user_id);
  DTO_FIELD(String, project_title, "project_title");
  DTO_FIELD(String, project_description, "project_description");

};

/**
 * 根据 user_id 获取 其projects数据 的 DTO
*/
class ProjectsByUserIdDto : public oatpp::DTO {
    DTO_INIT(ProjectsByUserIdDto, DTO)

    DTO_FIELD(Int32, user_id);
    DTO_FIELD(UInt32, count);
    DTO_FIELD(Vector<oatpp::Object<ProjectDto>>, projects);
};

/**
 * 完全展开的 project 项目信息
*/
class ProjectCompleteDto : public ProjectDto {
  DTO_INIT(ProjectCompleteDto, ProjectDto)

  DTO_FIELD(Vector<oatpp::Object<ComponentTransferDto>>, components, "components");
};


/**
 * 完成 组件 Components 移动所对应的 数据库操作
 * 不需要所有组件信息删除再添加，只需更改每个组件的 components_index
 * 前端传过来需要是：project_id， new_components_indexs: []
 * 即将 project_id 对应的组件列表 的排序 更改为 new_components_indexs
*/
class ProjectComponentsMovement : public oatpp::DTO {
  DTO_INIT(ProjectComponentsMovement, DTO)

  DTO_FIELD(Int32, project_id);
  DTO_FIELD(Vector<oatpp::Int32>, component_record_ids);
  DTO_FIELD(Vector<oatpp::Int32>, component_indexs);
};


#include OATPP_CODEGEN_END(DTO)

#endif /* ProjectDto_hpp */
