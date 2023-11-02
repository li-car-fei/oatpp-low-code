#ifndef CRUD_DB_HPP
#define CRUD_DB_HPP

#include "UserDb.hpp"
#include "ProjectDb.hpp"
#include "ComponentDb.hpp"
#include "oatpp-sqlite/orm.hpp"

#include <iostream>

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen


/**
 * 总的 DB 类，管理所有的 数据相关操作类
*/
class Db {
public:
    Db(const std::shared_ptr<oatpp::orm::Executor>& executor)
        : userDb(std::make_shared<UserDb>(executor)),
          projectDb(std::make_shared<ProjectDb>(executor)),
          componentDb(std::make_shared<ComponentDb>(executor)),
          childComponentDb(std::make_shared<ChildComponentDb>(executor))
    {
        std::cout << "Db start creating" << std::endl;
        
        oatpp::orm::SchemaMigration migration(executor);
        migration.addFile(1 /* start from version 1 */, DATABASE_MIGRATIONS "/001_init.sql");
        // TODO - Add more migrations here.
        migration.migrate(); // <-- run migrations. This guy will throw on error.

        auto version = executor->getSchemaVersion();
        OATPP_LOGD("Db", "Migration - OK. Version=%lld.", version);

    };

    std::shared_ptr<UserDb> userDb;
    std::shared_ptr<ProjectDb> projectDb;
    std::shared_ptr<ComponentDb> componentDb;
    std::shared_ptr<ChildComponentDb> childComponentDb;
};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

#endif //CRUD_DB_HPP