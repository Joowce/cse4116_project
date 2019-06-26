package com.dcclab.blockservice.Timer;

import android.os.Handler;
import android.os.Message;

public class Timer extends Handler {
    private static final int MESSAGE_TIMER_START = 0;
    private static final int MESSAGE_TIMER_REPEAT = 1;
    private static final int MESSAGE_TIMER_END = -1;
    private static final int interval = 1000;

    private int seconds = 0;
    private RepeatedTask task = null;

    /**
     * callback method for message event
     * message type is start: start timer
     * repeat: run task and send message after interval
     * end: remove repeated message
     * @param msg received message
     */
    @Override
    public void handleMessage (Message msg) {
        switch (msg.what) {
            case MESSAGE_TIMER_START:
                seconds = 0;
                this.removeMessages(MESSAGE_TIMER_REPEAT);
                this.sendEmptyMessageDelayed(MESSAGE_TIMER_REPEAT, interval);
                break;
            case MESSAGE_TIMER_REPEAT:
                seconds++;
                if (task != null) task.run(seconds);
                this.sendEmptyMessageDelayed(MESSAGE_TIMER_REPEAT, interval);
                break;
            case MESSAGE_TIMER_END:
                this.removeMessages(MESSAGE_TIMER_REPEAT);
                break;
            default: break;
        }
    }

    /**
     * set task
     * @param task task called when timer tick
     */
    public void setTask(RepeatedTask task) {
        this.task = task;
    }

    /**
     * end timer
     * send end message
     */
    public void endTimer() {
        this.sendEmptyMessage(MESSAGE_TIMER_END);
    }

    /**
     * start timer
     * send start message
     */
    public void startTimer() {
        this.sendEmptyMessage(MESSAGE_TIMER_START);
    }
}