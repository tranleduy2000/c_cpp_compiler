package com.duy.ccppcompiler.compiler.shell;

import android.support.annotation.NonNull;
import android.support.v4.util.ArraySet;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;

public class GccArgumentBuilder extends ArgumentBuilder {
    private String program;
    private HashMap<Type, ArrayList<String>> mFlags = new HashMap<>();

    public GccArgumentBuilder(String program) {
        this.program = program;
    }

    public GccArgumentBuilder() {
        this.program = "";
    }

    public GccArgumentBuilder addFlags(String... flags) {
        for (String flag : flags) {
            addFlag(flag);
        }
        return this;
    }

    public GccArgumentBuilder addFlag(String flag) {
        addFlag(Type.UNSPECIFIED, flag);
        return this;
    }

    public GccArgumentBuilder addFlag(Type type, String name) {
        if (mFlags.get(type) == null) {
            mFlags.put(type, new ArrayList<String>());
        }
        mFlags.get(type).add(name);
        return this;
    }

    public GccArgumentBuilder addFlags(Collection<String> flags) {
        addFlags(flags.toArray(new String[flags.size()]));
        return this;
    }


    @NonNull
    public String build() {
        StringBuilder cmd = new StringBuilder();
        cmd.append(program);
        if (mFlags.size() == 0) {
            return cmd.toString();
        }
        for (Type type : Type.values()) {
            Collection<String> flags = mFlags.get(type);
            if (flags == null) {
                continue;
            }
            if (!type.isAcceptDuplicate()) {
                flags = new ArraySet<>(flags);
            }
            for (String flag : flags) {
                if (flag != null && !flag.isEmpty()) {
                    if (cmd.length() != 0) {
                        cmd.append(" ");
                    }
                    cmd.append(flag);
                }
            }
        }
        return cmd.toString();
    }


    public enum Type {
        UNSPECIFIED(4, true),
        CXX_FLAG(3, true),
        C_FLAG(2, true),
        LD_FLAG(1, false);

        private Integer priority;
        private boolean acceptDuplicate;

        Type(int priority, boolean acceptDuplicate) {
            this.priority = priority;
            this.acceptDuplicate = acceptDuplicate;
        }

        public Integer getPriority() {
            return priority;
        }

        public boolean isAcceptDuplicate() {
            return acceptDuplicate;
        }
    }
}
