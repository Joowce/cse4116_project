// IHuinsSystemService.aidl
package com.dcclab.huinssystemservice;

// Declare any non-default types here with import statements

interface IHuinsSystemService {
    void init();
    void end();

    void startToListenSwitch();

    void writeDotMatrix(boolean[] matrix);
    void writeFND(int fnd);
    void writeLED(boolean[] led);
    void writeLCD(String str);
}
