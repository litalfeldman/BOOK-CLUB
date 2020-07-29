//package bgu.spl.net.impl.rci;
//
//import bgu.spl.net.api.MessagingProtocol;
//import bgu.spl.net.api.StompMessagingProtocol;
//import bgu.spl.net.srv.Connections;
//
//import java.io.Serializable;
//
//public class RemoteCommandInvocationProtocol<T> implements StompMessagingProtocol<Serializable> {
//
//    private T arg;
//
//    public RemoteCommandInvocationProtocol(T arg) {
//        this.arg = arg;
//    }
//
//
//    @Override
//    public void start(int connectionId, Connections<Serializable> connections) {
//
//    }
//
//    @Override
//    public void process(Serializable message) {
//
//    }
//
//    @Override
//    public boolean shouldTerminate() {
//        return false;
//    }
//}
