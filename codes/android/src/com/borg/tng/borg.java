package com.borg.tng;

import android.app.AlertDialog;
import android.app.NativeActivity;
import android.os.Bundle;
import android.content.Intent;
import android.content.DialogInterface;

import android.util.Log;


public class borg extends NativeActivity
{
	public String mCommandLine = "";

    @Override
    protected void onCreate (Bundle savedInstanceState)
    {
        Intent launchIntent = getIntent();
        String extra = launchIntent.getStringExtra("arguments");
        if (extra != null) {
            mCommandLine = extra;
            Log.v("com.borg.tng", "command line = " + mCommandLine);
        }
        super.onCreate(savedInstanceState);

        Log.v("com.borg.tng", "Calling BorgNative onCreate");
    }
    // We call this function from native to display a toast string
    public void showAlert(String title, String contents, boolean exitApp)
    {
        // We need to use a runnable here to ensure that when the spawned
        // native_app_glue thread calls, we actually post the work to the UI
        // thread.  Otherwise, we'll likely get exceptions because there's no
        // prepared Looper on the native_app_glue main thread.
        final String finalTitle = title;
        final String finalContents = contents;
        final boolean finalExit = exitApp;
        runOnUiThread(new Runnable() {
            public void run()
            {
                AlertDialog.Builder builder = new AlertDialog.Builder(borg.this);
                builder.setMessage(finalContents)       
                        .setTitle(finalTitle)       
                        .setCancelable(true)
                        .setPositiveButton("OK", new DialogInterface.OnClickListener()
                    {           
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.cancel();
                            if (finalExit)
                            	borg.this.finish();
                        }       
                    });

                builder.create().show();
            }
        });
    }
    
   

}