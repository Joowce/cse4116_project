package com.example.secureatm.mainMenu;

import android.view.View;

import lombok.Builder;
import lombok.Getter;
import lombok.Setter;

@Builder
class Task {
    @Getter @Setter
    private String taskName;

    @Getter @Setter
    private View.OnClickListener clickListener;
}
