//
// Created by MECHREVO on 2026/3/9.
//

#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"

LogicSystem::~LogicSystem()
{

}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
    if (_get_handlers.find(path) == _get_handlers.end())
    {
        return false;
    }
    _get_handlers[path](con);
    return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con)
{
    if (_post_handlers.find(path) == _post_handlers.end())
    {
        return false;
    }
    _post_handlers[path](con);
    return true;
}

void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
    _get_handlers.insert(make_pair(url,handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
    _post_handlers.insert(make_pair(url,handler));
}

LogicSystem::LogicSystem()
{
    RegGet("/get_test",[](std::shared_ptr<HttpConnection> connection)
    {
        boost::beast::ostream(connection->_response.body())<<"receive get_text req"<<std::endl;
        int i = 0;
        for (auto& elem : connection->_get_params) {
            i++;
            boost::beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;
            boost::beast::ostream(connection->_response.body()) << ", " <<  " value is " << elem.second << std::endl;
        }
    });

    RegPost("/get_verifycode",[](std::shared_ptr<HttpConnection> connection)
    {
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout<<"receive body is "<<body_str<<std::endl;
        connection->_response.set(boost::beast::http::field::content_type,"text/json");
        boost::property_tree::ptree root;
        boost::property_tree::ptree src_root;
        std::stringstream ss(body_str);

        try {
            // 解析 JSON
            boost::property_tree::read_json(ss, src_root);
        } catch (const boost::property_tree::json_parser_error& e) {
            std::cout << "Failed to parse JSON data: " << e.what() << std::endl;
            root.put("error", ErrorCodes::Error_Json);
            std::stringstream json_stream;
            boost::property_tree::write_json(json_stream, root);
            std::string jsonstr = json_stream.str();
            boost::beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        // 获取 email 字段
        auto email = src_root.get<std::string>("email", "");
        std::cout << "email is " << email << std::endl;
        GetVarifyRsp rsp = VerifyGrpcClient::GetInst()->GetVarifyCode(email);
        // 构建响应
        root.put("error", rsp.error());
        root.put("email", src_root.get<std::string>("email", ""));

        // 将 ptree 转换为 JSON 字符串
        std::stringstream json_stream;
        boost::property_tree::write_json(json_stream, root);
        std::string jsonstr = json_stream.str();

        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    });
}
