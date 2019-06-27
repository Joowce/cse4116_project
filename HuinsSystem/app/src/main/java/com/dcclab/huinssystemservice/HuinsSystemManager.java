package com.dcclab.huinssystemservice;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

import com.dcclab.huinssystemservice.IHuinsSystemService;
import com.dcclab.huinssystemservice.broadcaster.HuinsInputSender;

public class HuinsSystemManager {
    private final static String TAG = HuinsSystemManager.class.getName();
    private final static String SERVICE_NAME = "HUINS_SYSTEM_SERVICE";
    private final IHuinsSystemService service;
    private static HuinsSystemManager huinsManager;

    public static synchronized HuinsSystemManager getHuinsSystemService() {
        if (huinsManager != null) {
            IBinder binder = android.os.ServiceManager.getService(SERVICE_NAME);
            if (binder != null) {
                IHuinsSystemService HuinsService = IHuinsSystemService.Stub.asInterface(binder);
                huinsManager = new HuinsSystemManager(HuinsService);
            }
        }
        return huinsManager;
    }

    private HuinsSystemManager(IHuinsSystemService service) {
        if(service == null){
            throw new IllegalArgumentException("Huins System service is null");
        }
        this.service = service;
    }

    public void startService () {
        try {
            service.start();
        } catch (android.os.RemoteException ex) {
            Log.e(TAG, "service cannot start", ex);
        }
    }

    public void stopService () {
        try {
            service.stop();
        } catch (android.os.RemoteException ex) {
            Log.e(TAG, "service cannot stop", ex);
        }
    }

    public HuinsInputReceiver getHuinsInputReceiverInstance(Context context) {
        HuinsInputReceiver receiver = new HuinsInputReceiver();
        context.registerReceiver(receiver, new IntentFilter(HuinsInputSender.HUINS_SYSTEM_SERVICE));
        return receiver;
    }


    public interface HandlerSystemInput {
        int handleInput(int input);
    }

    public class HuinsInputReceiver extends BroadcastReceiver {
        HandlerSystemInput handler;
        @Override
        public void onReceive(Context context, Intent intent) {
            int input = intent.getIntExtra(HuinsInputSender.HUINS_SYSTEM_INPUT, 0);
            handler.handleInput(input);
        }

        public void setHandler(HandlerSystemInput handler) {
            this.handler = handler;
        }
    }
}
