#ifndef BASESERVICE_HPP
#define BASESERVICE_HPP

#include "db/Db.hpp"
#include "dto/StatusDto.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 * BaseService
 * 声明 Status 后续子类都用于HTTP状态的码的设定
 * 保存 std::shared_ptr<Db> m_database，用于数据库相关操作
 * 直接继承BaseService的类需要是 virtual 继承，则可保证所有相关Service类中都只有一份Db
*/
class BaseService {
    /* protected ，子类可以访问到，外界不可以访问 */
    protected:
        typedef oatpp::web::protocol::http::Status Status;
        std::shared_ptr<Db> m_database;
    
    public:
        BaseService(std::shared_ptr<Db> database) : m_database(database) {};
};


#endif //BASESERVICE_HPP

