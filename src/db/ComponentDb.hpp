#ifndef CRUD_ComponentDB_HPP
#define CRUD_ComponentDB_HPP

#include "dto/ComponentRecordDto.hpp"
#include "oatpp-sqlite/orm.hpp"
#include <iostream>

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

/**
 * ComponentDb client definitions.
 * update 中应当不更新 project_id 了
*/
class ComponentDb : public oatpp::orm::DbClient{
public:
    ComponentDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
    {};

    QUERY(createComponent,
        "INSERT INTO ComponentRecord "
        "(project_id, component_type, components_index, currentDate, header, "
        "title, content, mode, src, link, label1, label2, backgroundColor) VALUES "
        "(:component.project_id, :component.component_type, :component.components_index, "
        ":component.currentDate, :component.header, :component.title, :component.content, :component.mode, :component.src, "
        ":component.link, :component.label1, :component.label2, :component.backgroundColor);",
        PARAM(oatpp::Object<ComponentRecordDto>, component))

    QUERY(updateComponent,
        "UPDATE ComponentRecord "
        "SET "
        " component_type=:component.component_type, components_index=:component.components_index, "
        " currentDate=:component.currentDate, header=:component.header, title=:component.title, "
        " content=:component.content, mode=:component.mode, src=:component.src, link=:component.link, "
        " label1=:component.label1, label2=:component.label2, backgroundColor=:component.backgroundColor "
        "WHERE "
        " record_id=:component.record_id;",
        PARAM(oatpp::Object<ComponentRecordDto>, component))

    QUERY(deleteComponentByRecordId,
        "DELETE FROM ComponentRecord WHERE record_id=:record_id;",
        PARAM(oatpp::Int32, record_id))

    QUERY(deleteComponentsByProjectId,
        "DELETE FROM ChildComponentRecord WHERE project_id=:project_id;",
        PARAM(oatpp::Int32, project_id))

    QUERY(getComponentByRecordId,
        "SELECT * FROM ComponentRecord "
        "WHERE record_id=:record_id;",
        PARAM(oatpp::Int32, record_id))

    QUERY(getComponentsByProjectId,
        "SELECT * FROM ComponentRecord "
        "WHERE project_id=:project_id "
        "ORDER BY components_index;",
        PARAM(oatpp::Int32, project_id))

    QUERY(updateComponentIndexByRecordId,
        "UPDATE ComponentRecord "
        "SET components_index=:components_index "
        "WHERE "
        "record_id=:record_id;",
        PARAM(oatpp::Int32, components_index),
        PARAM(oatpp::Int32, record_id))
};


/**
 * ChildComponentDb client definitions.
 * update 中应当不更新 component_id 了
*/
class ChildComponentDb : public oatpp::orm::DbClient {
public:
    ChildComponentDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
    {};

    QUERY(createChildComponent,
        "INSERT INTO ChildComponentRecord "
        "(component_id, child_components_index, description_, "
        "title, clickable, src, link, text_, content) VALUES "
        "(:childComponent.component_id, :childComponent.child_components_index, "
        ":childComponent.description_, :childComponent.title, :childComponent.clickable, "
        ":childComponent.src, :childComponent.link, :childComponent.text_, :childComponent.content);",
        PARAM(oatpp::Object<ChildComponentTranserDto>, childComponent))

    QUERY(updateChildComponent,
        "UPDATE ChildComponentRecord "
        "SET "
        " child_components_index=:childComponent.child_components_index, "
        " description_=:childComponent.description_, title=:childComponent.title, clickable=:childComponent.clickable, "
        " src=:childComponent.src, link=:childComponent.link, text_=:childComponent.text_, content=:childComponent.content "
        "WHERE "
        " record_id=:childComponent.record_id;",
        PARAM(oatpp::Object<ChildComponentTranserDto>, childComponent))

    QUERY(getChildComponentByRecordId,
        "SELECT * FROM ChildComponentRecord WHERE record_id=:record_id;",
        PARAM(oatpp::Int32, record_id))

    QUERY(deleteChildComponentByRecordId,
        "DELETE FROM ChildComponentRecord WHERE record_id=:record_id;",
        PARAM(oatpp::Int32, record_id))


    QUERY(getChildComponentsByComponentId,
        "SELECT * FROM ChildComponentRecord "
        "WHERE component_id=:component_id "
        "ORDER BY child_components_index;",
        PARAM(oatpp::Int32, component_id))


    QUERY(deleChildComponentsByComponentId,
        "DELETE FROM ChildComponentRecord WHERE component_id=:component_id;",
        PARAM(oatpp::Int32, component_id))
};




#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

#endif //CRUD_ComponentDB_HPP