package com.bitwitcher.bwbogie;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class ModeActivity extends Activity {

	private Button btnTracking, btnAvoidance, btnFree;
	private String address = null;
	private TextView textViewStatus;

	public static String EXTRA_ADDRESS = "device_address";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_mode);

		Intent intent = getIntent();
		address = intent.getStringExtra(MainActivity.EXTRA_ADDRESS);

		btnTracking = findViewById(R.id.buttonTracking);
		btnAvoidance = findViewById(R.id.buttonAvoidance);
		btnFree = findViewById(R.id.buttonFree);
		
		btnTracking.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(ModeActivity.this, TrackingActivity.class);
				intent.putExtra(EXTRA_ADDRESS, address);
				startActivity(intent);
			}
		});

		btnAvoidance.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(ModeActivity.this, AvoidanceActivity.class);
				intent.putExtra(EXTRA_ADDRESS, address);
				startActivity(intent);
			}
		});

		btnFree.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(ModeActivity.this, FreeActivity.class);
				intent.putExtra(EXTRA_ADDRESS, address);
				startActivity(intent);
			}
		});
	}
}
