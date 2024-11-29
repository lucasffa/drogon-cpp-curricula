#include "OpenAIClient.h"

#include <drogon/HttpClient.h>
#include <drogon/drogon.h>
#include <json/json.h>

#include <sstream>  // Certifique-se de incluir o cabeçalho correto

std::shared_ptr<OpenAIClient> OpenAIClient::instance = nullptr;

OpenAIClient::OpenAIClient() {
  apiKey = drogon::app().getCustomConfig()["openai"]["api_key"].asString();
  apiUrl = "https://api.openai.com/v1/chat/completions";
}

void OpenAIClient::initialize() {
  if (!instance) {
    instance = std::shared_ptr<OpenAIClient>(new OpenAIClient());
  }
}

std::shared_ptr<OpenAIClient> OpenAIClient::getInstance() { return instance; }

std::string OpenAIClient::generateContent(const std::string& prompt) {
  // Configura o cliente HTTP
  auto client = drogon::HttpClient::newHttpClient("https://api.openai.com");

  // Cria a requisição
  auto req = drogon::HttpRequest::newHttpRequest();
  req->setMethod(drogon::Post);
  req->setPath("/v1/chat/completions");
  req->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
  req->addHeader("Authorization", "Bearer " + apiKey);

  // Monta o corpo da requisição
  Json::Value root;
  root["model"] = "gpt-3.5-turbo";
  root["messages"][0]["role"] = "user";
  root["messages"][0]["content"] = prompt;

  Json::StreamWriterBuilder writer;
  std::string requestBody = Json::writeString(writer, root);
  req->setBody(requestBody);

  // Envia a requisição de forma síncrona (para simplificar)
  auto [result, response] = client->sendRequest(req);

  if (result != drogon::ReqResult::Ok || !response) {
    // Tratar erro de conexão
    return "";
  }

  // Processa a resposta
  std::string responseBody =
      std::string(response->getBody());  // Converte para std::string
  Json::CharReaderBuilder reader;
  Json::Value jsonResponse;
  std::string errs;

  std::istringstream ss(responseBody);  // Agora funciona corretamente
  if (!Json::parseFromStream(reader, ss, &jsonResponse, &errs)) {
    // Tratar erro de parsing JSON
    return "";
  }

  // Extrai o conteúdo gerado
  if (jsonResponse["choices"].isArray() && !jsonResponse["choices"].empty()) {
    std::string content =
        jsonResponse["choices"][0]["message"]["content"].asString();
    return content;
  }

  return "";
}
