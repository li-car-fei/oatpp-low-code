#ifndef ComponentController_hpp
#define ComponentController_hpp

#include "service/ComponentService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen


/**
 * ChildComponent Rest controller
*/
class ChildComponentController : public oatpp::web::server::api::ApiController {
public:
    ChildComponentController(std::shared_ptr<Db> database, OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper),
            m_childComponentService{database}
    {};

private:
    ChildComponentService m_childComponentService;

public:
    static std::shared_ptr<ChildComponentController> createShared(
        std::shared_ptr<Db> database,
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    {
        return std::make_shared<ChildComponentController>(database);
    };

    /**
     * 具体的 http 路由 EndPoints
    */

   /**
    * 创建一个 ChildComponent
   */
   ENDPOINT_INFO(createChildComponent) {
    info->summary = "Create new ChildComponent";

    info->addConsumes<Object<ChildComponentTranserDto>>("application/json");

    info->addResponse<Object<ChildComponentTranserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "childComponent", createChildComponent,
            BODY_DTO(Object<ChildComponentTranserDto>, dto))
    {
        return createDtoResponse(Status::CODE_200, m_childComponentService.createChildComponent(dto));
    }

    /**
     * 根据 record_id 获取 子组件信息
    */
   ENDPOINT_INFO(getChildComponentById) {
    info->summary = "get childComponent by record id";

    info->addResponse<Object<ChildComponentTranserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->pathParams["recordId"].description = "child component record Id";
   }
   ENDPOINT("GET", "childComponent/{recordId}", getChildComponentById,
                PATH(Int32, recordId))
    {
        return createDtoResponse(Status::CODE_200, m_childComponentService.getChildComponentById(recordId));
    }

    /**
     * 根据 record_id 修改子组件信息
    */
   ENDPOINT_INFO(updateChildComponentById) {
    info->summary = "Update childComponent by record Id";

    info->addConsumes<Object<ChildComponentTranserDto>>("application/json");

    info->addResponse<Object<ChildComponentTranserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->pathParams["recordId"].description = "child component record id";
  }
  ENDPOINT("PUT", "childComponent/{recordId}", updateChildComponentById,
           PATH(Int32, recordId),
           BODY_DTO(Object<ChildComponentTranserDto>, dto))
  {
    dto->record_id = recordId;
    return createDtoResponse(Status::CODE_200, m_childComponentService.updateChildComponentById(dto));
  }

    /**
     * 根据 record_id 删除 子组件信息
    */
    ENDPOINT_INFO(deleteChildComponent) {
        info->summary = "Delete childComponent by record id";

        info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["recordId"].description = "child component record Id";
    }
    ENDPOINT("DELETE", "childComponent/{recordId}", deleteChildComponent,
                PATH(Int32, recordId))
    {
        return createDtoResponse(Status::CODE_200, m_childComponentService.deleteChildComponentById(recordId));
    }
};





/**
 * Component Rest controller
*/
class ComponentController : public oatpp::web::server::api::ApiController {
public:
    ComponentController(std::shared_ptr<Db> database, OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper),
            m_componentService{database}
    {};

private:
    ComponentService m_componentService;

public:
    static std::shared_ptr<ComponentController> createShared(
        std::shared_ptr<Db> database,
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    {
        return std::make_shared<ComponentController>(database);
    };

    /**
     * 具体的 http 路由 EndPoints
    */

   /**
    * 创建一个 Component 信息
    * 不传入 子组件的信息
   */
   ENDPOINT_INFO(createComponent) {
    info->summary = "Create new Component";

    info->addConsumes<Object<ComponentRecordDto>>("application/json");

    info->addResponse<Object<ComponentTransferDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "component", createComponent,
            BODY_DTO(Object<ComponentRecordDto>, componentRecordDto))
    {
        return createDtoResponse(Status::CODE_200, m_componentService.createComponent(componentRecordDto));
    }


    /**
     * 根据 record_id 获取 Component 信息
    */
   ENDPOINT_INFO(getComponentById) {
    info->summary = "get Component by record id";

    info->addResponse<Object<ComponentTransferDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->pathParams["recordId"].description = "component record Id";
   }
   ENDPOINT("GET", "component/{recordId}", getComponentById,
                PATH(Int32, recordId))
    {
        return createDtoResponse(Status::CODE_200, m_componentService.getComponentById(recordId));
    }

    /**
     * 根据 record_id 修改 Component 信息
    */
   ENDPOINT_INFO(updateComponentById) {
    info->summary = "Update Component by record Id";

    info->addConsumes<Object<ComponentRecordDto>>("application/json");

    info->addResponse<Object<ComponentTransferDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->pathParams["recordId"].description = "component record id";
    }
    ENDPOINT("PUT", "component/{recordId}", updateComponentById,
           PATH(Int32, recordId),
           BODY_DTO(Object<ComponentRecordDto>, dto))
    {
        dto->record_id = recordId;
        return createDtoResponse(Status::CODE_200, m_componentService.updateComponent(dto));
    }


    /**
     * 根据 record_id 删除 Component 信息
    */
    ENDPOINT_INFO(deleteComponent) {
        info->summary = "Delete Component by record id";

        info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["recordId"].description = "component record Id";
    }
    ENDPOINT("DELETE", "component/{recordId}", deleteComponent,
                PATH(Int32, recordId))
    {
        return createDtoResponse(Status::CODE_200, m_componentService.deleteComponent(recordId));
    }

};


#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif /* ComponentController_hpp */