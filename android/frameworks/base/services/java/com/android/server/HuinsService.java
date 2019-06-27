package com.android.server;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.os.IhuinsService;
import android.util.Slog;
 
public class HuinsService extends IhuinsService.Stub
{
 
    private static final String TAG = "HuinsService";
    private Context mContext;
    private int mNativePointer;
 
    HuinsService(Context context){
        mContext = context;
        init_native();
    }
 
 
    @Override
    public void set_dot_matrix(int m) {
	    set_dot_matrix_native(m);
    }

    @Override
    public void set_fnd(int n) {
	    set_fnd_native(n);
    }

    @Override
    public void set_lcd(byte[] buf) {
	    set_lcd_native(buf);
    }

    @Override
    public void set_led(int bm) {
	    set_led_native(bm);
    }

    @Override
    public void set_buzzer(int buzz) {
	    set_buzzer_native(buzz);
    }

    @Override
    public void set_motor(int action, int direction, int speed) {
	    set_motor_native(action, direction, speed);
    }
 
    private static native long init_native();
    private static native void finalize_native(long ptr);
    private static native void set_dot_matrix_native(int m);
    private static native void set_fnd_native(int n);
    private static native void set_lcd_native(byte[] buf);
    private static native void set_led_native(int bm);
    private static native void set_buzzer_native(int buzz);
    private static native void set_motor_native(int action, int direction, int
    speed);
}
