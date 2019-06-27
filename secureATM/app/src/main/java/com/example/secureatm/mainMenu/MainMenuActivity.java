package com.example.secureatm.mainMenu;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.TableLayout;

import com.example.secureatm.R;
import com.google.android.material.button.MaterialButton;

import butterknife.BindArray;
import butterknife.BindView;
import butterknife.ButterKnife;

public class MainMenuActivity extends AppCompatActivity {

    private String LOG_TAG = MainMenuActivity.class.getName();
    @BindView(R.id.menuContainer)
    public TableLayout menuContainer;
    @BindArray(R.array.tasks)
    public String[] tasks;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        setupMenuContent(tasks);
    }

    private void setupMenuContent(String[] tasks) {
        for (String task : tasks) {
            MaterialButton btn = new MaterialButton(this);
            btn.setGravity(Gravity.CENTER);
            btn.setText(task);
            menuContainer.addView(btn);
            btn.setOnClickListener((View view) -> {
                String btnText = ((Button) view).getText().toString();
                switch (btnText) {
                    case "":
                }
            });
        }
    }

}
