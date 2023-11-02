#ifndef ComponentDto_hpp
#define ComponentDto_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 组件类别
*/
ENUM(ComponentType, v_int32,
        VALUE(accordion, 0, "accordion"),
        VALUE(calendar, 1, "calendar"),
        VALUE(card, 2, "card"),
        VALUE(carousel, 3, "carousel"),
        VALUE(horizontalList, 4, "horizontalList"),
        VALUE(image, 5, "image"),
        VALUE(imageArea, 6, "imageArea"),
        VALUE(list, 7, "list"),
        VALUE(navBar, 8, "navBar"),
        VALUE(noticeBar, 9, "noticeBar"),
        VALUE(picker, 10, "picker"),
        VALUE(richText, 11, "richText"),
        VALUE(whiteSpace, 12, "whiteSpace")
)


/**
 * 组件内部 子组件 的 前后端交互的 DTO
*/
class ChildComponentTranserDto : public oatpp::DTO {
  
  DTO_INIT(ChildComponentTranserDto, DTO)

  DTO_FIELD(Int32, record_id);
  DTO_FIELD(Int32, component_id);
  DTO_FIELD(Int32, child_components_index);
  DTO_FIELD(String, description_);
  DTO_FIELD(String, title);
  /**
   * 前端传过来，以及传出去都设置是 bool 型
   * 在 service 里进行一个转换
  */
  DTO_FIELD(Boolean, clickable, "clickable");
  DTO_FIELD(String, src, "src");
  DTO_FIELD(String, link, "link");
  DTO_FIELD(String, text_, "text_");
  DTO_FIELD(String, content, "content");
};

/**
 * 组件内部 子组件 的 数据库中定义的 DTO
*/
class ChildComponentRecordDto : public oatpp::DTO {
  
  DTO_INIT(ChildComponentRecordDto, DTO)

  DTO_FIELD(Int32, record_id);
  DTO_FIELD(Int32, component_id);
  DTO_FIELD(Int32, child_components_index);
  DTO_FIELD(String, description_);
  DTO_FIELD(String, title);
  /**
   * 数据库内的 clickable 是 INTEGER类型的
  */
  DTO_FIELD(Int32, clickable, "clickable");
  DTO_FIELD(String, src, "src");
  DTO_FIELD(String, link, "link");
  DTO_FIELD(String, text_, "text_");
  DTO_FIELD(String, content, "content");
};


/**
 * 组件记录的 DTO
*/
class ComponentRecordDto : public oatpp::DTO {
  
  DTO_INIT(ComponentRecordDto, DTO)

  DTO_FIELD(Int32, record_id);
  DTO_FIELD(Int32, project_id, "project_id");
  DTO_FIELD(Enum<ComponentType>::AsString, component_type);
  DTO_FIELD(Int32, components_index);
  DTO_FIELD(String, currentDate);
  DTO_FIELD(String, header, "header");
  DTO_FIELD(String, title, "title");
  DTO_FIELD(String, content, "content");
  DTO_FIELD(String, mode, "mode");
  DTO_FIELD(String, src, "src");
  DTO_FIELD(String, link, "link");
  DTO_FIELD(String, label1, "label1");
  DTO_FIELD(String, label2, "label2");
  DTO_FIELD(String, backgroundColor, "backgroundColor");
};

/**
 * 组件记录 包括了子组件数组 数据 的 DTO
*/
class ComponentTransferDto : public ComponentRecordDto {
  DTO_INIT(ComponentTransferDto, ComponentRecordDto)

  DTO_FIELD(Vector<oatpp::Object<ChildComponentTranserDto>>, childs, "childs");
};




#include OATPP_CODEGEN_END(DTO)

#endif /* ComponentDto_hpp */
