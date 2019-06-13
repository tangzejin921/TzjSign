package com.tzj.so.tzjsign;

import android.content.Context;

/**
 * Copyright © 2019 健康无忧网络科技有限公司<br>
 * Author:      唐泽金 tangzejin921@qq.com<br>
 * Version:     1.0.0<br>
 * Date:        2019-06-05 16:34<br>
 * Description: jni 接口
 */
public class UtilSign {

    /**
     * 获取签名
     */
    public native static String getSign(Context context);

    /**
     * MD5 计算
     */
    public native static String md5(String str);

}
