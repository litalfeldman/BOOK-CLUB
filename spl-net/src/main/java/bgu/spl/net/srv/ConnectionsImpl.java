package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.Frame;
import bgu.spl.net.impl.stomp.Header;
import bgu.spl.net.impl.stomp.Pair;
import bgu.spl.net.impl.stomp.User;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl implements Connections<Frame> {

    private ConcurrentHashMap<String, List<Pair<Integer,Integer>>> topic_connectionId_idTopic=new ConcurrentHashMap<>();
    private ConcurrentHashMap<Integer, List<String>> connectionId_topic=new ConcurrentHashMap<>();
    private ConcurrentHashMap<Integer, User> connectionId_user=new ConcurrentHashMap<>();
    private ConcurrentHashMap<Integer,ConnectionHandler<Frame>> connectionId_connectionHandler=new ConcurrentHashMap<>();
    private AtomicInteger id=new AtomicInteger(0);

    @Override
    public boolean send(int connectionId, Frame msg) {
        if (connectionId_connectionHandler.containsKey(connectionId)) {
            try {
                System.out.println("server sending message:\n" + msg);
                connectionId_connectionHandler.get(connectionId).send(msg);
            } catch (IOException e) {
                e.printStackTrace()
                ;
            }
            return true;
        }
        return false;
    }

    @Override
    public void send(String channel, Frame msg) {

        try {
            if (topic_connectionId_idTopic.containsKey(channel)) {
                List<Pair<Integer,Integer>> listId=topic_connectionId_idTopic.get(channel);
                for (Pair<Integer,Integer> client : listId) {
                    int connectionId=client.getFirst();
                    connectionId_connectionHandler.get(connectionId).send(msg);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void disconnect(int connectionId) {//delete the client from the maps
        try {
            if (connectionId_topic.containsKey(connectionId)) {
                List<String> topicList = connectionId_topic.get(connectionId);
                connectionId_topic.remove(connectionId);
                for (String topic:topicList) {
                    List<Pair<Integer,Integer>> list=topic_connectionId_idTopic.get(topic);
                    for(Pair<Integer,Integer> pair: list ){
                        if(pair.getFirst().equals(connectionId)){
                            list.remove(pair);
                        }
                    }
                }
            }
            if (connectionId_connectionHandler.containsKey(connectionId)) {
                connectionId_connectionHandler.get(connectionId).close();
                connectionId_connectionHandler.remove(connectionId);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }



    @Override
    public void add(int connectionId, ConnectionHandler<Frame> handler) {
        connectionId_connectionHandler.put(connectionId,handler);
    }


    public void add(int connectionId, String topic,int topicId) {
        if (topic_connectionId_idTopic.containsKey(topic)) {
            List<Pair<Integer,Integer>> subs = topic_connectionId_idTopic.get(topic);
            Pair<Integer,Integer> pair= new Pair<>(connectionId,topicId);
            subs.add(pair);
        } else {
            List<Pair<Integer,Integer>> subs = new LinkedList<>();
            Pair<Integer,Integer> pair= new Pair<>(connectionId,topicId);
            subs.add(pair);
            topic_connectionId_idTopic.put(topic, subs);
        }

        if (connectionId_topic.containsKey(connectionId)) {
            List<String> topics = connectionId_topic.get(connectionId);
            topics.add(topic);
        } else {
            List<String> topics = new LinkedList<>();
            topics.add(topic);
            connectionId_topic.put(connectionId, topics);
        }
    }

    public void addUser(int connectionId, String topic,int topicId) {
        User user= connectionId_user.get(connectionId);
        user.Add(topic, topicId);
    }

    public void deleteUser(int connectionId, int topicId){
        User user= connectionId_user.get(connectionId);
        String topic= user.getTopic(topicId);

        List<Pair<Integer,Integer>> connectionIdList = topic_connectionId_idTopic.get(topic);
        Pair<Integer,Integer> pair= new Pair<>(connectionId,topicId);
        connectionIdList.remove(pair);

        List<String> topics = connectionId_topic.get(connectionId);
        topics.remove(topic);

        user.setLoggedIn(false);//the user is disconnected
    }

    //creates a message frame for each client and sends it to the handler
    public void sendMessage(String frameBody, String destination) {
        List<Pair<Integer,Integer>> connectionIdList = topic_connectionId_idTopic.get(destination);
        Frame frame;
//        List<Header> headers = new LinkedList<>();
        Header subscription;
        Header destinationHeader = new Header("destination", destination);
        Header messageId = new Header("messageId", this.id.toString());


        for (Pair<Integer,Integer> pair : connectionIdList){
            List<Header> headers = new LinkedList<>();
            subscription = new Header("subscription", pair.getSecond().toString());
            headers.add(subscription);
            headers.add(messageId);
            headers.add(destinationHeader);

            frame = new Frame("MESSAGE", headers, frameBody);
            this.send(pair.getFirst(), frame);
        }
    }

    public boolean userExist(int connectionId){
       return connectionId_user.containsKey(connectionId);
    }

    public boolean correctPassword(int connectionId, String password) {
        User user = connectionId_user.get(connectionId);
        return user.getPassword().equals(password);
    }

    public boolean userLoggedIn(int connectionId){
        return connectionId_user.get(connectionId).getloggedIn();
    }

    public void connect(int connectionId) {
        connectionId_topic.put(connectionId,new LinkedList<>());
    }

    @Override
    public void addNewUser(int connectionId, String name, String password) {
        User user= new User(name,password);
        this.connect(connectionId);
        connectionId_user.put(connectionId,user);
    }

}
