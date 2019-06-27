package com.dcclab.huinssystemservice.broadcaster;

import android.content.Context;
import android.content.Intent;

public class HuinsInputSender {
    public static final String HUINS_SYSTEM_SERVICE = "com.dcclab.huinssystemservice.broadcaster";
    public static final String HUINS_SYSTEM_INPUT = "com.dcclab.huinssystemservice.broadcaster.systeminput";
    Context context;

    public HuinsInputSender(Context context) {
        this.context = context;
    }

    public void send(int input) {
        Intent intent = new Intent(HUINS_SYSTEM_SERVICE);
        intent.putExtra(HUINS_SYSTEM_INPUT, input);
        context.sendBroadcast(intent);
    }
}
