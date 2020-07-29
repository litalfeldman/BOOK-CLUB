package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    void add(int connectionId, ConnectionHandler<T> handler);

    void add(int connectionId, String destination, int topicId);

    void addUser(int connectionId, String destination, int topicId);

    void deleteUser(int connectionId, int topicId);

    void sendMessage(String frameBody, String destination);

    boolean userExist(int connectionId);

    boolean correctPassword(int connectionId, String password);

    boolean userLoggedIn(int connectionId);

    void connect(int connectionId);

    void addNewUser(int connectionId, String name, String password);
}
