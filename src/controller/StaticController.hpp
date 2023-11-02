
#ifndef CRUD_STATICCONTROLLER_HPP
#define CRUD_STATICCONTROLLER_HPP

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

/**
 * 静态资源返回的 Controller
*/
class StaticController : public oatpp::web::server::api::ApiController {
public:
  /**
   * 传入返回数据 序列化器进行构建
  */
  StaticController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:

  /**
   * 静态函数：
   * 定义 createShared 函数，返回 StaticController 实例
   * OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) 从环境中找到序列化器作为函数的默认参数
   * createShared 函数内部通过传入序列化器构建一个指向StaticController的共享指针，并返回
  */
  static std::shared_ptr<StaticController> createShared(
    OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
  ){
    return std::make_shared<StaticController>(objectMapper);
  }

  /**
   * EndPoint 路由
  */
  ENDPOINT("GET", "/", root) {
    const char* html =
      "<html lang='en'>"
      "  <head>"
      "    <meta charset=utf-8/>"
      "  </head>"
      "  <body>"
      "    <p>Hello carfied's low-code development backend project!</p>"
      "    <a href='swagger/ui'>Checkout Swagger-UI page</a>"
      "  </body>"
      "</html>";
    auto response = createResponse(Status::CODE_200, html);
    /**
     * Header 中指明 response类型
    */
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }

};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif //CRUD_STATICCONTROLLER_HPP
