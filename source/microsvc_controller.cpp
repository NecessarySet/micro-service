//
//  Created by Ivan Mejia on 12/24/16.
//
// MIT License
//
// Copyright (c) 2016 ivmeroLabs.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <std_micro_service.hpp>
#include "microsvc_controller.hpp"
#include "sys_info.hpp"
#include "proto/status.pb.h"  //protocol buffer data definition

using namespace web;
using namespace http;

void MicroserviceController::initRestOpHandlers() {
    _listener.support(methods::GET, std::bind(&MicroserviceController::handleGet, this, std::placeholders::_1));
    _listener.support(methods::PUT, std::bind(&MicroserviceController::handlePut, this, std::placeholders::_1));
    _listener.support(methods::POST, std::bind(&MicroserviceController::handlePost, this, std::placeholders::_1));
    _listener.support(methods::DEL, std::bind(&MicroserviceController::handleDelete, this, std::placeholders::_1));
    _listener.support(methods::PATCH, std::bind(&MicroserviceController::handlePatch, this, std::placeholders::_1));
}

json::value GetComplexObject(std::string name) {
	auto person = json::value::object();
        auto address = json::value::object();
        person["name"] = json::value::string(name);
        address["street"] = json::value::string("848 Brunswick Street");
        address["postal code"] = json::value::string("E3B1J1");
        person["address"] = address;

	return person;
}

void MicroserviceController::handleGet(http_request message) {
    
    http_response response(status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));

    auto path = requestPath(message);
    if (!path.empty()) {

	auto output = json::value::object();
	response.set_status_code(status_codes::OK);

        if (path[0] == "service" && path[1] == "test") {
            output["version"] = json::value::string("0.1.1");
            output["status"] = json::value::string("ready!");
        }
        if (path[0] == "data") {
		if(path[1] == "status") {
			float cpuLoad = Sysinfo::GetCPULoad();
			float memoryLoad = Sysinfo::GetSystemMemoryUsagePercentage();
			output["CPU"] = json::value::number(cpuLoad);
			output["Memory"] = json::value::number(memoryLoad);
		}
		else if(path[1] == "number") {
            		output["data"] = json::value::number(3.414);
		}
		else if(path[1] == "string") {
            		output["header"] = json::value::string("A test string");
		}
		else if(path[1] == "object") {
            		output = GetComplexObject("Mark Masry"); 
		}
		else if(path[1] == "array" ) {
			
			std::vector<web::json::value> arrayNames;
			for(int i=0;i<10;i++) {
				arrayNames.push_back(GetComplexObject("Mark"));
			}
			output = json::value::array(arrayNames);
		}
		else {
			response.set_status_code(status_codes::NotFound);
		}

        }

	response.set_body(output);	
    }
    else {
	response.set_status_code(status_codes::NotFound);
    }
    
    message.reply(response);
}

void MicroserviceController::handlePatch(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::PATCH));
}

void MicroserviceController::handlePut(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::PUT));
}

void MicroserviceController::handlePost(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::POST));
}

void MicroserviceController::handleDelete(http_request message) {    
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::DEL));
}

void MicroserviceController::handleHead(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::HEAD));
}

void MicroserviceController::handleOptions(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::OPTIONS));
}

void MicroserviceController::handleTrace(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::TRCE));
}

void MicroserviceController::handleConnect(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::CONNECT));
}

void MicroserviceController::handleMerge(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::MERGE));
}

json::value MicroserviceController::responseNotImpl(const http::method & method) {
    auto response = json::value::object();
    response["serviceName"] = json::value::string("C++ Mircroservice Sample");
    response["http_method"] = json::value::string(method);
    return response ;
}
