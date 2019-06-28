// IHuinsSystemService.aidl
package android.os;

// Declare any non-default types here with import statements

interface IHuinsSystemService {
    void init();
    void end();

    void startToListenSwitch();

    void writeDotMatrix(in boolean[] matrix);
    void writeFND(int fnd);
    void writeLED(in boolean[] led);
    void writeLCD(String str);
    void writeBUZZER(int buzz);
    void writeMOTOR(int action, int direction, int speed);
}
