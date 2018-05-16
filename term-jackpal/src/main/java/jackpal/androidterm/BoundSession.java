/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package jackpal.androidterm;

import android.os.ParcelFileDescriptor;
import android.text.TextUtils;

import jackpal.androidterm.util.TermSettings;

class BoundSession extends GenericTermSession {
    private final String issuerTitle;

    private boolean fullyInitialized;

    BoundSession(ParcelFileDescriptor ptmxFd, TermSettings settings, String issuerTitle) {
        super(ptmxFd, settings, true);

        this.issuerTitle = issuerTitle;

        setTermIn(new ParcelFileDescriptor.AutoCloseInputStream(ptmxFd));
        setTermOut(new ParcelFileDescriptor.AutoCloseOutputStream(ptmxFd));
    }

    @Override
    public String getTitle() {
        final String extraTitle = super.getTitle();

        return TextUtils.isEmpty(extraTitle)
                ? issuerTitle
                : issuerTitle + " — " + extraTitle;
    }

    @Override
    public void initializeEmulator(int columns, int rows) {
        super.initializeEmulator(columns, rows);

        fullyInitialized = true;
    }

    @Override
    boolean isFailFast() {
        return !fullyInitialized;
    }
}
