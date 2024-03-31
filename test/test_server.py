import socket

clients = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

clients.connect(('localhost', 8080))

clients.sendall(b'Hello, world!')

response = clients.recv(1024)

print(response)

clients.close()
