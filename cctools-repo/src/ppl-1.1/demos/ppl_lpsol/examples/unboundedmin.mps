NAME          from_lp_file
ROWS
 N  r_000000
 L  C1
 L  r_000002
COLUMNS
    MARK0000  'MARKER'                 'INTORG'
    x1        r_000000  1
    x1        C1        2
    x1        r_000002  -4
    x2        r_000000  -2
    x2        C1        -1
    x2        r_000002  -4
    MARK0001  'MARKER'                 'INTEND'
RHS
    RHS       C1        0
    RHS       r_000002  0
BOUNDS
 PL FOO       x1
 PL FOO       x2
ENDATA
