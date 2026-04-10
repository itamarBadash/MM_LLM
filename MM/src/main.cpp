////////////////////////////////////////////////////////////////////////////////
// MM - Modern C++ Web API
// main.cpp - Crow-based REST API with Ollama Integration
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <fstream>

// Convenience alias for nlohmann::json
using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////////////
// Main Application Entry Point
////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Initialize Crow application with CORS middleware
    crow::App<crow::CORSHandler> app;

    ////////////////////////////////////////////////////////////////////////////
    // Configure CORS Middleware Globally
    // This handles CORS preflight (OPTIONS) requests automatically
    ////////////////////////////////////////////////////////////////////////////

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .headers("X-Custom-Header", "Upgrade-Insecure-Requests", "Content-Type", "Accept", "Origin", "Authorization")
        .methods("POST"_method, "GET"_method, "OPTIONS"_method)
        .origin("*");

    ////////////////////////////////////////////////////////////////////////////
    // POST Route: /api/chat
    // Description: Accepts a JSON payload with a "prompt" field, sends it to
    //              Ollama API for processing, and returns the LLM response.
    //              CORS is handled globally by the middleware, not manually.
    ////////////////////////////////////////////////////////////////////////////

    CROW_ROUTE(app, "/api/chat").methods("POST"_method)
    ([](const crow::request& req) {
        try {
            // Parse incoming JSON payload from request body
            json request_payload = json::parse(req.body);

            // Extract and validate the "prompt" field
            if (!request_payload.contains("prompt")) {
                json error_response = {
                    {"error", "Missing 'prompt' field in request"}
                };
                return crow::response(400, error_response.dump());
            }

            // Extract the prompt string
            std::string user_prompt = request_payload["prompt"].get<std::string>();

            ////////////////////////////////////////////////////////////////////
            // Prepare JSON payload for Ollama API
            // Model: gemma4 (use gemma2 or gemma if not available)
            // Stream: false (synchronous request)
            ////////////////////////////////////////////////////////////////////

            json ollama_payload = {
                {"model", "gemma4"},
                {"prompt", user_prompt},
                {"stream", false}
            };

            ////////////////////////////////////////////////////////////////////
            // Make HTTP POST request to Ollama API
            // Using cpr::Post for synchronous HTTP communication
            ////////////////////////////////////////////////////////////////////

            auto http_res = cpr::Post(
                cpr::Url{"http://localhost:11434/api/generate"},
                cpr::Body{ollama_payload.dump()},
                cpr::Header{{"Content-Type", "application/json"}}
            );

            ////////////////////////////////////////////////////////////////////
            // Check the HTTP response status code from Ollama
            ////////////////////////////////////////////////////////////////////

            if (http_res.status_code != 200) {
                json error_response = {
                    {"error", "Failed to communicate with Ollama API"},
                    {"status_code", http_res.status_code},
                    {"details", http_res.text}
                };
                return crow::response(500, error_response.dump());
            }

            ////////////////////////////////////////////////////////////////////
            // Parse Ollama response and extract the LLM's generated text
            ////////////////////////////////////////////////////////////////////

            json ollama_response = json::parse(http_res.text);

            // Extract the "response" field containing the LLM's text
            if (!ollama_response.contains("response")) {
                json error_response = {
                    {"error", "Invalid response from Ollama API"}
                };
                return crow::response(500, error_response.dump());
            }

            std::string llm_text = ollama_response["response"].get<std::string>();

            ////////////////////////////////////////////////////////////////////
            // Return successful response with LLM's reply
            ////////////////////////////////////////////////////////////////////

            json response_payload = {
                {"reply", llm_text}
            };

            return crow::response(200, response_payload.dump());

        } catch (const json::exception& e) {
            // Handle JSON parsing errors
            json error_response = {
                {"error", "Invalid JSON in request body"}
            };
            return crow::response(400, error_response.dump());

        } catch (const std::exception& e) {
            // Handle any other unexpected exceptions
            json error_response = {
                {"error", "Internal server error"},
                {"details", e.what()}
            };
            return crow::response(500, error_response.dump());
        }
    });

    ////////////////////////////////////////////////////////////////////////////
    // GET Route: /
    // Description: Serves the React frontend's index.html file
    ////////////////////////////////////////////////////////////////////////////

    CROW_ROUTE(app, "/").methods("GET"_method)
    ([]() {
        std::ifstream file("dist/index.html");
        if (!file) {
            return crow::response(404, "File not found");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        crow::response res(200, content);
        res.set_header("Content-Type", "text/html");
        return res;
    });

    ////////////////////////////////////////////////////////////////////////////
    // GET Route: /assets/<path>
    // Description: Serves static assets from the dist/assets/ directory
    ////////////////////////////////////////////////////////////////////////////

    CROW_ROUTE(app, "/assets/<path>").methods("GET"_method)
        ([](const crow::request&, std::string path) {
          std::string filepath = "dist/assets/" + path;
        std::ifstream file(filepath);
        if (!file) {
            return crow::response(404, "File not found");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        crow::response res(200, content);
        // Determine Content-Type based on file extension
        std::string ext = path.substr(path.find_last_of('.') + 1);
        std::string content_type;
        if (ext == "js") content_type = "text/javascript";
        else if (ext == "css") content_type = "text/css";
        else if (ext == "png") content_type = "image/png";
        else if (ext == "jpg" || ext == "jpeg") content_type = "image/jpeg";
        else if (ext == "html") content_type = "text/html";
        else content_type = "application/octet-stream";
        res.set_header("Content-Type", content_type);
        return res;
    });

    ////////////////////////////////////////////////////////////////////////////
    // Server Configuration and Startup
    ////////////////////////////////////////////////////////////////////////////

    // Configure the app to run on port 8080 with multithreading enabled
    app.port(8080)
       .multithreaded()
       .run();

    return 0;
}

