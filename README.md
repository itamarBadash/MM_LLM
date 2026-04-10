# MM - Local LLM Chat Application

A high-performance, privacy-first full-stack chat application for interacting with Large Language Models (LLMs) running locally via Ollama. Built with a modern C++ REST API backend and a responsive React frontend, designed for seamless local inference without any data transmission to external services.

## 📋 Overview

MM provides a complete solution for running private LLM conversations locally. The backend implements a lightweight HTTP API using the Crow framework, handling communication between the React frontend and Ollama. The frontend delivers a clean, intuitive chat interface with support for rich text formatting, mathematical expressions, and multi-language support.

**Key Characteristics:**
- **Zero External Dependencies:** All processing happens locally; no data leaves your system
- **High Performance:** Optimized C++ backend with minimal latency and efficient resource utilization
- **Production Ready:** Comprehensive error handling, CORS support, and proper HTTP semantics
- **Developer Friendly:** Clean code structure, well-documented build system, and unit test framework

## ✨ Core Features

### Backend
- **REST API Architecture:** Clean, RESTful endpoint design with proper HTTP status codes and JSON responses
- **CORS Support:** Global CORS middleware with configurable origin handling for cross-origin requests
- **Error Handling:** Comprehensive error responses with meaningful status codes (400, 500, etc.)
- **Multi-threaded:** Built-in support for concurrent request handling with thread-safe operations
- **Static Asset Serving:** Integrated serving of React frontend assets (HTML, CSS, JavaScript)

### Frontend
- **React 18 Architecture:** Modern functional components with React Hooks for state management
- **Auto-expanding Input:** Responsive textarea that automatically adjusts height as you type
- **Markdown Rendering:** Full Markdown support using `react-markdown` for formatted responses
- **LaTeX/KaTeX Support:** Mathematical equations rendered beautifully using `rehype-katex` and `remark-math`
- **Multi-language Support:** Automatic text direction detection (`dir="auto"`) for RTL languages (Arabic, Hebrew, etc.)
- **Loading States:** Visual feedback with animated "Thinking..." indicator during API calls
- **Keyboard Shortcuts:** Enter to send, Shift+Enter for new line

## 🛠️ Technology Stack

