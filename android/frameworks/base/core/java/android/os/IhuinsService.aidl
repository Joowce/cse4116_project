package android.os;

interface IhuinsService{
	void set_dot_matrix(int m);
	void set_fnd(int n);
	void set_lcd(in byte[] _buf);
	void set_led(int bm);
	void set_buzzer(int buzz);
	void set_motor(int action, int direction, int speed);
}
