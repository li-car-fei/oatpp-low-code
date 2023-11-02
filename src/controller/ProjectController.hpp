#ifndef ProjectController_hpp
#define ProjectController_hpp

#include "service/ProjectService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

/**
 * Project REST controller
*/
class ProjectController : public oatpp::web::server::api::ApiController {
public:
    ProjectController(std::shared_ptr<Db> database,
                      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper), 
            m_projectService{database},                                  /** 使用{}指定参数，避免和拷贝构造弄混 */
            m_projectCompleteService{database}
    {}

private:
    ProjectService m_projectService;
    ProjectCompleteService m_projectCompleteService;

public:
    static std::shared_ptr<ProjectController> createShared(
        std::shared_ptr<Db> database,
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    {
        return std::make_shared<ProjectController>(database);
    };

    /**
     * 具体的 http 路由 EndPoints
    */

    /**
     * 创建一个 project 基础信息
     * 没有详细的 组件信息
    */
    ENDPOINT_INFO(createProject) {
    info->summary = "Create new Project";

    info->addConsumes<Object<ProjectDto>>("application/json");

    info->addResponse<Object<ProjectDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "projects", createProject,
            BODY_DTO(Object<ProjectDto>, projectDto))
    {
        return createDtoResponse(Status::CODE_200, m_projectService.createProject(projectDto));
    }


    /**
     * 根据 project_id 读取 project 信息，
     * 不填充 组件完整信息
    */
    ENDPOINT_INFO(getProjectById) {
        info->summary = "Get one project by project id";

        info->addResponse<Object<ProjectDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["projectId"].description = "Project Identifier";
    }
    ENDPOINT("GET", "project/{projectId}", getProjectById,
           PATH(Int32, projectId))
    {
        return createDtoResponse(Status::CODE_200, m_projectService.getProjectById(projectId));
    }


    /**
     * 根据 project_id 修改 project 基础信息
    */
   ENDPOINT_INFO(updateProjectById) {
        info->summary = "Update Project info by record Id";

        info->addConsumes<Object<ProjectDto>>("application/json");

        info->addResponse<Object<ProjectDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["projectId"].description = "project id";
    }
    ENDPOINT("PUT", "project/{projectId}", updateProjectById,
           PATH(Int32, projectId),
           BODY_DTO(Object<ProjectDto>, dto))
    {
        dto->project_id = projectId;
        return createDtoResponse(Status::CODE_200, m_projectService.updateProject(dto));
    }


    /**
     * 根据 userId 得到其所有 project的基础信息
     * 不填充 完整组件信息 
    */
    ENDPOINT_INFO(getProjectsByUserId)
    {
            info->summary = "Get projects by user id";

            info->addResponse<Object<ProjectsByUserIdDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

            info->pathParams["userId"].description = "user Id";
    }
    ENDPOINT("GET", "projects/{userId}", getProjectsByUserId,
                PATH(Int32, userId))
    {
        return createDtoResponse(Status::CODE_200, m_projectService.getProjectsByUserId(userId));
    }


    /**
     * 根据 project_id 删除 project 信息; 需要向下层组件递归删除信息；
    */
   ENDPOINT_INFO(deleteProjectById) {
        info->summary = "Delete project by project id";

        info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["projectId"].description = "project Id";
    }
    ENDPOINT("DELETE", "project/{projectId}", deleteProjectById,
                PATH(Int32, projectId))
    {
        return createDtoResponse(Status::CODE_200, m_projectCompleteService.deleteProjectById(projectId));
    }


    /**
     * 根据 project_id 得到完全展开 组件与子组件信息 的 完整 project信息
    */
   ENDPOINT_INFO(getProjectCompleteById) {
        info->summary = "Get complete project info by project id";

        info->addResponse<Object<ProjectCompleteDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["projectId"].description = "Project Identifier";
    }
    ENDPOINT("GET", "project/complete/{projectId}", getProjectCompleteById,
           PATH(Int32, projectId))
    {
        return createDtoResponse(Status::CODE_200, m_projectCompleteService.getProjectCompleteById(projectId));
    }


    /**
     * 完整 project 信息保存
    */
    ENDPOINT_INFO(saveProjectComplete) {
        info->summary = "save complete project info";

        info->addConsumes<Object<ProjectCompleteDto>>("application/json");

        info->addResponse<Object<ProjectCompleteDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["projectId"].description = "Project Identifier";
    }
    ENDPOINT("PUT", "project/complete/{projectId}", saveProjectComplete,
            PATH(Int32, projectId),
            BODY_DTO(Object<ProjectCompleteDto>, dto))
    {
        dto->project_id = projectId;
        return createDtoResponse(Status::CODE_200, m_projectCompleteService.saveProjectCompolete(dto));
    }


    /**
     * 组件位置移动
    */
   ENDPOINT_INFO(componentsMove) {
        info->summary = "componnets movement";

        info->addConsumes<Object<ProjectComponentsMovement>>("application/json");

        info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["projectId"].description = "project id";
    }
    ENDPOINT("PUT", "component/move/{projectId}", componentsMove,
           PATH(Int32, projectId),
           BODY_DTO(Object<ProjectComponentsMovement>, dto))
    {
        dto->project_id = projectId;
        return createDtoResponse(Status::CODE_200, m_projectCompleteService.projectComponentsMove(dto));
    }

};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif /* ProjectController_hpp */