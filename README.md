## 👻 Daemon Process in C 🖥️

A simple **daemon process** implemented in **C**, which listens on a local port and logs client interactions. This project demonstrates the creation and management of a daemon process, as well as basic network communication. 🔄

## 📜 Features  

✅ Runs as a **background daemon** 🏴‍☠️  
✅ Listens for **TCP connections** on `localhost:8000`  
✅ Logs all received messages to `daemon.log` 📜  
✅ Gracefully handles the `quit` command to terminate the process 🚪  
✅ Demonstrates **process control** and **network handling**  

## 🛠️ Installation & Usage  

### 📦 Compilation  

```bash
gcc -o daemon main.c
```

### 🚀 Running the Daemon  

```bash
./daemon    # Runs the daemon in the background
```

### 🖥️ Connecting to the Daemon  

Use **Netcat (nc)** to send messages:

```bash
nc localhost 8000
hello
?
quit
```

### 📜 Example Log Output  

After interacting with the daemon, you can check `daemon.log`:

```
Fri Mar 14 16:35:45 2025  [ INFO ]  Started
Fri Mar 14 16:35:45 2025  [ INFO ]  Waiting for client connection
Fri Mar 14 16:36:01 2025  [ LOG  ]  Read: hello
Fri Mar 14 16:36:04 2025  [ LOG  ]  Read: ?
Fri Mar 14 16:36:06 2025  [ LOG  ]  Read: quit
Fri Mar 14 16:36:06 2025  [ INFO ]  Quit after 'quit'
Fri Mar 14 16:36:06 2025  [ INFO ]  Cleaning all
```

## 🎯 How It Works  

[See : daemon](https://en.wikipedia.org/wiki/Daemon_(computing))
1. **Creates a daemon** process by forking and detaching from the terminal.  
2. **Listens on TCP port 8000** for incoming connections.  
3. **Logs all received messages** into `daemon.log`.  
4. **Terminates** when receiving the `quit` command.  

