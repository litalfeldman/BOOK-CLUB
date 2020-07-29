package bgu.spl.net.impl.stomp;

//import bgu.spl.net.api.MessageEncoderDecoder;
//import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
//import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        int port = Integer.parseInt(args[0]);
        if (args[1].equals("tpc")) {
            Server.threadPerClient(
                    port, //port
                    () -> new StompMessagingProtocolImpl(), //protocol factory
                    () -> new MessageEncoderDecoderImpl() //message encoder decoder factory
            ).serve();
        } else {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    port, //port
                    StompMessagingProtocolImpl::new, //protocol factory
                    MessageEncoderDecoderImpl::new //message encoder decoder factory
            ).serve();
        }
    }
}


