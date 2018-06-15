/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.commonsware.cwac.pager;

import android.os.Parcel;

public class SimplePageDescriptor implements PageDescriptor {
    public static final Creator<SimplePageDescriptor> CREATOR =
            new Creator<SimplePageDescriptor>() {
                public SimplePageDescriptor createFromParcel(Parcel in) {
                    return new SimplePageDescriptor(in);
                }

                public SimplePageDescriptor[] newArray(int size) {
                    return new SimplePageDescriptor[size];
                }
            };
    private String tag = null;
    private String title = null;

    public SimplePageDescriptor(String tag, String title) {
        this.tag = tag;
        this.title = title;
    }

    public SimplePageDescriptor(Parcel in) {
        tag = in.readString();
        title = in.readString();
    }

    @Override
    public int describeContents() {
        return (0);
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        out.writeString(tag);
        out.writeString(title);
    }

    public String getTitle() {
        return (title);
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getFragmentTag() {
        return (tag);
    }
}