#include "JWT.hpp"


JWT::JWT(const oatpp::String& secret,
         const oatpp::String& issuer)
  : m_secret(secret)
  , m_issuer(issuer)
  , m_verifier(jwt::verify()
               .allow_algorithm(jwt::algorithm::hs256{ secret })
               .with_issuer(issuer))
{}

/* 创建 JWT token */
oatpp::String JWT::createToken(const std::shared_ptr<Payload>& payload) {
  auto token = jwt::create()
    .set_issuer(m_issuer)
    .set_type("JWS")

    .set_payload_claim("userId", jwt::claim(payload->userId_))

    .sign(jwt::algorithm::hs256{m_secret});
  return token;
}

/* 解码 JWT */
std::shared_ptr<JWT::Payload> JWT::readAndVerifyToken(const oatpp::String& token) {

  auto decoded = jwt::decode(token);
  m_verifier.verify(decoded);

  auto payload = std::make_shared<Payload>();
  payload->userId_ = decoded.get_payload_claim("userId").as_string();

  return payload;

}