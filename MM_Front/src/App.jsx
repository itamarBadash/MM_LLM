import { useState } from 'react';
import ReactMarkdown from 'react-markdown';
import remarkMath from 'remark-math';
import rehypeKatex from 'rehype-katex';
import 'katex/dist/katex.min.css';

function App() {
  const [messages, setMessages] = useState([]);
  const [input, setInput] = useState('');
  const [loading, setLoading] = useState(false);

  const sendMessage = async (prompt) => {
    try {
      const response = await fetch('http://localhost:8080/api/chat', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ prompt }),
      });

      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }

      const data = await response.json();
      return data;
    } catch (error) {
      if (error instanceof TypeError) {
        console.error('Network error - CORS or connectivity issue:', error.message);
      } else if (error instanceof SyntaxError) {
        console.error('JSON parsing error - invalid response format:', error.message);
      } else {
        console.error('API error:', error.message);
      }
      throw error;
    }
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    if (!input.trim()) return;

    const userMessage = { type: 'user', text: input };
    setMessages(prev => [...prev, userMessage]);
    setInput('');
    setLoading(true);

    // Reset textarea height
    const textarea = document.querySelector('.input-form textarea');
    if (textarea) {
      textarea.style.height = 'auto';
    }

    try {
      const data = await sendMessage(input);
      const botMessage = { type: 'bot', text: data.reply };
      setMessages(prev => [...prev, botMessage]);
    } catch (error) {
      const errorMessage = { type: 'bot', text: 'Sorry, something went wrong.' };
      setMessages(prev => [...prev, errorMessage]);
    } finally {
      setLoading(false);
    }
  };

  const handleInputChange = (e) => {
    setInput(e.target.value);
    // Auto-resize textarea
    e.target.style.height = 'auto';
    e.target.style.height = e.target.scrollHeight + 'px';
  };

  const handleKeyDown = (e) => {
    if (e.key === 'Enter' && !e.shiftKey) {
      e.preventDefault();
      handleSubmit(e);
    }
  };

  return (
    <div className="app">
      <div className="messages">
        {messages.map((msg, index) => (
          <div key={index} className={`message ${msg.type}`}>
            {msg.type === 'bot' && <span className="bot-icon">✨</span>}
            <div dir="auto">
              {msg.type === 'bot' ? <ReactMarkdown remarkPlugins={[remarkMath]} rehypePlugins={[rehypeKatex]}>{msg.text}</ReactMarkdown> : msg.text}
            </div>
          </div>
        ))}
        {loading && <div className="message bot loading"><span className="bot-icon">✨</span><div dir="auto">Thinking...</div></div>}
      </div>
      <form className="input-form" onSubmit={handleSubmit}>
        <textarea
          rows={1}
          value={input}
          onChange={handleInputChange}
          onKeyDown={handleKeyDown}
          placeholder="Type your message..."
          disabled={loading}
        />
        <button type="submit" disabled={loading}>Send</button>
      </form>
    </div>
  );
}

export default App;