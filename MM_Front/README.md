# LLM Chat App Frontend

A React frontend for a local LLM chat application built with Vite.

## Setup

1. Install dependencies:
   ```bash
   npm install
   ```

2. Start the development server:
   ```bash
   npm run dev
   ```

3. Open your browser to `http://localhost:5173` (or the port shown in the terminal).

## Requirements

- Node.js (version 16 or higher)
- A backend server running on `http://localhost:8080` that accepts POST requests to `/api/chat` with JSON body `{"prompt": "user input"}` and returns `{"reply": "bot response"}`.

## Usage

- Type your message in the input field at the bottom.
- Press Enter or click Send to submit.
- Your message appears instantly, followed by the bot's response.

## Troubleshooting

- If the app doesn't load, ensure dependencies are installed with `npm install`.
- If chat doesn't work, check that the backend is running on `http://localhost:8080/api/chat`.
- For build issues, run `npm run build` to check for errors.
- Lint with `npm run lint` to check code quality.