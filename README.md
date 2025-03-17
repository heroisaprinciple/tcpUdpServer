
<h1>Simple TCP Client-Server Implementation</h1>

<p>This is a basic TCP client-server implementation in C that demonstrates socket programming fundamentals. The program can be run in either server or client mode, allowing for simple message exchange between the two.</p>

<h2>Features</h2>
<ul>
    <li>Simple TCP socket communication</li>
    <li>Server that listens for connections</li>
    <li>Client that connects to the server</li>
    <li>Basic message exchange</li>
    <li>Error handling for common socket operations</li>
</ul>

<h2>Requirements</h2>
<ul>
    <li>GCC compiler</li>
    <li>A Linux machine</li>
    <li>Network connectivity between client and server machines</li>
</ul>

<h2>Compilation</h2>
<p>Compile the program using GCC:</p>
<pre><code>gcc -o main main.c</code></pre>

<h2>Usage</h2>
<p>You need to run the program in two separate terminal windows or on two different machines.</p>

<h3>Running the Server</h3>
<p>In the first terminal window, start the server:</p>
<pre><code>./main server</code></pre>
<p>The server will start and listen for incoming connections on port 3490.</p>

<h3>Running the Client</h3>
<p>In the second terminal window, start the client:</p>
<pre><code>./main client</code></pre>
<p>You'll need to modify the IP address in the source code.</p>

<h2>How It Works</h2>
<ol>
    <li>The server initializes a socket, binds to a port, and listens for connections</li>
    <li>The client creates a socket and connects to the server's IP and port</li>
    <li>Once connected, the client sends a message to the server: "Hi, server"</li>
    <li>The server receives the message, displays it, and sends a response: "A message received!"</li>
    <li>The client receives the response and displays it</li>
    <li>Both client and server close their connections</li>
</ol>

<h2>Configuration</h2>
<ul>
    <li><strong>Port</strong>: The default port is set to 3490 (<code>MYPORT</code> macro)</li>
    <li><strong>Backlog</strong>: The connection queue size is set to 10 (<code>BACKLOG</code> macro)</li>
    <li><strong>Buffer Size</strong>: The maximum message size is set to 100 bytes (<code>MAXDATASIZE</code> macro)</li>
</ul>

<h2>Customizing the Implementation</h2>
<p>To change the server IP address that the client connects to, modify the following line in the <code>client()</code> function:</p>
<pre><code>int status = getaddrinfo("[ipAddr]", MYPORT, &hints, &servinfo);</code></pre>
<p>Replace <code>[ipAddr]</code> with the appropriate IP address.</p>

<h2>Notes</h2>
<div class="note">
    <ul>
        <li>This implementation is intended for educational purposes</li>
        <li>It doesn't handle persistent connections or multiple clients</li>
        <li>The client must be started after the server is running</li>
        <li>A 1-second delay is added to the client to ensure the server has time to start</li>
    </ul>
</div>

<h2>Resources</h2>
<ul>
    <li><a href="https://beej.us/guide/bgnet/html//index.html" target="_blank">Beej's Guide to Network Programming</a> - An excellent resource for socket programming in C</li>
</ul>

<h2>Future Plans</h2>
<div class="future-plan">
    <ul>
        <li>Implement support for multiple concurrent clients</li>
        <li>Add a more interactive chat interface with user input</li>
        <li>Create persistent connections that don't terminate after a single message</li>
        <li>Implement a protocol for message formatting</li>
        <li>Add encryption for secure communication</li>
        <li>Develop a more robust error handling system</li>
        <li>Create a configuration file for easy customization</li>
    </ul>
</div>
