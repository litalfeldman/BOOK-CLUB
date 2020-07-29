package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;

public class User {
    private String name;
    private String password;
    private ConcurrentHashMap<Integer,String> id_topic=new ConcurrentHashMap<>();
    private boolean loggedIn;

    public User (String name,String password){
        this.name=name;
        this.password=password;
        this.loggedIn=true;
    }

    public String getPassword(){
        return password;
    }

    public String getName(){
        return name;
    }

    public boolean getloggedIn(){
        return loggedIn;
    }

    public void setLoggedIn(boolean ans){
        loggedIn=ans;
    }

    public void Add(String topic, int topicId){
        id_topic.put(topicId,topic);
    }

    public String getTopic(int topicId) {
        return id_topic.get(topicId);
    }
}