### Backend (C++17)
| Component | Version | Purpose |
|-----------|---------|---------|
| [Crow](https://crowcpp.org/) | v1.2.0 | HTTP web framework and routing |
| [nlohmann/json](https://github.com/nlohmann/json) | v3.11.3 | JSON serialization/deserialization |
| [cpr](https://github.com/libcpr/cpr) | v1.10.5 | HTTP client for Ollama communication |
| [Asio](https://github.com/chriskohlhoff/asio) | v1.28.0 | Network I/O and async operations |
| [GoogleTest](https://github.com/google/googletest) | v1.14.0 | Unit testing framework |
| CMake | 3.14+ | Build system |

### Frontend (JavaScript/React)
| Component | Version | Purpose |
|-----------|---------|---------|
| [React](https://react.dev/) | 18.2.0 | UI library and component framework |
| [Vite](https://vitejs.dev/) | 5.0.8 | Build tool and dev server |
| [react-markdown](https://github.com/remarkjs/react-markdown) | 10.1.0 | Markdown-to-JSX renderer |
| [remark-math](https://github.com/remarkjs/remark-math) | 6.0.0 | Math expression parser |
| [rehype-katex](https://github.com/remarkjs/rehype-katex) | 7.0.1 | LaTeX equation rendering |
| ESLint | 8.55.0 | Code linting and quality |

### AI Runtime
- [Ollama](https://ollama.ai/) - Local LLM inference engine (any compatible model)
- Tested with: Gemma, Llama 2/3, Mistral, Neural Chat, and other GGUF-format models

## 📁 Project Structure

```
MM_LLM/
├── MM/                           # C++ Backend
│   ├── src/
│   │   └── main.cpp             # Crow REST API implementation
│   ├── tests/
│   │   └── test_main.cpp        # Unit tests
│   └── CMakeLists.txt           # CMake build configuration
│
├── MM_Front/                     # React Frontend
│   ├── src/
│   │   ├── App.jsx              # Main chat component
│   │   ├── App.css              # Styling
│   │   └── main.jsx             # React entry point
│   ├── index.html               # HTML template
│   ├── vite.config.js           # Vite configuration
│   └── package.json             # Dependencies
│
└── README.md                     # This file
```

## 🚀 Getting Started

### System Requirements

- **Operating System:** Windows, macOS, or Linux
- **C++ Compiler:** MSVC 2019+, GCC 9+, or Clang 10+
- **Build Tools:** CMake 3.14 or higher
- **Runtime:** Node.js 16+ (frontend), Python 3.8+ (for Ollama)
- **Memory:** 4GB minimum (8GB+ recommended for larger models)

### Prerequisites Installation

#### Windows
```powershell
# Install CMake (if not already installed)
# Download from https://cmake.org/download/

# Install Node.js
# Download from https://nodejs.org/

# Install Ollama
# Download from https://ollama.ai/
```

#### macOS
```bash
# Install CMake, Node.js, and Ollama via Homebrew
brew install cmake node
# Download Ollama from https://ollama.ai/
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install cmake build-essential nodejs
# Download Ollama from https://ollama.ai/
```

### Step 1: Start Ollama

Before running the application, ensure Ollama is running with a model loaded:

```bash
# Start Ollama service
ollama serve

# In another terminal, pull a model (one-time)
ollama pull gemma      # ~5GB
# or
ollama pull neural-chat  # ~4GB
# or any other available model
```

Verify Ollama is running:
```bash
curl http://localhost:11434/api/tags
```

### Step 2: Build and Run the Backend

#### Option A: Using CLion (Recommended)
1. Open the `MM` folder in CLion
2. CMake will automatically configure dependencies via FetchContent
3. Click **Build** → **Build 'MM'**
4. Click **Run** to start the server on `http://localhost:8080`

#### Option B: Command Line
```powershell
cd MM
mkdir cmake-build-release
cd cmake-build-release
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
.\bin\MM.exe      # Windows
# or
./bin/MM          # macOS/Linux
```

**Expected Output:**
```
Starting Crow server on 0.0.0.0:8080
(Crow v1.2.0)
```

### Step 3: Build and Run the Frontend

```bash
cd MM_Front

# Install dependencies
npm install

# Development mode with hot reload
npm run dev

# Or build for production
npm run build
npm run preview
```

The frontend will be available at `http://localhost:5173` in development, or integrated into the backend at `http://localhost:8080` after building.

### Step 4: Access the Application

1. Open your browser and navigate to `http://localhost:8080`
2. Type a message and press Enter to chat
3. The backend will forward your message to the local Ollama instance and stream the response

## 🔌 API Documentation

### POST /api/chat

Send a prompt to the LLM and receive a response.

**Request:**
```json
{
  "prompt": "What is machine learning?"
}
```

**Response (Success - 200 OK):**
```json
{
  "reply": "Machine learning is a subset of artificial intelligence that enables systems to learn and improve from experience without being explicitly programmed..."
}
```

**Response (Error - 400 Bad Request):**
```json
{
  "error": "Missing 'prompt' field in request"
}
```

**Response (Error - 500 Internal Server Error):**
```json
{
  "error": "Failed to communicate with Ollama API",
  "status_code": 500,
  "details": "Connection refused"
}
```

### GET /

Serves the React frontend (index.html and assets).

### GET /assets/<path>

Serves static frontend assets (CSS, JavaScript, images).

## 🧪 Testing

### Run Backend Tests

```powershell
cd MM\cmake-build-release
ctest --verbose
# or
.\bin\MM_tests.exe
```

## 📝 Configuration

### Backend Configuration

Edit `MM/src/main.cpp` to modify:
- **Port:** Line 188 - `app.port(8080)`
- **Ollama Endpoint:** Line 78 - `cpr::Url{"http://localhost:11434/api/generate"}`
- **LLM Model:** Line 67 - `{"model", "gemma4"}`
- **Streaming:** Line 69 - `{"stream", false}` (set to `true` for streaming responses)
- **CORS Origins:** Line 34 - `.origin("*")` (restrict to specific domains in production)

### Frontend Configuration

Edit `MM_Front/vite.config.js` for Vite options and `MM_Front/src/App.jsx` for:
- **API Endpoint:** Line 14 - `fetch('http://localhost:8080/api/chat', ...)`
- **Placeholder Text:** Line 100 - `placeholder="Type your message..."`

## 🛡️ Security Considerations

1. **CORS Configuration:** Currently configured with `origin("*")`. For production:
   ```cpp
   .origin("https://yourdomain.com")  // Restrict to specific origin
   ```

2. **Input Validation:** Backend validates JSON structure and required fields

3. **Error Handling:** Sensitive error details are logged but not exposed to clients in some cases

4. **Local Execution:** No external API calls or data transmission (except to local Ollama)

## 🚀 Performance Optimization

### Backend
- **Multi-threading:** Enabled with `.multithreaded()`
- **Static Linking:** All dependencies statically linked for portability
- **C++17 Optimizations:** Modern C++ features for efficiency

### Frontend
- **Code Splitting:** Vite automatically optimizes bundle
- **Lazy Loading:** React components load on demand
- **Asset Optimization:** Automatic minification and compression in production build

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| **"Connection refused" to Ollama** | Ensure Ollama is running: `ollama serve` in another terminal |
| **CORS errors in browser console** | Verify backend is running on `http://localhost:8080` |
| **Frontend shows "Network error"** | Check API endpoint in `App.jsx` and ensure backend is accessible |
| **"Model not found" error** | Pull the model: `ollama pull gemma` (or your preferred model) |
| **High latency responses** | Larger models have slower inference; use smaller models for speed |
| **Build failures on Windows** | Ensure Visual Studio C++ build tools are installed |
| **Build failures on macOS** | Install Xcode Command Line Tools: `xcode-select --install` |

## 📚 Additional Resources

- [Crow Documentation](https://crowcpp.org/master/)
- [Ollama Models](https://ollama.ai/library)
- [React Documentation](https://react.dev/)
- [Vite Guide](https://vitejs.dev/guide/)
- [nlohmann/json API](https://nlohmann.github.io/json/)
- [KaTeX Documentation](https://katex.org/)

