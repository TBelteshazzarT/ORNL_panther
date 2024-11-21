#!/usr/bin/env python3

import paramiko

# Create an SSH client
ssh_client = paramiko.SSHClient()
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

# Connect to the server
ssh_client.connect("localhost", 2222, username="user", password="password")

# Create a channel for communication
channel = ssh_client.invoke_shell()

# Send a message to the server
data_sending = "Hello from the client!\n"
data_sending.encode()
channel.send(data_sending)

# Receive a message from the server
data = channel.recv(1024)
print("Received from server:", data.decode())

# Close the channel and the SSH connection
channel.close()
ssh_client.close()