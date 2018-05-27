* Problem:    UNKNOWN
* Class:      MIP
* Rows:       2
* Columns:    2 (1 integer, 0 binary)
* Non-zeros:  4
* Format:     Fixed MPS
*
NAME
ROWS
 N  R0000000
 L  R0000001
 L  R0000002
COLUMNS
    M0000001  'MARKER'                 'INTORG'
    C0000001  R0000000             4   R0000001           0.7
    C0000001  R0000002           0.1
    M0000002  'MARKER'                 'INTEND'
    C0000002  R0000000             1   R0000001             1
    C0000002  R0000002         0.025
RHS
    RHS1      R0000001           630   R0000002           135
BOUNDS
 LO BND1      C0000002            20
 UP BND1      C0000002           500
ENDATA
