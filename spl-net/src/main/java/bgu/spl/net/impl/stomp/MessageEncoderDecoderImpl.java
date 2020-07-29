package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;


//public class MessageEncoderDecoderImpl<T> implements MessageEncoderDecoder<T> {

public class MessageEncoderDecoderImpl implements MessageEncoderDecoder<Frame> {

    private byte[] bytes = new byte[1 << 10]; //start with 1k
    private int len = 0;

    @Override
    public Frame decodeNextByte(byte nextByte) {
        //notice that the top 128 ascii characters have the same representation as their utf-8 counterparts
        //this allow us to do the following comparison
        if (nextByte == '\u0000') {
            return popFrame();
        }
        pushByte(nextByte);
        return null; //not a "frame" yet
    }

    @Override
    public byte[] encode(Frame message) {
        return (message + "\0").getBytes(); //uses utf8 by default
    }

    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }
        bytes[len++] = nextByte;
    }

    private Frame popFrame() {
        //notice that we explicitly requesting that the string will be decoded from UTF-8
        //this is not actually required as it is the default encoding in java.
        String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        Frame frame = Frame.stringToFrame(result);
        len = 0;
        return frame;

    }
}

