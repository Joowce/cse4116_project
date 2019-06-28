package android.os;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;
import android.os.IHuinsSystemService;


public class HuinsSystemManager {
    private final static String TAG = HuinsSystemManager.class.getName();
    private final static String SERVICE_NAME = "HUINS_SYSTEM_SERVICE";
    private final IHuinsSystemService service;
    private static HuinsSystemManager huinsManager;

    private boolean listeningState = false;

    public static synchronized HuinsSystemManager getHuinsSystemService() {
        if (huinsManager != null) {
            IBinder binder = android.os.ServiceManager.getService(SERVICE_NAME);
            if (binder != null) {
                IHuinsSystemService huinsService = IHuinsSystemService.Stub.asInterface(binder);
                huinsManager = new HuinsSystemManager(huinsService);
                try {
                    huinsService.init();
                } catch (android.os.RemoteException ex) {
                    Log.e(TAG, "Unable to initialize service");
                }
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

    public void startService() {
        try {
            if (!listeningState) service.startToListenSwitch();
        } catch (android.os.RemoteException ex) {
            Log.e(TAG, "service cannot start", ex);
        }
    }

    public void stopService () {
        try {
            service.end();
        } catch (android.os.RemoteException ex) {
            Log.e(TAG, "service cannot end", ex);
        }
    }

    public void writeDotMatrix(boolean[] matrix) {
        try{
            service.writeDotMatrix(matrix);
        } catch (android.os.RemoteException ex) {
            Log.e(TAG, "Unable to write Dot Matrix " + matrix.toString(), ex);
        }
    }

    public void writeFND(int fnd) {
        try{
            service.writeFND(fnd);
        }catch (android.os.RemoteException ex) {
            Log.e(TAG, "Unable to write fnd " + fnd, ex);
        }
    }

    public void writeLCD(String str) {
        try{
            service.writeLCD(str);
        }catch (android.os.RemoteException ex) {
            Log.e(TAG, "Unable to write fnd " + str, ex);
        }
    }


    public void writeLED(boolean[] led) {
        try{
            service.writeLED(led);
        }catch (android.os.RemoteException ex) {
            Log.e(TAG, "Unable to write led " + led.toString(), ex);
        }
    }


    public HuinsInputReceiver getHuinsInputReceiverInstance(Context context) {
        HuinsInputReceiver receiver = new HuinsInputReceiver();
        context.registerReceiver(receiver, new IntentFilter("com.android.server.huins"));
        return receiver;
    }


    public interface HandlerSystemInput {
        void handleInput(boolean[] input);
    }

    public class HuinsInputReceiver extends BroadcastReceiver {
        HandlerSystemInput handler;
        @Override
        public void onReceive(Context context, Intent intent) {
            boolean[] input = intent.getBooleanArrayExtra("com.android.server.huins.input");
            handler.handleInput(input);
        }

        public void setHandler(HandlerSystemInput handler) {
            this.handler = handler;
        }
    }
}
