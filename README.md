pong-rpi-client

This repo servers as a Raspberry Pi pong client that is to be used for cloud pong.
A cloud pong server will be later implemented and will this program will send controller
input data out to the cloud pong server. The new input will update the game state and the
UI will be streamed and the client will receive encoded frames, decode and render to the user.
This will allow for multiplayer pong where two Raspberry pi clients play against each other and connect to the server.

# BUILDING
git clone https://github.com/a6guerre/pong-rpi-client
cd pong-rpi-client
make

# RUNNING
./pong-rpi-cient

# Note.
Currently only horizontal axis of left joystick is mapped since this the one to be used.
I am using a XBOX controller in PC mode.
In the future, support for more controllers can be added.
