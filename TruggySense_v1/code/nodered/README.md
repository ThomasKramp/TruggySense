##### Node-Red 
Node-red is a low-code programming interface for event-driven applications where both a backend and frontend can be developed by creating flow-graphs in combination with custom function codes and an extensive library of modules. I though it could be fun to learn something new here!

###### Installation
The node-red instance will be running locally on the host which is connected to the Teensy 4.1. For this, it must be installed!

1. **Download and install nodejs**
I'll give a demonstration for a Debian 12 instance (my host pc). Nevertheless, you can find [here](https://nodejs.org/en/download) some more reference to install nodejs.
``` bash
# Download and install nvm:
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.2/install.sh | bash

# in lieu of restarting the shell
\. "$HOME/.nvm/nvm.sh"

# Download and install Node.js:
nvm install 23

# Verify the Node.js version:
node -v # Should print "v23.11.0".
nvm current # Should print "v23.11.0".

# Verify npm version:
npm -v # Should print "10.9.2".
```

2. **Download and install node-red through npm (node package manager)**
Node-red can be deployed locally (both Windows and Linux), in a server and even in a docker container! I'll chose the locally -> Linux version. More information can be found [here](https://nodered.org/docs/getting-started/local).

``` bash
sudo npm install -g --unsafe-perm node-red
```

Upon successful installation, the node-red instance can be started with the ``` node-red ```  command. Now, the local instance is running at http://localhost:1880. 

Next, we need to add some extra node-red modules in order to run the flow-graph that we will import later. Go to the running instance at address http://localhost:1880, click on the hamburger menu at the right top, select "Manage Palet" (or "ALT+SHIFT+P"). Then, go to the "Install" tap. Here, we are going to install the following modules:
- ```@flowfuse/node-red-dashboard```
- ```@flowfuse/node-red-dashboard-2-ui-iframe```
- ```node-red-node-serialport```
- ```node-red-contrib-web-worldmap```

Now that all dependencies are installed, we can add the flowgraph by pressing "CTRL+i" where we then select the "flowgraph.json" file in the Tech Report folder under the "node-red" file.
