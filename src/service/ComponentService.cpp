#include "ComponentService.hpp"

/**
* 创建一个组件记录，不考虑传入 子组件列表的情况 
*/
oatpp::Object<ComponentTransferDto> ComponentService::createComponent(const oatpp::Object<ComponentRecordDto>& dto) {

    auto dbResult = m_database->componentDb->createComponent(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto recordId = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());

    return getComponentById((v_int32) recordId, dbResult->getConnection());
};


/**
* 根据 record_id 修改组件信息，不传入子组件的信息
* 返回 要给完整组件信息记录，填充了空子组件列表的
*/
oatpp::Object<ComponentTransferDto> ComponentService::updateComponent(const oatpp::Object<ComponentRecordDto>& dto, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->componentDb->updateComponent(dto, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    return getComponentById((v_int32) dto->record_id, dbResult->getConnection());
};


/**
* 根据 record_id 删除组件
* 需要 调用子组件的服务，删除相关的子组件
*/
oatpp::Object<StatusDto> ComponentService::deleteComponent(const oatpp::Int32& record_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->componentDb->deleteComponentByRecordId(record_id, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    /* 删除相关的 子组件 */
    ChildComponentService::deleChildComponentsByComponentId(record_id);

    auto status = StatusDto::createShared();
    status->status = "OK";
    status->code = 200;
    status->message = "The Component was successfully deleted";

    return status;
};


/**
* 根据 project_id 删除 所有附属的组件记录（当project删除时，其组件一并删除）
* 每个组件都还要删除其子组件信息：因此，先根据 project_id 找出相关的记录，再调用 deleteComponent 一个一个地完整删除
*/
oatpp::Object<StatusDto> ComponentService::deleteComponentsByProjectId(const oatpp::Int32& project_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->componentDb->getComponentsByProjectId(project_id, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto componentsOfProjectId = dbResult->fetch<oatpp::Vector<oatpp::Object<ComponentRecordDto>>>();
    for (auto& componentRecord : *componentsOfProjectId)
    {
        ComponentService::deleteComponent(componentRecord->record_id, dbResult->getConnection());
    }

    auto status = StatusDto::createShared();
    status->status = "OK";
    status->code = 200;
    status->message = "The Components of the Project were successfully deleted";

    return status;
};


/**
 * 根据 component_id 取得 component 的信息
 * 需要填充 子组件列表信息
*/
oatpp::Object<ComponentTransferDto> ComponentService::getComponentById(const oatpp::Int32& record_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection){
    auto component_result = m_database->componentDb->getComponentByRecordId(record_id, connection);
    OATPP_ASSERT_HTTP(component_result->isSuccess(), Status::CODE_500, component_result->getErrorMessage());
    OATPP_ASSERT_HTTP(component_result->hasMoreToFetch(), Status::CODE_404, "Component not found");

    auto base_result = component_result->fetch<oatpp::Vector<oatpp::Object<ComponentRecordDto>>>();
    OATPP_ASSERT_HTTP(base_result->size() == 1, Status::CODE_500, "Unknown error");

    /* 不可以自动按值拷贝 */
    auto transfer_result = oatpp::Object<ComponentTransferDto>::createShared();
    /* 一个一个值的拷贝 */
    transfer_result->record_id = base_result[0]->record_id;
    transfer_result->project_id = base_result[0]->project_id;
    transfer_result->component_type = base_result[0]->component_type;
    transfer_result->components_index = base_result[0]->components_index;
    transfer_result->currentDate = base_result[0]->currentDate;
    transfer_result->header = base_result[0]->header;
    transfer_result->title = base_result[0]->title;
    transfer_result->content = base_result[0]->content;
    transfer_result->mode = base_result[0]->mode;
    transfer_result->src = base_result[0]->src;
    transfer_result->link = base_result[0]->link;
    transfer_result->label1 = base_result[0]->label1;
    transfer_result->label2 = base_result[0]->label2;
    transfer_result->backgroundColor = base_result[0]->backgroundColor;

    /* 调用 childComponent 的服务，找到当前 component 对应的所有的 child components */
    transfer_result->childs = ChildComponentService::getChildComponentsByComponentId(record_id, component_result->getConnection());

    return transfer_result;
};


/**
* 根据 project_id 获取 组件基础信息列表
*/
oatpp::Vector<oatpp::Object<ComponentRecordDto>> ComponentService::getComponentsByProjectId(const oatpp::Int32& project_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->componentDb->getComponentsByProjectId(project_id, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto items = dbResult->fetch<oatpp::Vector<oatpp::Object<ComponentRecordDto>>>();

    auto components_results = oatpp::Vector<oatpp::Object<ComponentRecordDto>>::createShared();
    components_results = items;

    return components_results;
};



/* 
* 根据 project_id 获取完整组件列表信息，需要填充每个组件的子组件信息
* 先根据 project_id 得到正序排序的 组件基础信息，再根据组件id调用 getComponentById 得到其完整信息
*/
oatpp::Vector<oatpp::Object<ComponentTransferDto>> ComponentService::getCompleteComponentsByProjectId(const oatpp::Int32& project_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection){

    auto db_components_base = m_database->componentDb->getComponentsByProjectId(project_id, connection);
    OATPP_ASSERT_HTTP(db_components_base->isSuccess(), Status::CODE_500, db_components_base->getErrorMessage());

    auto components_base = db_components_base->fetch<oatpp::Vector<oatpp::Object<ComponentRecordDto>>>();

    /**
     * oatpp::Vector 需要初始化，不然是 nullptr: 
     * https://github.com/oatpp/oatpp/issues/787
    */
    auto complete_components_result = oatpp::Vector<oatpp::Object<ComponentTransferDto>>::createShared();
    for(auto& componentRecord : *components_base){
        complete_components_result->emplace_back(ComponentService::getComponentById(componentRecord->record_id, connection));
    }

    return complete_components_result;

}




/**
* 创建 子组件基本信息
*/
oatpp::Object<ChildComponentTranserDto> ChildComponentService::createChildComponent(const oatpp::Object<ChildComponentTranserDto>& dto, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->childComponentDb->createChildComponent(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto childComponentId = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());

    return getChildComponentById((v_int32) childComponentId);
};


/**
* 根据 record_id 修改 子组件信息
*/
oatpp::Object<ChildComponentTranserDto> ChildComponentService::updateChildComponentById(const oatpp::Object<ChildComponentTranserDto>& dto, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->childComponentDb->updateChildComponent(dto, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    return getChildComponentById(dto->record_id);
};


/**
* 根据 record_id 获取 子组件信息
*/
oatpp::Object<ChildComponentTranserDto> ChildComponentService::getChildComponentById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection){

    auto dbResult = m_database->childComponentDb->getChildComponentByRecordId(id, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "ChildComponent not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<ChildComponentTranserDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

    return result[0];
};


/**
* 根据 record_id 删除 子组件信息
*/
oatpp::Object<StatusDto> ChildComponentService::deleteChildComponentById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection){

    auto dbResult = m_database->childComponentDb->deleteChildComponentByRecordId(id, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  
    auto status = StatusDto::createShared();
    status->status = "OK";
    status->code = 200;
    status->message = "ChildComponent was successfully deleted";

    return status;
};
   


/**
* 根据 component_id 得到 所有的 正序排序的 子组件序列
*/
oatpp::Vector<oatpp::Object<ChildComponentTranserDto>> ChildComponentService::getChildComponentsByComponentId(const oatpp::Int32& componentId, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->childComponentDb->getChildComponentsByComponentId(componentId, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto result = oatpp::Vector<oatpp::Object<ChildComponentTranserDto>>::createShared();

    result = dbResult->fetch<oatpp::Vector<oatpp::Object<ChildComponentTranserDto>>>();

    return result;
};



/**
* 根据 component_id 删除 所有附属的子组件记录（当父组件删除时，其子组件一并删除）
*/
oatpp::Object<StatusDto> ChildComponentService::deleChildComponentsByComponentId(const oatpp::Int32& component_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

    auto dbResult = m_database->childComponentDb->deleChildComponentsByComponentId(component_id, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto status = StatusDto::createShared();
    status->status = "OK";
    status->code = 200;
    status->message = "ChildComponent of the father component were successfully deleted";

    return status;
};



/**
 * child component 中的 clickable 字段 需要完成 bool 和 INT32 的转换
 * T : 希望转出的类型
 * V： 传入的类型
 * 内部 clickable 字段转成什么类型
*/
template<class T, class V, class K>
oatpp::Vector<oatpp::Object<T>> ComponentService::childComponentsClickableTransfer(oatpp::Vector<oatpp::Object<V>>& inVec){
    oatpp::Vector<oatpp::Object<T>> outVec;

    for(const auto& recordDto : inVec){
        oatpp::Object<T> transferDto;
        /* 直接拷贝，会不会按值成功传递？？？？？？？ */
        transferDto = recordDto;
        /* 数据类型转换 */
        transferDto->clickable = static_cast<K>(recordDto->clickable);
        outVec.push_back(transferDto);
    }

    return outVec;
};
