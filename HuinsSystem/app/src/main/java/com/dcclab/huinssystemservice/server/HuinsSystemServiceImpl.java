package com.dcclab.huinssystemservice.server;

import com.dcclab.huinssystemservice.HuinsSystemController;
import com.dcclab.huinssystemservice.IHuinsSystemService;

public class HuinsSystemServiceImpl extends IHuinsSystemService.Stub {
    private HuinsSystemController controller;

    private HuinsSystemServiceImpl() {
        controller = HuinsSystemController.getInstance();
        controller.setHandler(new HuinsSystemController.HandlerSystemInput(){
            @Override
            public int handleInput(int input) {
                broadcast(input);
                return input < 20 ? 1 : 0;
            }
        });
    }

    static HuinsSystemServiceImpl getInstance() {
        return new HuinsSystemServiceImpl();
    }

    @Override
    public void start() {
        controller.start();
    }
    @Override
    public void stop() {
        controller.stop();
    }

    private void broadcast(int input) {}
}
