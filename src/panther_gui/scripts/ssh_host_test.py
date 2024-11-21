#!/usr/bin/env python3


import socket
import paramiko

# Create an SSH server
ssh_server = paramiko.SSHServer()
ssh_server.set_missing_host_key_policy(paramiko.AutoAddPolicy())

# Generate a temporary SSH key (for demonstration purposes)
key = paramiko.RSAKey.generate(bits=2048)
ssh_server.add_host_key(key)

# Start the server
ssh_server.listen(2222)

while True:
    # Accept a client connection
    client, addr = ssh_server.accept()

    # Create a channel for communication
    channel = client.invoke_shell()

    # Send a message to the client
    channel.send("Hello from the server!\n")

    # Receive a message from the client
    data = channel.recv(1024)
    print("Received from client:", data.decode())

    # Close the channel and the client connection
    channel.close()
    client.close()
