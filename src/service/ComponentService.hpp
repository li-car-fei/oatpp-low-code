#ifndef CRUD_COMPONENTSERVICE_HPP
#define CRUD_COMPONENTSERVICE_HPP

#include "db/Db.hpp"
#include "dto/ComponentRecordDto.hpp"
#include "dto/StatusDto.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"


/**
 * child component 子组件相关的服务
*/
class ChildComponentService {
private:
    typedef oatpp::web::protocol::http::Status Status;
private:
    std::shared_ptr<Db> m_database = nullptr;

public:
    ChildComponentService() = default;
    ChildComponentService(std::shared_ptr<Db> database) : m_database(database) {};

    /**
     * child component 子组件相关的服务
    */

   /**
    * 创建 子组件信息
   */
   oatpp::Object<ChildComponentTranserDto> createChildComponent(const oatpp::Object<ChildComponentTranserDto>& dto, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);
   
   /**
    * 根据 record_id 获取 子组件信息
   */
   oatpp::Object<ChildComponentTranserDto> getChildComponentById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);
   
    /**
     * 根据 record_id 修改 子组件信息
    */
   oatpp::Object<ChildComponentTranserDto> updateChildComponentById(const oatpp::Object<ChildComponentTranserDto>& dto, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);
   
   /**
    * 根据 record_id 删除 子组件信息
   */
   oatpp::Object<StatusDto> deleteChildComponentById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);

   /**
    * 根据 component_id 得到 所有的 正序排序的 子组件序列
   */
   oatpp::Vector<oatpp::Object<ChildComponentTranserDto>> getChildComponentsByComponentId(const oatpp::Int32& componentId, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);

   /**
    * 根据 component_id 删除 所有附属的子组件记录（当父组件删除时，其子组件一并删除）
   */
   oatpp::Object<StatusDto> deleChildComponentsByComponentId(const oatpp::Int32& component_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);
};


/**
 * component 组件相关的服务
*/
class ComponentService : public ChildComponentService {
private:
    typedef oatpp::web::protocol::http::Status Status;
private:
    std::shared_ptr<Db> m_database = nullptr;

public:
    ComponentService() = default;
    ComponentService(std::shared_ptr<Db> database) : m_database(database), ChildComponentService(database) {};

    /**
     * ComponentRecord 表相关的服务操作
    */

   /**
    * 根据 record_id 查询组件信息，需要填充子组件信息
   */
   oatpp::Object<ComponentTransferDto> getComponentById(const oatpp::Int32& record_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);

   /**
    * 创建一个组件记录，不考虑传入 子组件列表的情况 
    * 返回 要给完整组件信息记录，填充了空子组件列表的
   */
   oatpp::Object<ComponentTransferDto> createComponent(const oatpp::Object<ComponentRecordDto>& dto);

   /**
    * 根据 record_id 修改组件信息，不传入子组件的信息
    * 返回 要给完整组件信息记录，填充了空子组件列表的
   */
   oatpp::Object<ComponentTransferDto> updateComponent(const oatpp::Object<ComponentRecordDto>& dto);

   /**
    * 根据 record_id 删除组件
    * 需要 调用子组件的服务，删除相关的子组件
   */
   oatpp::Object<StatusDto> deleteComponent(const oatpp::Int32& record_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);


    /**
     * 根据 project_id 删除 所有附属的组件记录（当project删除时，其组件一并删除）
    */
    oatpp::Object<StatusDto> deleteComponentsByProjectId(const oatpp::Int32& project_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);


    /**
     * 根据 project_id 获取 组件基础信息列表
    */
    oatpp::Vector<oatpp::Object<ComponentRecordDto>> getComponentsByProjectId(const oatpp::Int32& project_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);


   /* 
   * 根据 project_id 获取完整组件列表信息，需要填充每个组件的子组件信息
   * 先根据 project_id 得到正序排序的 组件信息，再根据组件id调用 getComponentById 得到其完整信息
   */
   oatpp::Vector<oatpp::Object<ComponentTransferDto>> getCompleteComponentsByProjectId(const oatpp::Int32& project_id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);


    /**
    * child component 中的 clickable 字段 需要完成 bool 和 INT32 的转换
    * T : 希望转出的类型
    * V： 传入的类型
    * 内部 clickable 字段转成什么类型
    */
   template<class T, class V, class K>
   oatpp::Vector<oatpp::Object<T>> childComponentsClickableTransfer(oatpp::Vector<oatpp::Object<V>>& inVec);
};


#endif //CRUD_COMPONENTSERVICE_HPP