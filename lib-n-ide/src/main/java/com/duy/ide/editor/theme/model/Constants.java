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

package com.duy.ide.editor.theme.model;

public class Constants {
    public static final String C_PLUS_PLUS_SAMPLE = "/*\n" +
            " * Block comment \n" +
            " */\n" +
            "#include <vector>\n" +
            "\n" +
            "using namespace std;  // line comment\n" +
            "namespace foo {\n" +
            "\n" +
            "  typedef struct Struct {\n" +
            "    int field;\n" +
            "  } Typedef;\n" +
            "  enum Enum {Foo = 1, Bar = 2};\n" +
            "\n" +
            "  Typedef *globalVar;\n" +
            "  extern Typedef *externVar;\n" +
            "\n" +
            "  template<typename T, int N>\n" +
            "  class Class {\n" +
            "    T n;\n" +
            "  public:\n" +
            "    /**\n" +
            "     * Semantic highlighting:\n" +
            "     * Generated spectrum to pick colors for local variables and parameters:\n" +
            "     *  Color#1 SC1.1 SC1.2 SC1.3 SC1.4 Color#2 SC2.1 SC2.2 SC2.3 SC2.4 Color#3\n" +
            "     *  Color#3 SC3.1 SC3.2 SC3.3 SC3.4 Color#4 SC4.1 SC4.2 SC4.3 SC4.4 Color#5\n" +
            "     */\n" +
            "    void function(int param1, int param2, int param3) {\n" +
            "      int localVar1, localVar2, localVar3;\n" +
            "      int *localVar = new int[1];\n" +
            "      this->n = N;\n" +
            "      localVar1 = param1 + param2 + localVar3;\n" +
            "\n" +
            "    label:\n" +
            "      printf(\"Formatted string %d\\n\\g\", localVar[0]);\n" +
            "      printf(R\"**(Formatted raw-string %d\\n)**\", 1);\n" +
            "      std::cout << (1 << 2) << std::endl;  \n" +
            "\n" +
            "    #define FOO(A) A\n" +
            "    #ifdef DEBUG\n" +
            "      printf(\"debug\");\n" +
            "    #endif\n" +
            "    }\n" +
            "  };\n" +
            "}\n";
}
