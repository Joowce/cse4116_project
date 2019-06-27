package com.dcclab.huinssystemservice;

import android.content.Context;

import com.dcclab.huinssystemservice.broadcaster.HuinsInputSender;


public class HuinsSystemServiceImpl extends com.dcclab.huinssystemservice.IHuinsSystemService.Stub {
    private HuinsSystemController controller;
    private HuinsInputSender sender;

    public HuinsSystemServiceImpl(Context context) {
        controller = HuinsSystemController.getInstance();
        controller.setHandler(new HuinsSystemController.HandlerSystemInput(){
            @Override
            public void handleInput(boolean[] input) {
                broadcast(input);
            }
        });
        sender = new HuinsInputSender(context);
    }

    @Override
    public void init() {
        controller.init();
    }

    @Override
    public void end() {
        controller.stop();
    }

    @Override
    public void startToListenSwitch() {
        controller.listenSwitchInput();
    }

    @Override
    public void writeDotMatrix(boolean[] matrix) {
        controller.writeDotMatrix(matrix);
    }

    @Override
    public void writeFND(int fnd) {
        controller.writeFND(fnd);
    }

    @Override
    public void writeLCD(String str) {
        controller.writeLCD(str);
    }

    @Override
    public void writeLED(boolean[] led) {
        controller.writeLED(led);
    }

    private void broadcast(boolean[] input) {
        sender.send(input);
    }
}
