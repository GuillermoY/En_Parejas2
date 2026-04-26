# An example of a client/server program

This is a server/client example where the server receives messages, prints them out and responds to the corresponding client with a `"Received"` message. Clients ask the user for a message, send it to the server, and so on (unlike `client_server_1` they do not exist after one message). The `TCP` version must keep track of all connected clients, the `UDP` one does not because anyone can send a message any time.

# How to executes these examples

We will assume that your project is called `TPV2` and is located in `C:\hlocal\TPV2`. Add an example to your project (as they all have a ``main'' method, you can only include one at a time).


- To execute the server, open a console and then type

```
  cd c:
  cd c:\hlocal\TPV2
  set PATH=SDL\bin;%PATH%
  bin\x64\Debug\TPV2.exe server 2000
```

- To execute the client, open another console and the type
  
```
  cd c:
  cd c:\hlocal\TPV2
  set PATH=SDL\bin;%PATH%
  bin\x64\Debug\TPV2.exe client localhost 2000
```
  
- To get usage information execute:

```
  cd c:
  cd c:\hlocal\TPV2
  set PATH=SDL\bin;%PATH%
  bin\x64\Debug\TPV2.exe
```

You can also create a files called `server.BAT` and `client.BAT` with 
the commands above, and then just click on the file in a file explorer.

# How to open a console

- Press `Windows+R` (or openb the `Start` menu and select `Run`)
- Type ``cmd`` in the text field and click ``OK`` (or press ``ENTER``)  


More information: [https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/](https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/).
