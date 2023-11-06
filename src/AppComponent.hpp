#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "SwaggerComponent.hpp"
#include "DatabaseComponent.hpp"

#include "ErrorHandler.hpp"

#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"
#include "interceptor/AuthInterceptor.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 * OATPP_CREATE_COMPONENT 是一个宏定义：
 * #define OATPP_CREATE_COMPONENT(TYPE, NAME) \
    oatpp::base::Environment::Component<TYPE> NAME = oatpp::base::Environment::Component<TYPE>

    第一项是要创建组件的类型，第二项是创建的组件变量名
    展开后，还要 () 传入构建Component的参数，此处应该需要直接传入构建好的组件进行注册
    因此，此处做法是一个 lamdba 表达式且立即执行，返回新建的组件

    tips: 组件是在环境 oatpp::base::Environment 中创建的，因此后续能够通过 OATPP_COMPONENT 找到并赋予给新的共享指针；
*/

/**
 * OATPP_COMPONENT 是一个宏定义：
 *                  注入组件；创建类型为type，变量名为name的变量，并将已经注册的组件分配给它（都是共享指针）；
*/

/**
 *  在环境中构建各种组件的集合；
 *  如构建TCP连接、构建路由器、构建HTTP Handler、构建服务器等
 * 
 * 使用的宏定义，因此其实是每个组件在 AppComponent 新建对象时，就都新建了一个
 */
class AppComponent {
public:

  /**
   *  Swagger component
   */
  SwaggerComponent swaggerComponent;

  /**
   * Database component
   */
  DatabaseComponent databaseComponent;


  /**
   * 构建 JWT鉴权 所需的 JWT 对象
  */
  OATPP_CREATE_COMPONENT(std::shared_ptr<JWT>, jwt)([]{
    return std::make_shared<JWT>("<my-secret>", "<my-issuer>");
  }());

  /**
   *  构建 handler 中需要使用的 Data-transfer-object 序列化器
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
    return objectMapper;
  }());
  
  /**
   *  构建 TCP连接 组件
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, oatpp::network::Address::IP_4});
  }());
  
  /**
   *  构建 路由器 组件
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  构建 HTTP 请求处理器组件，需要将路由器传入进行初始化
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {

    OATPP_COMPONENT(std::shared_ptr<JWT>, jwt); // get JWT component
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper); // get ObjectMapper component

    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

    /**
     * connectionHandler->setErrorHandler 设置全局的错误处理器
     * 当 service 中的 assert 判断出现错误时，用 ErrorHandler 中的逻辑进行返回
    */
    connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(objectMapper));

    /**
     *  设置全局鉴权拦截器
    */
    connectionHandler->addRequestInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
    connectionHandler->addRequestInterceptor(std::make_shared<AuthInterceptor>(jwt));

    return connectionHandler;

  }());

};

#endif /* AppComponent_hpp */
