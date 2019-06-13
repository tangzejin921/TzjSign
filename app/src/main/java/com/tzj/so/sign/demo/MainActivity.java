package com.tzj.so.sign.demo;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.tzj.so.tzjsign.UtilSign;

import java.security.MessageDigest;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = findViewById(R.id.sample_text);


        String sign = UtilSign.getSign(this);
        tv.setText(sign);
    }



    /**
     * 获取应用签名
     */
    public static String getSignature(Context context) {
        try {
            PackageInfo pis = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            return MD5(pis.signatures[0].toCharsString());
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * md5
     */
    public static String MD5(String str){
        try {
            byte[] btInput = str.getBytes();
            MessageDigest mdInst = MessageDigest.getInstance("MD5");
            mdInst.update(btInput);
            byte[] md = mdInst.digest();
            StringBuilder sb = new StringBuilder();
            for (byte aMd : md) {
                int val = ((int) aMd) & 0xff;
                if (val < 16)
                    sb.append("0");
                sb.append(Integer.toHexString(val));
            }
            return sb.toString().toLowerCase();
        } catch (Exception e) {
            throw new RuntimeException("MD5 操作失败:"+e.getMessage());
        }
    }
}
