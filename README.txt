Programs should be compiled without any optimizations. (Makefile accounts for this.)

To use:

1. Run receiver before running the transmitter using the following command line input:
	./receiver.exe transmitter.exe

2. Run transmitter using the following command line input:
	./transmitter.exe <yourmessagehere>
	
where <yourmessagehere> is the message you wish to transmit. By default, the maximum character length is 256. This can be edited by changing the definition of "MAX_MESSAGE_LENGTH" in receiver.c, but given the low bit transmission rate, I would not recommend sending a message this long.

transmitter.exe will send 0xff as a start signal and will print "Start signal transmitted."  after doing so. receiver.exe should print "Start signal received." at the same time. If this does not happen, you can kill transmitter.exe and run it again. (Killing receiver.exe is not necessary if it has not indicated that it has received the start signal.)

At the end of the transmission, transmitter.exe will send 0x00 and terminate. receiver.exe will then print the received message both as characters and as a series of bytes, then terminate.

Acknowledgments:

Assembly code used in receiver.c found here: https://github.com/defuse/flush-reload-attacks

Basic sending strategy (sender continuously does some action, receiver counts cache hits and misses) inspired by: https://github.com/moehajj/Flush-Reload
