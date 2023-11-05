#include "ProjectService.hpp"


/**
* 创建一个 project信息，返回创建成功的基础信息，不需要填充
*/
oatpp::Object<ProjectDto> ProjectService::createProject(const oatpp::Object<ProjectDto>& dto) {

    auto dbResult = m_database->projectDb->createProject(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto projectId = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());

    return getProjectById((v_int32) projectId, dbResult->getConnection());
};


/**
* 根据 project_id， 修改 project 基础信息，不填充
*/
oatpp::Object<ProjectDto> ProjectService::updateProject(const oatpp::Object<ProjectDto>& dto) {

  auto dbResult = m_database->projectDb->updateProject(dto);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  return getProjectById((v_int32) dto->project_id, dbResult->getConnection());
}


/**
 * 根据 project_id 取得 project 的信息
 * 不填充
*/
oatpp::Object<ProjectDto> ProjectService::getProjectById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

  auto dbResult = m_database->projectDb->getProjectById(id, connection);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "project not found");

  auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<ProjectDto>>>();
  OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

  return result[0];
}


/**
 * 根据 UserId 获取其所有 project 信息
 * 不需要填充组件列表信息
*/
oatpp::Object<ProjectsByUserIdDto> ProjectService::getProjectsByUserId(const oatpp::Int32& id){
    auto dbResult = m_database->projectDb->getProjectByUserId(id);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto items = dbResult->fetch<oatpp::Vector<oatpp::Object<ProjectDto>>>();

    auto result = ProjectsByUserIdDto::createShared();
    result->user_id = id;
    result->count = items->size();
    result->projects = items;

    return result;
}



/**
* 根据 project_id 删除 project 信息
* 需要调用 ComponentService 的服务删除对应的组件
*/
oatpp::Object<StatusDto> ProjectCompleteService::deleteProjectById(const oatpp::Int32& project_id) {

  auto dbResult = m_database->projectDb->deleteProjectById(project_id);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  ComponentService::deleteComponentsByProjectId(project_id, dbResult->getConnection());

  auto status = StatusDto::createShared();
  status->status = "OK";
  status->code = 200;
  status->message = "Project was successfully deleted";
  
  return status;
};



/**
* 根据 project id 获得 完整的 project 信息
* 完全展开 组件与子组件
*/
oatpp::Object<ProjectCompleteDto> ProjectCompleteService::getProjectCompleteById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {
  /* 先获取project基础信息 */
  auto project_base_info = ProjectService::getProjectById(id, connection);

  auto project_complete_result = ProjectCompleteDto::createShared();
  /* 基础信息一个一个拷贝 */
  project_complete_result->user_id = project_base_info->user_id;
  project_complete_result->project_id = project_base_info->project_id;
  project_complete_result->project_title = project_base_info->project_title;
  project_complete_result->project_description = project_base_info->project_description;

  /* 完整填充的组件列表 */
  project_complete_result->components = ComponentService::getCompleteComponentsByProjectId(id, connection);

  return project_complete_result;
};



/**
* 完整 project 信息保存
* 层次遍历下去，判断没有该项数据再保存
*/
oatpp::Object<ProjectCompleteDto> ProjectCompleteService::saveProjectCompolete(const oatpp::Object<ProjectCompleteDto>& project) {

  /* project 信息直接更新 */
  auto old_project_info = oatpp::Object<ProjectDto>::createShared();
  old_project_info->project_id = project->project_id;
  old_project_info->user_id = project->user_id;
  old_project_info->project_title = project->project_title;
  ProjectService::updateProject(old_project_info);

  /* 重用的 connection */
  auto connection =  m_database->projectDb->getConnection();

  /* 遍历更新组件 vector */
  for(auto& componentTransferDto : *project->components) {
    /* 组件基础信息更新 */
    auto component_base_info = oatpp::Object<ComponentRecordDto>::createShared();
    component_base_info->record_id = componentTransferDto->record_id;
    component_base_info->project_id = componentTransferDto->project_id;
    component_base_info->component_type = componentTransferDto->component_type;
    component_base_info->components_index = componentTransferDto->components_index;
    component_base_info->currentDate = componentTransferDto->currentDate;
    component_base_info->header = componentTransferDto->header;
    component_base_info->title = componentTransferDto->title;
    component_base_info->content = componentTransferDto->content;
    component_base_info->mode = componentTransferDto->mode;
    component_base_info->src = componentTransferDto->src;
    component_base_info->link = componentTransferDto->link;
    component_base_info->label1 = componentTransferDto->label1;
    component_base_info->label2 = componentTransferDto->label2;
    component_base_info->backgroundColor = componentTransferDto->backgroundColor;

    ComponentService::updateComponent(component_base_info, connection);

    /* 子组件信息更新 */
    for(auto& childComponnetTransferDto : *componentTransferDto->childs) {
      ComponentService::ChildComponentService::updateChildComponentById(childComponnetTransferDto, connection);
    }
  }

  /* 返回完整的新 project 信息 */
  return ProjectCompleteService::getProjectCompleteById(project->project_id);
};



/**
* 组件 位置 的变化移动
* 传过来：ProjectComponentsMovement
* 传回去：StatusDto
* 根据 id数组 和 index数组，一个一个地更新 component_index 数据项即可
*/
oatpp::Object<StatusDto> ProjectCompleteService::projectComponentsMove(const oatpp::Object<ProjectComponentsMovement>& moveData) {

  auto components_base = ComponentService::getComponentsByProjectId(moveData->project_id);
  size_t components_now_len = components_base->size();

  size_t length = moveData->component_record_ids->size();

  if (components_now_len != length){
    OATPP_ASSERT_HTTP(false, Status::CODE_500, " components length dose not match ");
  }
  
  /* 重用 connection */
  auto connection = m_database->componentDb->getConnection();

  for (size_t i = 0; i < length; i++)
  {
    auto dbResult = m_database->componentDb->updateComponentIndexByRecordId(moveData->component_indexs->at(i), moveData->component_record_ids->at(i), connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  }

  auto status = StatusDto::createShared();
  status->status = "OK";
  status->code = 200;
  status->message = "Components Movement Success";
  
  return status;
};



