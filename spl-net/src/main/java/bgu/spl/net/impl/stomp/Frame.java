package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;

import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class Frame {

//    public static void main(String[] args) {
//        String msg="SUBSCRIBE\ndestination:sci-fi\nid:78\nreceipt:77\n\n\0";
//        byte[] bytes=msg.getBytes();
//        MessageEncoderDecoderImpl p=new MessageEncoderDecoderImpl();
//        Frame frame=null;
//
//        for (byte aByte : bytes) {
//            frame=p.decodeNextByte(aByte);
//        }
//        int g=0;
//    }

    private String Command;
    private List<Header> headers;
    private String frameBody;

    public Frame(String Command, List<Header> headers, String frameBody) {
        this.Command = Command;
        this.headers = headers;
        this.frameBody = frameBody;
    }

    public List<Header> getHeaders() {
        return headers;
    }

    public String getCommand() {
        return Command;
    }

    public String getFrameBody() {
        return frameBody;
    }

    @Override
    public String toString() {
        return frameToString();
    }

    public String frameToString() {
        String output = Command + "\n";
        String headerLine = "";

        for (Header header : this.headers) {
            headerLine = header.getName() + ":" + header.getValue() + "\n";
            output += headerLine;
        }
        output += "\n";
        if (!frameBody.equals("")) {

            output += frameBody + "\n";
        }
        output += "\u0000";

        return output;
    }


    public static Frame stringToFrame(String decodedString) {
        String commandLine = "";
        List<Header> headersList = new LinkedList<>();
        String body = "";

        Scanner s = new Scanner(decodedString);
        if (s.hasNextLine())
            commandLine = s.nextLine();

        //put all the headers to the list until the separation line
        while (s.hasNextLine()) {
            String headerLine = s.nextLine();
            if (!headerLine.isEmpty()) {

                String[] headerParam = headerLine.split(":", 2);
                Header header = new Header(headerParam[0], headerParam[1]);
                headersList.add(header);
            } else {
                break;
            }
        }
        if (s.hasNextLine()) {
            body = s.nextLine();

        }
        s.close();

        return new Frame(commandLine, headersList, body);
    }
}
