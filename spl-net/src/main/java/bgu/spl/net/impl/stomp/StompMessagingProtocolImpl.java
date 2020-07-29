package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

import java.util.LinkedList;
import java.util.List;


public class StompMessagingProtocolImpl implements StompMessagingProtocol<Frame> {
    private int connectionId;
    private ConnectionsImpl connections;
    private boolean shouldTerminate;

    @Override
    public void start(int connectionId, Connections<Frame> connections) {
        this.connectionId = connectionId;
        this.connections = (ConnectionsImpl) connections;
        this.shouldTerminate = false;
    }

    @Override
    public void process(Frame message) {
        String command = message.getCommand();
        if (command.equals("CONNECT")) {
            String password = "";
            String name = "";
            String acceptVersion = "";
            List<Header> headers = message.getHeaders();
            for (Header header : headers) {
                if (header.getName().equals("passcode")) {
                    password = header.getValue();
                }
                if (header.getName().equals("login")) {
                    name = header.getValue();
                }
                if (header.getName().equals("accept-version")) {
                    acceptVersion = header.getValue();
                }
            }
            if (connections.userExist(connectionId)) {
                //user is already exist
                if (connections.userLoggedIn(connectionId)) {
                    Header errorHeader = new Header("message", "User already logged in");
                    List<Header> headersToSend = new LinkedList<>();
                    headersToSend.add(errorHeader);
                    Frame frame = new Frame("ERROR", headersToSend, "");
                    connections.send(connectionId, frame);
                }
                //incorrect password
                else if (connections.correctPassword(connectionId, password)) {
                    connections.connect(connectionId);
                } else {
                    Header errorHeader = new Header("message", "Wrong password");
                    List<Header> headersToSend = new LinkedList<>();
                    headersToSend.add(errorHeader);
                    Frame frame = new Frame("ERROR", headersToSend, "");
                    connections.send(connectionId, frame);
                }
            } else {//the user not exist
                connections.addNewUser(connectionId, name, password);
                Header errorHeader = new Header("version", acceptVersion);
                List<Header> headersToSend = new LinkedList<>();
                headersToSend.add(errorHeader);
                Frame frame = new Frame("CONNECTED", headersToSend, "");
                connections.send(connectionId, frame);
            }
        }


        if (command.equals("SEND")) {
            String destination = "";
            List<Header> headers = message.getHeaders();
            for (Header header : headers) {
                if (header.getName().equals("destination")) {
                    destination = header.getValue();
                }
            }
            connections.sendMessage(message.getFrameBody(), destination);


        } else if (command.equals("SUBSCRIBE")) {
            String destination = "";
            int topicId = 0;
            String receipt = "";
            List<Header> headers = message.getHeaders();
            for (Header header : headers) {
                if (header.getName().equals("destination")) {
                    destination = header.getValue();
                }
                if (header.getName().equals("id")) {
                    topicId = Integer.parseInt(header.getValue());
                }
                if (header.getName().equals("receipt")) {
                    receipt = header.getValue();
                }
            }
            connections.add(connectionId, destination, topicId);
            connections.addUser(connectionId, destination, topicId);
            //send a receipt frame to the client
            Header receiptHeader = new Header("receipt-id", receipt);
            List<Header> headersToSend = new LinkedList<>();
            headersToSend.add(receiptHeader);
            Frame frame = new Frame("RECEIPT", headersToSend, "");
            connections.send(connectionId, frame);


        } else if (command.equals("UNSUBSCRIBE")) {
            int topicId = 0;
            String receipt = "";
            List<Header> headers = message.getHeaders();
            for (Header header : headers) {
                if (header.getName().equals("id")) {
                    topicId = Integer.parseInt(header.getValue());
                }
                if (header.getName().equals("receipt")) {
                    receipt = header.getValue();
                }
            }
            connections.deleteUser(connectionId, topicId);
            //  send a receipt frame to the client
            Header receiptHeader = new Header("receipt-id", receipt);
            List<Header> headersToSend = new LinkedList<>();
            headersToSend.add(receiptHeader);
            Frame frame = new Frame("RECEIPT", headersToSend, "");
            connections.send(connectionId, frame);


        } else if (command.equals("DISCONNECT")) {
            String receipt = "";
            List<Header> headers = message.getHeaders();
            for (Header header : headers) {
                if (header.getName().equals("receipt")) {
                    receipt = header.getValue();
                }
            }
            //  send a receipt frame to the client
            Header receiptHeader = new Header("receipt-id", receipt);
            List<Header> headersToSend = new LinkedList<>();
            headersToSend.add(receiptHeader);
            Frame frame = new Frame("RECEIPT", headersToSend, "");

            connections.send(connectionId, frame);
            connections.disconnect(connectionId);

            shouldTerminate = true;
        }

    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
