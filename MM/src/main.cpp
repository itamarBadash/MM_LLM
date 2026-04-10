////////////////////////////////////////////////////////////////////////////////
// MM - Modern C++ Web API
// main.cpp - Crow-based REST API with Ollama Integration
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

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
    // Server Configuration and Startup
    ////////////////////////////////////////////////////////////////////////////

    // Configure the app to run on port 8080 with multithreading enabled
    app.port(8080)
       .multithreaded()
       .run();

    return 0;
}