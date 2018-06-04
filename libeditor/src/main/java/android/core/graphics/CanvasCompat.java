/*
 *
 *  * Copyright (C) 2006 The Android Open Source Project
 *  *
 *  * Licensed under the Apache License, Version 2.0 (the "License");
 *  * you may not use this file except in compliance with the License.
 *  * You may obtain a copy of the License at
 *  *
 *  *      http://www.apache.org/licenses/LICENSE-2.0
 *  *
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS,
 *  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  * See the License for the specific language governing permissions and
 *  * limitations under the License.
 *
 */

package android.core.graphics;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Build;
import android.support.annotation.NonNull;

import com.jecelyin.common.utils.ReflectionUtil;

import java.lang.reflect.InvocationTargetException;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class CanvasCompat {

    public static void drawTextRun(Canvas c, @NonNull char[] text, int index, int count, int contextIndex,
                                   int contextCount, float x, float y, boolean isRtl, @NonNull Paint paint) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            c.drawTextRun(text, index, count, contextIndex, contextCount, x, y, isRtl, paint);
        } else {
            try {
                //c.drawTextRun(text, index, count, contextIndex, contextCount, x, y, isRtl ? 1 : 0, paint);
                ReflectionUtil.callAny(c, "drawTextRun",
                        new Class[]{char[].class, int.class, int.class, int.class, int.class, float.class, float.class, int.class, Paint.class},
                        new Object[]{text, index, count, contextIndex, contextCount, x, y, isRtl ? 1 : 0, paint}
                );
            } catch (NoSuchMethodException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        }
    }


}
