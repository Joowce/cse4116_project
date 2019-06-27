package com.android.server.huins;

import android.util.Log;

public class HuinsSystemController {
    private static final String TAG = "HuinsSystemController";
    private HandlerSystemInput handler;
    private Runnable thread;

    private int inputFd;
    private int outputFd;

    private boolean listeningState;

    static public HuinsSystemController getInstance() {
        return new HuinsSystemController();
    }

    public void init() {
        inputFd = initInputDevices_native();
        outputFd = initOutputDevices_native();
    }

    public void setHandler(HandlerSystemInput handler) {
        this.handler = handler;
    }

    public void listenSwitchInput() {
        if (thread != null && listeningState) {
            Log.e(TAG, "Already listen switch input");
            return;
        }
        listeningState = true;
        thread = new Runnable() {
            private boolean[] prev;
            @Override
            public void run () {
                while(listeningState) {
                    boolean[] input = getSwitchStatus_native(inputFd);
                    for (int i = 0; i < 9; i++) {
                        if (prev[i] != input[i]) {
                            handler.handleInput(input);
                            break;
                        }
                        try{
                            thread.wait(300);
                        } catch (java.lang.InterruptedException ex) {
                            Log.e("Unable to thread wait", ex.getMessage());
                        }
                    }
                    prev = input;
                }
            }
        };
        thread.run();
    }

    public void writeDotMatrix(boolean[] matrix){
        writeDotMatrix_native(outputFd, matrix);
    }

    public void writeFND(int fnd) {
        writeFND_native(outputFd, fnd);
    }

    public void writeLCD(String str) {
        writeLCD_native(outputFd, str);
    }

    public void writeLED(boolean[] led) {
        writeLED_native(outputFd, led);
    }


    public void stop() {
        listeningState = false;

        endInputDevices_native(inputFd);
        endOutputDevices_native(outputFd);

        thread = null;
        inputFd = 0;
        outputFd = 0;
    }

//-------------------------------------------------------------------------------------

    private native int initInputDevices_native();
    private native int initOutputDevices_native();
    private native boolean[] getSwitchStatus_native(int fd);
    private native void writeDotMatrix_native(int fd, boolean[] matrix);
    private native void writeFND_native(int fd, int fnd);
    private native void writeLCD_native(int fd, String str);
    private native void writeLED_native(int fd, boolean[] led);
    private native void endInputDevices_native(int fd);
    private native void endOutputDevices_native(int fd);

    public interface HandlerSystemInput {
        void handleInput(boolean[] switches);
    }
}
