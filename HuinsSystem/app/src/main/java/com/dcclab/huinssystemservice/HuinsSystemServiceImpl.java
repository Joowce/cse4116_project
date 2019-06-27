package com.dcclab.huinssystemservice;

import android.content.Context;

import com.dcclab.huinssystemservice.broadcaster.HuinsInputSender;


public class HuinsSystemServiceImpl extends com.dcclab.huinssystemservice.IHuinsSystemService.Stub {
    private HuinsSystemController controller;
    private HuinsInputSender sender;
    private Context context;

    public HuinsSystemServiceImpl(Context context) {
        controller = HuinsSystemController.getInstance();
        controller.setHandler(new HuinsSystemController.HandlerSystemInput(){
            @Override
            public int handleInput(int input) {
                broadcast(input);
                return input < 20 ? 1 : 0;
            }
        });
        this.context = context;
        sender = new HuinsInputSender(context);
    }

    @Override
    public void start() {
        controller.start();
    }
    @Override
    public void stop() {
        controller.stop();
    }

    private void broadcast(int input) {
        sender.send(input);
    }
}
