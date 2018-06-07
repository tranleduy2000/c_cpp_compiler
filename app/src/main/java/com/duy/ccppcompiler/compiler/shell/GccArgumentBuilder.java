package com.duy.ccppcompiler.compiler.shell;

import android.support.annotation.NonNull;
import android.support.v4.util.Pair;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;

public class GccArgumentBuilder {
    private String program;
    private ArrayList<Pair<Type, String>> mFlags = new ArrayList<>();

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

    private void addFlag(Type type, String name) {
        mFlags.add(new Pair<Type, String>(type, name));
    }

    public GccArgumentBuilder addFlags(Collection<String> flags) {
        addFlags(flags.toArray(new String[flags.size()]));
        return this;
    }


    @SuppressWarnings("ConstantConditions")
    @NonNull
    public String build() {
        StringBuilder cmd = new StringBuilder();
        cmd.append(program);
        if (mFlags.size() == 0) {
            return cmd.toString();
        }

        Collections.sort(mFlags, new Comparator<Pair<Type, String>>() {
            @SuppressWarnings("ConstantConditions")
            @Override
            public int compare(Pair<Type, String> o1, Pair<Type, String> o2) {
                if (!o1.first.getPriority().equals(o2.first.getPriority())) {
                    return o1.first.getPriority().compareTo(o2.first.getPriority());
                } else {
                    return o1.second.compareTo(o2.second);
                }
            }
        });
        Pair<Type, String> prev = null;
        for (int i = 0; i < mFlags.size(); i++) {
            final Pair<Type, String> flag = mFlags.get(i);
            final Type type = flag.first;
            final String name = flag.second;
            if (name != null && !name.isEmpty()) {
                if (prev != null) {
                    if (prev.equals(flag) && !type.isAcceptDuplicate()) {
                        continue;
                    }
                }
                if (cmd.length() != 0) {
                    cmd.append(" ");
                }
                cmd.append(flag);
                prev = flag;
            }
        }
        return cmd.toString();
    }


    enum Type {
        UNSPECIFIED(4, true),
        CPP_FLAG(3, true),
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
