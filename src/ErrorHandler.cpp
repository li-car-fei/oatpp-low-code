#include "ErrorHandler.hpp"

// 构造函数，传入 返回数据的序列化器
ErrorHandler::ErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper)
  : m_objectMapper(objectMapper)
{}

std::shared_ptr<ErrorHandler::OutgoingResponse>
ErrorHandler::handleError(const Status& status, const oatpp::String& message, const Headers& headers) {

  // 创建一个 StatusDto Data-transfer-object
  auto error = StatusDto::createShared();
  // 设置它的值
  error->status = "ERROR";
  error->code = status.code;                          // status 是 oatpp::web::protocol::http::Status 当前http请求的状态
  error->message = message;

  // 工厂模式创建返回
  auto response = ResponseFactory::createResponse(status, error, m_objectMapper);

  for(const auto& pair : headers.getAll()) {
    response->putHeader(pair.first.toString(), pair.second.toString());
  }

  return response;

}