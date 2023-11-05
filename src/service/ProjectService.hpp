#ifndef CRUD_PROJECTSERVICE_HPP
#define CRUD_PROJECTSERVICE_HPP

#include "db/Db.hpp"
#include "dto/ProjectDto.hpp"
#include "dto/StatusDto.hpp"

#include "BaseService.hpp"
#include "ComponentService.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"

#include <iostream>

class ProjectService : virtual public BaseService {

public:
    ProjectService(std::shared_ptr<Db> database) : BaseService(database) {};

    /**
     * Project 表相关的 操作函数 service
    */


   /**
    * 创建一个 project 信息，返回创建成功的基础信息，不填充
   */
   oatpp::Object<ProjectDto> createProject(const oatpp::Object<ProjectDto>& dto);


    /* 
   * 根据 project_id 获取 project 基础信息，不填充 
   */
   oatpp::Object<ProjectDto> getProjectById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);
   

   /**
    * 根据 project_id， 修改 project 基础信息，不填充
   */
    oatpp::Object<ProjectDto> updateProject(const oatpp::Object<ProjectDto>& dto);
   
   /**
    * 根据 project_id 删除 project 信息
    * 需要放在 ProjectCompleteService，需要向上层递归删除对应组件与子组件记录
   */


   /**
    * 根据 UserId 获取其所有 project 信息，不填充
   */
    oatpp::Object<ProjectsByUserIdDto> getProjectsByUserId(const oatpp::Int32& id);

};

/**
 * 需要有一个 能够调用 Project、Component、ChildComponent 服务的类，完成 完全展开的 project 信息的 插入与
 * 这个类应当 继承于 Component、ChildComponent，可以直接调用两者的函数
*/
class ProjectCompleteService : public ProjectService, ComponentService {

public:
    /**
     * 虚继承中，需要由最上层派生类调用最下层基类的构造函数
     * (普通继承则不用，调用直接继承的基类的构造即可)
    */
    ProjectCompleteService(std::shared_ptr<Db> database) : BaseService(database), ProjectService(database), ComponentService(database) {};


    /**
    * 根据 project_id 删除 project 信息
    * 需要调用 projectService 的服务删除基础信息，调用 ComponentService 的服务删除对应的组件
    */
   oatpp::Object<StatusDto> deleteProjectById(const oatpp::Int32& project_id);


    /**
     * 根据 project id 获得 完整的 project 信息
     * 完全展开 组件与子组件
    */
   oatpp::Object<ProjectCompleteDto> getProjectCompleteById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);

    
    /**
     * 完整 project 信息保存
     * 层次遍历下去，判断没有该项数据再保存
    */
    oatpp::Object<ProjectCompleteDto> saveProjectCompolete(const oatpp::Object<ProjectCompleteDto>& project);


    /**
     * 组件 位置 的变化移动
     * 传过来：ProjectComponentsMovement
     * 传回去：StatusDto
     * 根据 id数组 和 index数组，一个一个地更新 component_index 数据项即可
    */
   oatpp::Object<StatusDto> projectComponentsMove(const oatpp::Object<ProjectComponentsMovement>& moveData);
};


#endif //CRUD_PROJECTSERVICE_HPP