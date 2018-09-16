package xyz.pallad.glenn.blueserial;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class MainActivity extends AppCompatActivity {

	ListView deviceList;

	private BluetoothAdapter myBluetooth = null;
	private Set<BluetoothDevice> pairedDevices;
	public static String EXTRA_ADDRESS = "device_address";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		deviceList = (ListView)findViewById(R.id.deviceList);

		myBluetooth = BluetoothAdapter.getDefaultAdapter();
		if ( myBluetooth==null ) {
			Toast.makeText(getApplicationContext(), "Bluetooth device not available", Toast.LENGTH_LONG).show();
			finish();
		} else if ( !myBluetooth.isEnabled() ) {
			Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(turnBTon, 1);
		}

		pairedDevicesList();
	}

	private void pairedDevicesList () {
		pairedDevices = myBluetooth.getBondedDevices();
		ArrayList list = new ArrayList();

		if ( pairedDevices.size() > 0 ) {
			for ( BluetoothDevice bt : pairedDevices ) {
				list.add(bt.getName().toString() + "\n" + bt.getAddress().toString());
			}
		} else {
			Toast.makeText(getApplicationContext(), "No Paired Bluetooth Devices Found.", Toast.LENGTH_LONG).show();
		}

		final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, list);
		deviceList.setAdapter(adapter);
		deviceList.setOnItemClickListener(myListClickListener);
	}

	private AdapterView.OnItemClickListener myListClickListener = new AdapterView.OnItemClickListener() {
		@Override
		public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
			String info = ((TextView) view).getText().toString();
			String address = info.substring(info.length()-17);

			Intent i = new Intent(MainActivity.this, ControlActivity.class);
			i.putExtra(EXTRA_ADDRESS, address);
			startActivity(i);
		}
	};
}
