#ifndef UploadController_HPP
#define UploadController_HPP

#include "oatpp/core/data/stream/FileStream.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

/**
 * Upload file controller
*/
class UploadController : public oatpp::web::server::api::ApiController {
public:
    UploadController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {};

public:
    static std::shared_ptr<UploadController> createShared()
    {
        return std::make_shared<UploadController>();
    };

    
    /**
     * 上传图片的 http 路由 EndPoints
    */
   ENDPOINT_INFO(upload) {
    info->summary = "upload image to parse";
   }
   ENDPOINT("POST", "/upload", upload, REQUEST(std::shared_ptr<IncomingRequest>, request)) {

        oatpp::data::stream::FileOutputStream fileOutputStream(UPLOADFILEPATH);
        request->transferBodyToStream(&fileOutputStream); // transfer body chunk by chunk
        return createResponse(Status::CODE_200, "OK");
    }
};


#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif  /* UploadController_HPP */