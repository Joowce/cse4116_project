package com.dcclab.huinssystemservice;


public class HuinsSystemController {
    private HandlerSystemInput handler;
    private Runnable thread;

    private HuinsSystemController() {
        init();
    }

    static public HuinsSystemController getInstance() {
        return new HuinsSystemController();
    }

    public void setHandler(HandlerSystemInput handler) {
        this.handler = handler;
    }

    public void start() {
        if (thread != null) return;

        thread = new Runnable() {
            @Override
            public void run () {
                while(true) {
                    int input = listen();
                    if (handler.handleInput(input) <= 0) break;
                }
            }
        };
        thread.run();
    }

    public void stop() {
        end();
        thread = null;
    }


    private native void init();
    private native int listen();
    private native void end();

    public interface HandlerSystemInput {
        int handleInput(int input);
    }
}
