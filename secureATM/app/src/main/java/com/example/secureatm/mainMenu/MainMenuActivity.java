package com.example.secureatm.mainMenu;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.RadioButton;

import com.example.secureatm.R;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

import android.os.HuinsSystemManager;


public class MainMenuActivity extends AppCompatActivity {

    private String LOG_TAG = MainMenuActivity.class.getName();
    private boolean[] ledPressed;
    private HuinsSystemManager huinsService;
    @BindView(R.id.FND_Input) private EditText FNDInput;
    @BindView(R.id.LCD_input) private EditText LCDInput;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        huinsService = (HuinsSystemManager) getApplicationContext()
                .getSystemService(Context.HUINS_SYSTEM_SERVICE);
        huinsService.init();
    }

    @OnClick({R.id.radioButton1, R.id.radioButton2, R.id.radioButton3, R.id.radioButton4,
            R.id.radioButton5, R.id.radioButton6, R.id.radioButton7, R.id.radioButton8})
    public void handleRadioButtons(View button) {
        RadioButton radioButton = (RadioButton) button;
        int id = Integer.parseInt(radioButton.getText().toString());
        ledPressed[id] = radioButton.isChecked();
        huinsService.writeLED(ledPressed);
    }

    @OnClick(R.id.FND_button)
    public void handleFND(View textIput) {
        int num = Integer.parseInt(((EditText) textIput).getText().toString());
        huinsService.writeFND(num);
    }

    @OnClick(R.id.LCD_input)
    public void handleLCD(View textInput) {
        String str = ((EditText) textInput).getText().toString();
        huinsService.writeLCD(str);
    }
}
