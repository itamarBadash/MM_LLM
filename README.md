# Local LLM Chat Interface

A modern, fast, and secure full-stack chat application designed to run Large Language Models (LLMs) locally. This project features a high-performance C++ backend communicating with local Ollama models, paired with a sleek, responsive React frontend inspired by Google Gemini.

## ✨ Features

* **Ultra-Fast Backend:** Built with C++ and the Crow microframework for minimal overhead and maximum performance.
* **Modern UI/UX:** A clean, minimal React interface built with Vite. Features auto-expanding input, smooth scrolling, and mobile responsiveness.
* **Rich Text Rendering:** Full support for Markdown, code blocks, and complex mathematical equations (LaTeX/KaTeX).
* **Multi-Language Support:** Handles RTL (Right-to-Left) languages like Hebrew smoothly with auto-directional text detection.
* **Local & Private:** Zero data leaves your machine. Connects directly to local models (Gemma, Llama 3, etc.) via Ollama.
* **CORS Ready:** Fully configured global middleware handling cross-origin requests securely.

## 🛠️ Tech Stack

**Backend (C++)**
* [Crow](https://crowcpp.org/) - C++ Microframework for the web
* [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++
* [cpr](https://docs.libcpr.org/) - C++ Requests (HTTP client)
* CMake & CLion

**Frontend (React)**
* [React 18](https://react.dev/) & [Vite](https://vitejs.dev/)
* `react-markdown` - For parsing LLM text formatting
* `rehype-katex` & `remark-math` - For academic and mathematical LaTeX rendering

**AI Engine**
* [Ollama](https://ollama.ai/) running `gemma4` (or any configured model)

## 🚀 Getting Started

### Prerequisites
1. **Ollama:** Installed and running locally. Pull your preferred model (e.g., `ollama run gemma4`).
2. **C++ Environment:** CMake and a modern C++ compiler (MSVC, GCC, or Clang).
3. **Node.js:** Installed for the frontend environment.

### Running the Backend
1. Open the project in CLion (or your preferred C++ IDE).
2. Build the project using CMake.
3. Run the executable (`MM.exe`). The server will start on `http://localhost:8080`.

### Running the Frontend
1. Open a terminal and navigate to the frontend directory:
   ```bash
   cd chat-frontend