
#ifndef CRUD_ProjectDB_HPP
#define CRUD_ProjectDB_HPP

#include "dto/ProjectDto.hpp"
#include "oatpp-sqlite/orm.hpp"

#include <iostream>

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

/**
 * ProjectDb client definitions.
 */
class ProjectDb : public oatpp::orm::DbClient {
public:

  /**
   * 构造 ProjectDb 数据库clent
   * 需要 传入 Executor 执行SQL语句的执行器
  */
  ProjectDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {}

  QUERY(createProject,
        "INSERT INTO Project"
        "(user_id, project_title, project_description) VALUES "
        "(:project.user_id, :project.project_title, :project.project_description);",
        PARAM(oatpp::Object<ProjectDto>, project))

  QUERY(updateProject,
        "UPDATE Project "
        "SET "
        " project_title=:project.project_title, "
        " project_description=:project.project_description "
        "WHERE "
        " project_id=:project.project_id;",
        PARAM(oatpp::Object<ProjectDto>, project))

  QUERY(getProjectById,
        "SELECT * FROM Project WHERE project_id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(getProjectByUserId,
        "SELECT * FROM Project WHERE user_id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(getAllProjectsNoLimit,
        "SELECT * FROM Project;")

  QUERY(deleteProjectById,
        "DELETE FROM Project WHERE project_id=:id;",
        PARAM(oatpp::Int32, id))

};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

#endif //CRUD_ProjectDB_HPP
