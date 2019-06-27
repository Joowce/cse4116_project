package com.android.server.huins;

import android.content.Context;
import android.content.Intent;

public class HuinsInputSender {
    public static final String HUINS_SYSTEM_SERVICE = "com.android.server.huins";
    public static final String HUINS_SYSTEM_INPUT = "com.android.server.huins.input";
    Context context;

    public HuinsInputSender(Context context) {
        this.context = context;
    }

    public void send(boolean[] input) {
        Intent intent = new Intent(HUINS_SYSTEM_SERVICE);
        intent.putExtra(HUINS_SYSTEM_INPUT, input);
        context.sendBroadcast(intent);
    }
}
